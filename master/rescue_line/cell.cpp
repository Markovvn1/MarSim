#include "cell.hpp"

#include <cmath>

Cell::Cell()
{
	type = 0;
	rot = 0;
}

void Cell::onClick(bool right)
{
	if (right)
	{
		rot += 1;
	}
	else
	{
		type = (type + 1) % 7;
		rot = 0;
	}
}

#define COLOR_LINE 0.05, 0.05, 0.05
#define COLOR_GREEN 0.05, 0.55, 0.02

void Cell::render(cairo_t* cairo, uint sz, Params* params)
{
	if (type == 0) return;

	cairo_save(cairo);
	cairo_scale(cairo, sz, sz);

	// mask
//	cairo_rectangle(cairo, 0, 0, 1, 1);
//	cairo_clip (cairo);
//	cairo_new_path (cairo);

	// rotate
	if (rot % 4 != 0)
	{
		cairo_translate(cairo, 0.5, 0.5);
		cairo_rotate(cairo, M_PI * (rot % 4) / 2);
		cairo_translate(cairo, -0.5, -0.5);
	}

	float line_width = params->line_thickness / CELL_SIZE;
	cairo_set_line_width(cairo, line_width);
	cairo_set_source_rgb(cairo, COLOR_LINE);

	switch (type)
	{
	case 1:
		cairo_move_to(cairo, 0, 0.5);
		cairo_line_to(cairo, 1, 0.5);
		cairo_stroke(cairo);
		break;
	case 2:
	{
		double dash[2] = {2./6, 1./6};
		cairo_set_dash(cairo, dash, 2, 1./6);
		cairo_move_to(cairo, 0, 0.5);
		cairo_line_to(cairo, 1, 0.5);
		cairo_stroke(cairo);
		break;
	}
	case 3:
		cairo_move_to(cairo, 0, 0.5);
		cairo_line_to(cairo, 0.5, 0.5);
		cairo_line_to(cairo, 0.5, 0);
		cairo_stroke(cairo);
		break;
	case 4:
		cairo_arc(cairo, 0, 0, 0.5, 0, M_PI / 2);
		cairo_stroke(cairo);
		break;
	case 5:
		cairo_set_source_rgb(cairo, COLOR_GREEN);
		switch ((rot / 4) % 3)
		{
		case 0:
			cairo_rectangle(cairo, 0.5 - line_width * 1.5, 0.5 - line_width * 1.5, line_width, line_width);
			break;
		case 1:
			cairo_rectangle(cairo, 0.5 + line_width * 0.5, 0.5 - line_width * 1.5, line_width, line_width);
			break;
		case 2:
			cairo_rectangle(cairo, 0.5 - line_width * 1.5, 0.5 - line_width * 1.5, line_width, line_width);
			cairo_rectangle(cairo, 0.5 + line_width * 0.5, 0.5 - line_width * 1.5, line_width, line_width);
			break;
		}
		cairo_fill(cairo);

		cairo_set_source_rgb(cairo, COLOR_LINE);
		cairo_move_to(cairo, 0, 0.5);
		cairo_line_to(cairo, 1, 0.5);
		cairo_move_to(cairo, 0.5, 0.5);
		cairo_line_to(cairo, 0.5, 0);
		cairo_stroke(cairo);
		break;
	case 6:
		cairo_set_source_rgb(cairo, COLOR_GREEN);
		if ((rot / 4) % 2 == 0)
		{
			cairo_rectangle(cairo, 0.5 - line_width * 1.5, 0.2 - line_width * 1.5 + 0.005, line_width, line_width);
			cairo_rectangle(cairo, 0.2 - line_width * 1.5 + 0.005, 0.5 - line_width * 1.5, line_width, line_width);
		}
		else
		{
			cairo_rectangle(cairo, 0.5 + line_width * 0.5, 0.2 - line_width * 1.5 + 0.005, line_width, line_width);
			cairo_rectangle(cairo, 0.2 - line_width * 1.5 + 0.005, 0.5 + line_width * 0.5, line_width, line_width);
		}
		cairo_fill(cairo);

		cairo_set_source_rgb(cairo, COLOR_LINE);
		cairo_arc(cairo, 0.5, 0.5, 0.3, 0, M_PI * 2);
		cairo_move_to(cairo, 0.5, 0);
		cairo_line_to(cairo, 0.5, 0.2);
		cairo_move_to(cairo, 0, 0.5);
		cairo_line_to(cairo, 0.2, 0.5);
		cairo_stroke(cairo);
	}

	cairo_restore(cairo);
//	cairo_reset_clip(cairo);
}
