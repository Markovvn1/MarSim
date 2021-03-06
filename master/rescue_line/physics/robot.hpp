#pragma once

#include <stdlib.h>
#include <cairo/cairo.h>
#include <vector>
#include <stdint.h>

struct Params;

typedef std::vector<std::pair<double, double>> Sensors;

class Robot
{
private:
	Params* params;
	double x, y;
	double angle;
	bool needUpdateSinCos; // Если угол изменился
	double sinA, cosA;
	double mL, mR; // скорость вращения колес в рад/сек

	// Размеры
	double h, w; // height, width
	double wheel_w, wheel_r;

	Sensors sensors; // Координаты сенсоров

	void updateSinCos();
	void checkPos(); // Проверка выхода за границы, нормализация угла

public:
	Robot();
	Robot(double x, double y, double angle, Params* params);

	void render(cairo_t* cairo, uint sz, double alpha);
	void update(double t); // Посчитать физику. Считать что за время t (сек) объекты не меняли скорость

	// Находится ли на координатах (сx, сy) робот
	bool isRobot(double cx, double cy); // в метрах

	double getX(); // в метрах
	double getY();
	double getAngle(); // в радианах

	void moveTo(double x, double y, double angle);
	void setSpeed(double mL, double mR); // скорость вращения колес в рад/сек

	int lenSensors();
	std::pair<double, double> getSensor(int i);
	void addSensor(double x, double y);
};
