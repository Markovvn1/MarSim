#pragma once

#include <stdlib.h>
#include <cairo/cairo.h>
#include <vector>

struct Params;

class Robot
{
private:
	double x, y;
	double angle;

	double h, w; // height, width

	std::vector<std::pair<double, double>> sensors; // Координаты сенсоров

public:
	Robot();
	Robot(double x, double y, double angle);

	void render(cairo_t* cairo, uint sz, double alpha, Params const* params);

	// Находится ли на координатах (сx, сy) робот
	bool isRobot(double cx, double cy); // в метрах

	double getX(); // в метрах
	double getY();
	double getAngle(); // в радианах

	void moveTo(double x, double y, double angle);

	void addSensor(double x, double y);
};
