#pragma once

#include "utils/utils.hpp"
#include "window/panel.hpp"

class PanelControl : public IPanel
{
private:
	int sz; // Размер одной ячейки
	int state[1]; // 0 - ничего, 1 - мышка наведена, 2 - машка нажата
	int data[1]; // Данные с кнопок (какую картинку рисовать)

protected:
	virtual void eventRender(cairo_t* cairo);
	virtual void eventMouse(const EventMouse& event); // Абсолютные координаты
	virtual void eventKeyboard(const EventKeyboard& event);
	virtual void eventReshape(const Rect& newRect); // Абсолютные координаты

public:
	PanelControl();
	PanelControl(IPanel* parent);
	virtual ~PanelControl();

	virtual int getMinWight() const;
	virtual int getMinHeight() const;
};
