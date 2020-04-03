#pragma once

#include "physics/robot.hpp"

#define CELL_SIZE 0.3

struct Params
{
	bool physics_active = false;
	double line_thickness = 0.015;
	Robot robot;
//	double fieldSizeX = 0, fieldSizeY = 0;
};
