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
	double mL, mR; // скорость вращения колес в рад/сек

	// Размеры
	double h, w; // height, width
	double wheel_w, wheel_r;

	std::vector<std::pair<double, double>> sensors; // Координаты сенсоров

public:
	Robot();
	Robot(double x, double y, double angle);

	void render(cairo_t* cairo, uint sz, double alpha, Params const* params);
	void update(double t); // Посчитать физику. Считать что за время t (сек) объекты не меняли скорость

	// Находится ли на координатах (сx, сy) робот
	bool isRobot(double cx, double cy); // в метрах

	double getX(); // в метрах
	double getY();
	double getAngle(); // в радианах

	void moveTo(double x, double y, double angle);
	void setSpeed(double mL, double mR); // скорость вращения колес в рад/сек

	void addSensor(double x, double y);
};
