#include "panelWorkspace.hpp"

#include "../../../utils/utils.hpp"
#include "../../colors.hpp"
#include <iostream>

using namespace std;

PanelWorkspace::PanelWorkspace() : IPanel()
{
	sz = 0;
	sx = 0; sy = 0;
	cells = NULL;
	pickRobot = pickControlCircle = activeRobot = false;
	pickDx = pickDy = pickDA = 0;
	rotRmin = rotRmax = 0;
}

PanelWorkspace::PanelWorkspace(Params* params, IPanel* parent) : IPanel(params, parent)
{
	sz = 0;
	sx = 8; sy = 6;
	cells = new Cell[sx * sy];
	pickRobot = pickControlCircle = activeRobot = false;
	pickDx = pickDy = pickDA = 0;
	rotRmin = rotRmax = 0;
}

PanelWorkspace::~PanelWorkspace()
{

}

#define COLOR_BACKGROUND 0.95, 0.95, 0.95
#define COLOR_GRID 0.32, 0.32, 0.32

void PanelWorkspace::eventRender(cairo_t* cairo)
{
	cairo_save(cairo);
	cairo_translate(cairo, rect.x, rect.y);

	cairo_rectangle(cairo, 0, 0, rect.width, rect.height);
	cairo_clip(cairo);
	cairo_new_path(cairo);
	cairo_rectangle(cairo, 0, 0, rect.width, rect.height);
	cairo_set_source_rgb(cairo, COLOR_BACKGROUND);
	cairo_fill(cairo);

	// Render cells
	for (uint y = 0; y < sy; y++)
	{
		for (uint x = 0; x < sx; x++)
		{
			cells[y * sx + x].render(cairo, sz, params);
			cairo_translate(cairo, sz, 0);
		}
		cairo_translate(cairo, -sz * sx, sz);
	}
	cairo_translate(cairo, 0, -sz * sy);


	// Render grid
	cairo_set_line_width(cairo, 1);

	for (uint i = 1; i < sy; i++)
	{
		uint y = rect.height * i / sy;
		cairo_move_to(cairo, 0, y + 0.5);
		cairo_line_to(cairo, rect.width, y + 0.5);
	}

	for (uint i = 1; i < sx; i++)
	{
		uint x = rect.width * i / sx;
		cairo_move_to(cairo, x + 0.5, 0);
		cairo_line_to(cairo, x + 0.5, rect.height);
	}

	cairo_set_source_rgb(cairo, COLOR_GRID);
	cairo_stroke(cairo);

	// Render robot
	params->robot.render(cairo, sz, pickRobot ? 0.4 : 0.8, params);
	if (activeRobot)
	{
		cairo_arc(
				cairo, params->robot.getX() / CELL_SIZE * sz,
				params->robot.getY() / CELL_SIZE * sz,
				(rotRmin + rotRmax) / 2, 0, 2 * M_PI);
		cairo_set_source_rgba(cairo, 0.7, 0.1, 0.1, pickControlCircle ? 0.2 : 0.4);
		cairo_set_line_width(cairo, rotRmax - rotRmin);
		cairo_stroke(cairo);
	}

	cairo_reset_clip(cairo);

	cairo_restore(cairo);
}

void PanelWorkspace::eventMouse(const EventMouse& event)
{
	// Обработка поднимания кольца
	if (event.getButton() == M_BUTTON_L_DOWN && activeRobot && !pickControlCircle)
	{
		double dx = event.x - rect.x - params->robot.getX() / CELL_SIZE * sz;
		double dy = event.y - rect.y - params->robot.getY() / CELL_SIZE * sz;
		double d = dx * dx + dy * dy;
		if (d > rotRmin * rotRmin && d < rotRmax * rotRmax)
		{
			pickControlCircle = true;
			pickDA = atan2(dy, dx) - params->robot.getAngle();
			setRender();
		}
	}

	// Обработка активации (+ понимание) и деактивации робота
	if ((event.getButtonEvent() == M_BUTTON_EVENT_DOWN && activeRobot && !pickControlCircle) ||
			(event.getButton() == M_BUTTON_L_DOWN && !activeRobot))
	{
		double realX = (event.x - rect.x) * CELL_SIZE / sz;
		double realY = (event.y - rect.y) * CELL_SIZE / sz;
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
		double realX = (event.x - rect.x) * CELL_SIZE / sz;
		double realY = (event.y - rect.y) * CELL_SIZE / sz;

		params->robot.moveTo(realX + pickDx, realY + pickDy, params->robot.getAngle());

		setRender();
	}

	// Обработка перемещение кольца
	if (pickControlCircle)
	{
		double dx = event.x - rect.x - params->robot.getX() / CELL_SIZE * sz;
		double dy = event.y - rect.y - params->robot.getY() / CELL_SIZE * sz;

		params->robot.moveTo(params->robot.getX(), params->robot.getY(), atan2(dy, dx) - pickDA);

		setRender();
	}

	// Обработка редактирования поля
	if ((event.getButton() == M_BUTTON_L_DOWN || event.getButton() == M_BUTTON_R_DOWN) && !activeRobot)
	{
		int x = (event.x - rect.x) / sz;
		int y = (event.y - rect.y) / sz;

		cells[y * sx + x].onClick(event.getButton() == M_BUTTON_R_DOWN);

		setRender();
	}
}

void PanelWorkspace::eventKeyboard(const EventKeyboard& event)
{
//	wcout << event << endl;
}

void PanelWorkspace::eventReshape(const Rect& newRect)
{
	sz = newRect.height / sy;
	rotRmin = sz * 0.7;
	rotRmax = sz * 0.9;
}



uint PanelWorkspace::sizeX()
{
	return sx;
}

uint PanelWorkspace::sizeY()
{
	return sy;
}

int PanelWorkspace::getMinWight() const
{
	return 40 * sx;
}

int PanelWorkspace::getMinHeight() const
{
	return 40 * sy;
}
