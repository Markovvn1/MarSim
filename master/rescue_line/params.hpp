#pragma once

#include "physics/robot.hpp"

#define CELL_SIZE 0.3

struct Params
{
	Robot robot;

	double line_thickness = 0.015;
	uint sx = 8, sy = 6; // Количество ячеек по x и по y
};
