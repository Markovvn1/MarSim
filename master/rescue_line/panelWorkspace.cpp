#include "panelWorkspace.hpp"

#include <X11/keysym.h>

#include "cell.hpp"
#include "rescue_line.hpp"
#include "utils/rate.hpp"
#include "utils/utils.hpp"
#include "utils/graphics.hpp"
#include "colors.hpp"

#define FIELD_SZ 200
#define FIELD_SMOOTH_K 0.7

using namespace std;

PanelWorkspace::PanelWorkspace() : IPanel()
{
	core = NULL;
	sz = 0;
	cells = NULL;
	pickRobot = pickControlCircle = activeRobot = false;
	pickDx = pickDy = pickDA = 0;
	rotRmin = rotRmax = 0;

	fieldSurface = NULL;
	fieldCairo = NULL;
	fieldData = NULL;
	fieldStride = 0;
}

PanelWorkspace::PanelWorkspace(RescueLine* core, IPanel* parent) : IPanel(parent)
{
	Params* params = core->getParams();

	this->core = core;
	sz = 0;
	cells = new Cell[params->sx * params->sy];
	pickRobot = pickControlCircle = activeRobot = false;
	pickDx = pickDy = pickDA = 0;
	rotRmin = rotRmax = 0;

	fieldSurface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, params->sx * FIELD_SZ, params->sy * FIELD_SZ);
	fieldCairo = cairo_create(fieldSurface);
	fieldStride = cairo_image_surface_get_stride(fieldSurface);
	fieldData = cairo_image_surface_get_data(fieldSurface);

	updateFieldForSensors();
}

PanelWorkspace::~PanelWorkspace()
{

}

#define COLOR_BACKGROUND 0.95, 0.95, 0.95
#define COLOR_GRID 0.32, 0.32, 0.32

void PanelWorkspace::onStart()
{
	pickRobot = pickControlCircle = activeRobot = false;

	updateFieldForSensors();
}

void PanelWorkspace::onStop()
{

}

void PanelWorkspace::eventRender(cairo_t* cairo)
{
	Params* params = core->getParams();

	cairo_save(cairo);
	cairo_translate(cairo, field.x, field.y);

	cairo_rectangle(cairo, 0, 0, field.width, field.height);
	cairo_clip(cairo);
	cairo_new_path(cairo);

	drawField(cairo, sz);

	// Render grid
	cairo_set_line_width(cairo, 1);

	for (uint i = 1; i < params->sy; i++)
	{
		uint y = field.height * i / params->sy;
		cairo_move_to(cairo, 0, y + 0.5);
		cairo_line_to(cairo, field.width, y + 0.5);
	}

	for (uint i = 1; i < params->sx; i++)
	{
		uint x = field.width * i / params->sx;
		cairo_move_to(cairo, x + 0.5, 0);
		cairo_line_to(cairo, x + 0.5, field.height);
	}

	cairo_set_source_rgb(cairo, COLOR_GRID);
	cairo_stroke(cairo);

	// Render robot
	params->robot.render(cairo, sz, pickRobot ? 0.4 : 0.8);
	if (activeRobot)
	{
		cairo_arc(
				cairo, params->robot.getX() / CELL_SIZE * sz,
				field.height - params->robot.getY() / CELL_SIZE * sz,
				(rotRmin + rotRmax) / 2, 0, 2 * M_PI);
		cairo_set_source_rgba(cairo, 0.7, 0.1, 0.1, pickControlCircle ? 0.2 : 0.4);
		cairo_set_line_width(cairo, rotRmax - rotRmin);
		cairo_stroke(cairo);
	}

	cairo_reset_clip(cairo);

	cairo_restore(cairo);

	if (core->isActive()) setRender(); // animation
}

void PanelWorkspace::eventMouse(const EventMouse& event)
{
	if (core->isActive()) return; // Не принимаем никакие команды пока работает симуляция

	Params* params = core->getParams();

	// Обработка поднимания кольца
	if (event.getButton() == M_BUTTON_L_DOWN && activeRobot && !pickControlCircle)
	{
		double dx = event.x - field.x - params->robot.getX() / CELL_SIZE * sz;
		double dy = event.y - field.y - (field.height - params->robot.getY() / CELL_SIZE * sz);
		double d = dx * dx + dy * dy;
		if (d > rotRmin * rotRmin && d < rotRmax * rotRmax)
		{
			pickControlCircle = true;
			pickDA = params->robot.getAngle() + atan2(dy, dx);
			setRender();
		}
	}

	// Обработка активации (+ понимание) и деактивации робота
	if ((event.getButtonEvent() == M_BUTTON_EVENT_DOWN && activeRobot && !pickControlCircle) ||
			(event.getButton() == M_BUTTON_L_DOWN && !activeRobot))
	{
		double realX = (event.x - field.x) * CELL_SIZE / sz;
		double realY = (field.height - (event.y - field.y)) * CELL_SIZE / sz;
		bool isRobot = params->robot.isRobot(realX, realY);

		if (isRobot && event.getButton() == M_BUTTON_L_DOWN)
		{
			pickRobot = true;
			activeRobot = true;
			pickDx = params->robot.getX() - realX;
			pickDy = params->robot.getY() - realY;
			setRender();
		}

		if (!isRobot && activeRobot)
		{
			activeRobot = false;
			setRender();
			return;
		}
	}

	// Обработка отпускания робота
	if (event.getButton() == M_BUTTON_L_UP || event.getEvent() == M_LEAVE)
	{
		if (pickRobot)
		{
			pickRobot = false;
			setRender();
		}
		if (pickControlCircle)
		{
			pickControlCircle = false;
			setRender();
		}
	}

	// Обработка перемещения робота
	if (pickRobot)
	{
		double realX = (event.x - field.x) * CELL_SIZE / sz;
		double realY = (field.height - (event.y - field.y)) * CELL_SIZE / sz;

		params->robot.moveTo(realX + pickDx, realY + pickDy, params->robot.getAngle());

		setRender();
	}

	// Обработка перемещение кольца
	if (pickControlCircle)
	{
		double dx = event.x - field.x - params->robot.getX() / CELL_SIZE * sz;
		double dy = event.y - field.y - (field.height - params->robot.getY() / CELL_SIZE * sz);

		params->robot.moveTo(params->robot.getX(), params->robot.getY(), pickDA - atan2(dy, dx));

		setRender();
	}

	// Обработка редактирования поля
	if ((event.getButton() == M_BUTTON_L_DOWN || event.getButton() == M_BUTTON_R_DOWN) && !activeRobot)
	{
		int x = (event.x - field.x) / sz;
		int y = (event.y - field.y) / sz;
		if (x >= 0 && y >= 0 && x < (int)params->sx && y < (int)params->sy)
		{
			cells[y * params->sx + x].onClick(event.getButton() == M_BUTTON_R_DOWN);

			setRender();
		}
	}
}

