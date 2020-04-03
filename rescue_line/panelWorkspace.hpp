#pragma once

#include "utils/utils.hpp"
#include "window/panel.hpp"

#include "cell.hpp"
#include "params.hpp"
#include "physics/robot.hpp"

class PanelWorkspace : public IPanel
{
private:
	Params* params;
	Cell* cells;
	uint sz; // Размер одной ячейки
	uint sx, sy; // Количество ячеек по x и по y
	Rect field;

	// Взаимодействие с роботом
	bool pickRobot; // Подобрал робота (держит его мышкой)
	bool pickControlCircle; // Подобрал кольцо вокруг робота
	bool activeRobot; // Робот недавно был затронут (показывается кольцо вокруг робота)
	double pickDx, pickDy, pickDA;
	double rotRmin, rotRmax; // Радиусы кольца вокруг робота

protected:
	virtual void eventRender(cairo_t* cairo);
	virtual void eventMouse(const EventMouse& event); // Абсолютные координаты
	virtual void eventKeyboard(const EventKeyboard& event);
	virtual void eventReshape(const Rect& newRect); // Абсолютные координаты

public:
	PanelWorkspace();
	PanelWorkspace(IPanel* parent, Params* params);
	virtual ~PanelWorkspace();

	uint sizeX();
	uint sizeY();

	virtual int getMinWight() const;
	virtual int getMinHeight() const;
};
