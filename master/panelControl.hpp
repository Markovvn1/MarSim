#pragma once

#include "core/sysMsg.hpp"
#include "core/core.hpp"
#include "utils/utils.hpp"
#include "window/panel.hpp"

class PanelControl : public IPanel, public SysMsg
{
private:
	ICore* core;
	int sz; // Размер одной ячейки
	int state[1]; // 0 - ничего, 1 - мышка наведена, 2 - машка нажата
	int data[1]; // Данные с кнопок (какую картинку рисовать)

protected:
	virtual void onStart();
	virtual void onStop();

	virtual void eventRender(cairo_t* cairo);
	virtual void eventMouse(const EventMouse& event); // Абсолютные координаты
	virtual void eventKeyboard(const EventKeyboard& event);
	virtual void eventReshape(const Rect& newRect); // Абсолютные координаты

public:
	PanelControl();
	PanelControl(IPanel* parent);
	virtual ~PanelControl();

	void setCore(ICore* core);

	virtual int getMinWight() const;
	virtual int getMinHeight() const;
};
