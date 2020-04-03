#include "robot.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>

#include "params.hpp"

using namespace std;

Robot::Robot()
{
	x = y = angle = 0;
	h = 0.2;
	w = 0.2;
}

Robot::Robot(double x, double y, double angle) : Robot()
{
	this->x = x;
	this->y = y;
	this->angle = angle;
}

void Robot::render(cairo_t* cairo, uint sz, double alpha, Params const* params)
{
	cairo_save(cairo);
	cairo_scale(cairo, sz / CELL_SIZE, sz / CELL_SIZE);

	cairo_translate(cairo, x, y);
	cairo_rotate(cairo, angle);

	// Основа
	cairo_rectangle(cairo, -(w - 0.03) / 2, -h / 2, w - 0.03, h);
	cairo_set_source_rgba(cairo, 0.5, 0.5, 0.5, alpha);
	cairo_fill(cairo);

	// Дисплей
	cairo_rectangle(cairo, -0.08 / 2, (h - 0.05) / 2, 0.08, -0.05);
	cairo_set_source_rgba(cairo, 0.6, 0.8, 0.7, alpha);
	cairo_fill(cairo);

	// Датчики
	cairo_set_source_rgba(cairo, 0.8, 0.1, 0.1, alpha);
	for (const std::pair<double, double>& p : sensors)
		cairo_arc(cairo, p.first, p.second, 0.02, 0, 2 * M_PI);
	cairo_fill(cairo);

	// Колеса
	cairo_set_source_rgba(cairo, 0.2, 0.2, 0.2, alpha);
	cairo_rectangle(cairo, -w / 2, -0.06 / 2, 0.03, 0.06);
	cairo_fill(cairo);
	cairo_rectangle(cairo, w / 2, -0.06 / 2, -0.03, 0.06);
	cairo_fill(cairo);

	cairo_restore(cairo);
}

bool Robot::isRobot(double cx, double cy)
{
	double dx = cx - x, dy = cy - y;
	double r = dx * dx + dy * dy;

	// simple cases
	if (r * 4 > h * h + w * w) return false;
	double mi = min(h, w);
	if (r * 4 < mi * mi) return true;

	// precomputation
	double sinA = sin(angle), cosA = cos(angle);
	double pdx = cosA * w / 2 - sinA * h / 2;
	double pdy = sinA * w / 2 + cosA * h / 2;

	double px = dx - pdx, py = dy - pdy;
	// vector product: (px; py) x (sinA; -cosA)
	if (px * -cosA - py * sinA < 0) return false;
	// vector product: (px; py) x (-cosA; -sinA)
	if (px * -sinA - py * -cosA > 0) return false;

	px = dx + pdx, py = dy + pdy;
	// vector product: (px; py) x (cosA; sinA)
	if (px * sinA - py * cosA > 0) return false;
	// vector product: (px; py) x (-sinA; cosA)
	if (px * cosA - py * -sinA < 0) return false;

	return true;
}

double Robot::getX() { return x; }
double Robot::getY() { return y; }
double Robot::getAngle() { return angle; }

void Robot::moveTo(double x, double y, double angle)
{
	this->x = x;
	this->y = y;
	this->angle = angle;
}

void Robot::addSensor(double x, double y)
{
	sensors.push_back({x, y});
}
