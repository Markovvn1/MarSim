#pragma once

#include <stdlib.h>
#include <cairo/cairo.h>

#include "physics/robot.hpp"
#include "params.hpp"

class Cell
{
private:
	uint type;
	uint rot;

public:
	Cell();

	void onClick(bool right);

	void render(cairo_t* cairo, uint sz, Params const* params);
};
