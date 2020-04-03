#include "panelControl.hpp"

#include "utils/utils.hpp"
#include "colors.hpp"
#include <iostream>

using namespace std;

PanelControl::PanelControl() : IPanel()
{
	sz = 0;
}

PanelControl::PanelControl(IPanel* parent) : IPanel(parent)
{
	sz = 0;
	data[0] = false;
	state[0] = 0;
}

PanelControl::~PanelControl()
{

}

#define IMG_SCALE 0.8
#define COLOR_MAIN COLOR_PANEL_MAIN
#define COLOR_ON_MOUSE 0.31, 0.31, 0.31
#define COLOR_BORDER 0.24, 0.24, 0.24

#define COLOR1_MAIN 0.1, 0.7, 0.2
#define COLOR2_MAIN 0.7, 0.13, 0.13

void PanelControl::eventRender(cairo_t* cairo)
{
	cairo_save(cairo);
	cairo_translate(cairo, rect.x, rect.y);

	for (int x = 0; x < 1; x++)
	{
		cairo_rectangle(cairo, x * sz, 0, sz, sz);
		if (state[0] == 0)
			cairo_set_source_rgb(cairo, COLOR_MAIN);
		else
			cairo_set_source_rgb(cairo, COLOR_ON_MOUSE);
		cairo_fill(cairo);

		if (state[0] == 2)
		{
			cairo_move_to(cairo, (x + 1) * sz, 0.5);
			cairo_line_to(cairo, x * sz + 0.5, 0.5);
			cairo_line_to(cairo, x * sz + 0.5, sz);
			cairo_set_line_width(cairo, 1);
			cairo_set_source_rgb(cairo, COLOR_BORDER);
			cairo_stroke(cairo);
		}
	}

	cairo_scale(cairo, sz, sz);

	cairo_translate(cairo, 0.5, 0.5);

	if (!data[0])
	{
		#define t1 (0.4330127018922193 * IMG_SCALE)
		cairo_move_to(cairo, -t1, IMG_SCALE / 2);
		cairo_line_to(cairo, -t1, -IMG_SCALE / 2);
		cairo_line_to(cairo, t1, 0);
		cairo_close_path(cairo);

		cairo_set_source_rgb(cairo, COLOR1_MAIN);
		cairo_fill(cairo);
	}
	else
	{
		#define t2 (0.3 * IMG_SCALE)
		cairo_move_to(cairo, -(IMG_SCALE - t1) / 2, IMG_SCALE / 2);
		cairo_line_to(cairo, -(IMG_SCALE - t1) / 2, -IMG_SCALE / 2);
		cairo_move_to(cairo, (IMG_SCALE - t1) / 2, IMG_SCALE / 2);
		cairo_line_to(cairo, (IMG_SCALE - t1) / 2, -IMG_SCALE / 2);

		cairo_set_line_width(cairo, t2);
		cairo_set_source_rgb(cairo, COLOR2_MAIN);
		cairo_stroke(cairo);
	}

	cairo_restore(cairo);
}

void PanelControl::eventMouse(const EventMouse& event)
{
	int x = (event.x - rect.x) / sz;

	if (event.inRect(rect) && x < 1)
	{
		if (state[x] == 0) state[x] = 1;
		if (event.getButton() == M_BUTTON_L_DOWN) state[x] = 2;
		if (event.getButton() == M_BUTTON_L_UP)
		{
			state[x] = 1;
			data[x] = !data[x];
		}

		setRender();

		return;
	}
	else
	{
		for (int x = 0; x < 1; x++)
		{
			if (state[x] != 0) setRender();
			state[x] = 0;
		}
	}
}

void PanelControl::eventKeyboard(const EventKeyboard& event)
{

}

void PanelControl::eventReshape(const Rect& newRect)
{
	sz = newRect.height;
}


int PanelControl::getMinWight() const
{
	return 30;
}

int PanelControl::getMinHeight() const
{
	return 30;
}