void PanelWorkspace::eventKeyboard(const EventKeyboard& event)
{
	if (event.keysym == XK_space && event.type == K_KEYDOWN)
	{
		if (core->isActive())
			core->stop();
		else
			core->start();
	}
}

void PanelWorkspace::eventReshape(const Rect& newRect)
{
	Params* params = core->getParams();

	sz = min(newRect.width / params->sx, newRect.height / params->sy);
	rotRmin = sz * 0.7;
	rotRmax = sz * 0.9;

	field.width = sz * params->sx;
	field.height = sz * params->sy;
	field.x = newRect.x + (newRect.width - field.width) / 2;
	field.y = newRect.y + (newRect.height - field.height) / 2;
}


void PanelWorkspace::drawField(cairo_t* cairo, uint sz)
{
	Params* params = core->getParams();

	cairo_rectangle(cairo, 0, 0, sz * params->sx, sz * params->sy);
	cairo_set_source_rgb(cairo, COLOR_BACKGROUND);
	cairo_fill(cairo);

	// Render cells
	for (uint y = 0; y < params->sy; y++)
	{
		for (uint x = 0; x < params->sx; x++)
		{
			cells[y * params->sx + x].render(cairo, sz, params);
			cairo_translate(cairo, sz, 0);
		}
		cairo_translate(cairo, -sz * params->sx, sz);
	}
	cairo_translate(cairo, 0, -sz * params->sy);
}

void PanelWorkspace::updateFieldForSensors()
{
	Params* params = core->getParams();

	drawField(fieldCairo, FIELD_SZ);
	cairo_surface_flush(fieldSurface);

	float r = FIELD_SZ / (float)CELL_SIZE * FIELD_SMOOTH_K * params->line_thickness;
	gaussianBlur(fieldData, params->sx * FIELD_SZ, params->sy * FIELD_SZ, fieldStride, r);
}

void PanelWorkspace::updateParams()
{
	eventReshape(rect);
}

int PanelWorkspace::getColor(double x, double y)
{
	Params* params = core->getParams();

	int width = params->sx * FIELD_SZ;
	int height = params->sy * FIELD_SZ;

	x = x / CELL_SIZE * FIELD_SZ;
	y = height - y / CELL_SIZE * FIELD_SZ;

	if (x < 0.5) x = 0.5;
	if (y < 0.5) y = 0.5;
	if (x > width - 0.5) x = width - 0.5;
	if (y > height - 0.5) x = height - 0.5;

	int x0 = x, y0 = y;

	// 0 1    1 0    2 3    3 2
	// 2 3 or 3 2 or 0 1 or 1 0
	uint8_t* pix0 = fieldData + y0 * fieldStride + x0 * 4;
	uint8_t* pix1 = pix0, *pix2 = pix0, *pix3 = pix0;

	double cx = x0 + 0.5, cy = y0 + 0.5; // Центр клетки

	if (x < cx)
	{
		if (x0 > 0) { pix1 -= 4; pix3 -= 4; }
	}
	else
	{
		if (x0 < width - 1) { pix1 += 4; pix3 += 4; }
	}

	if (y < cy)
	{
		if (y0 > 0) { pix2 -= fieldStride; pix3 -= fieldStride; }
	}
	else
	{
		if (y0 < height - 1) { pix2 += fieldStride; pix3 += fieldStride; }
	}

	float wx = abs(cx - x), wy = abs(cy - y);
	float w0 = (1 - wx) * (1 - wy);
	float w1 =    wx    * (1 - wy);
	float w2 = (1 - wx) *    wy;
	float w3 =    wx    *    wy;

	int res = 0xffffffff;

	((uint8_t*)&res)[0] = pix0[0] * w0 + pix1[0] * w1 + pix2[0] * w2 + pix3[0] * w3;
	((uint8_t*)&res)[1] = pix0[1] * w0 + pix1[1] * w1 + pix2[1] * w2 + pix3[1] * w3;
	((uint8_t*)&res)[2] = pix0[2] * w0 + pix1[2] * w1 + pix2[2] * w2 + pix3[2] * w3;

	return res;
}

int PanelWorkspace::getMinWight() const
{
	return 320;
}

int PanelWorkspace::getMinHeight() const
{
	return 240;
}
