#pragma once

#include "core/sysMsg.hpp"
#include "utils/utils.hpp"
#include "window/panel.hpp"

#include "physics/robot.hpp"

class Cell;
class RescueLine;

class PanelWorkspace : public IPanel, protected SysMsg
{
	friend class RescueLine;

private:
	RescueLine* core;
	Cell* cells;
	uint sz; // Размер одной ячейки
	Rect field;

	// Взаимодействие с роботом
	bool pickRobot; // Подобрал робота (держит его мышкой)
	bool pickControlCircle; // Подобрал кольцо вокруг робота
	bool activeRobot; // Робот недавно был затронут (показывается кольцо вокруг робота)
	double pickDx, pickDy, pickDA;
	double rotRmin, rotRmax; // Радиусы кольца вокруг робота

protected:
	virtual void onStart();
	virtual void onStop();

	virtual void eventRender(cairo_t* cairo);
	virtual void eventMouse(const EventMouse& event); // Абсолютные координаты
	virtual void eventKeyboard(const EventKeyboard& event);
	virtual void eventReshape(const Rect& newRect); // Абсолютные координаты

public:
	PanelWorkspace();
	PanelWorkspace(RescueLine* core, IPanel* parent);
	virtual ~PanelWorkspace();

	void updateParams();

	virtual int getMinWight() const;
	virtual int getMinHeight() const;
};
