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
		rot = (rot + 1) % 4;
	else
		type = (type + 1) % 5;
}

#define COLOR_LINE 0.05, 0.05, 0.05

void Cell::render(cairo_t* cairo, uint sz, Params const* params)
{
	if (type == 0) return;

	cairo_set_source_rgb(cairo, COLOR_LINE);

	cairo_save(cairo);
	cairo_scale(cairo, sz, sz);

	// mask
//	cairo_rectangle(cairo, 0, 0, 1, 1);
//	cairo_clip (cairo);
//	cairo_new_path (cairo);

	// rotate
	if (rot != 0)
	{
		cairo_translate(cairo, 0.5, 0.5);
		cairo_rotate(cairo, M_PI * rot / 2);
		cairo_translate(cairo, -0.5, -0.5);
	}

	cairo_set_line_width(cairo, params->line_thickness / CELL_SIZE);

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
	}

	cairo_restore(cairo);
//	cairo_reset_clip(cairo);
}
