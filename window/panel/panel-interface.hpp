#pragma once

#include <cairo/cairo.h>

#include "utils/utils.hpp"
#include "params.hpp"
#include "../event/event.hpp"

class IPanel
{
private:
	bool mouseActive; // mouse was in this panel
	bool needRender;

protected:
	IPanel* parent;

	Rect rect; // Абсолютные координаты
	Params* params;

	virtual void eventRender(cairo_t* cairo);
	virtual void eventMouse(const EventMouse& event); // Абсолютные координаты
	virtual void eventKeyboard(const EventKeyboard& event);
	virtual void eventReshape(const Rect& newRect); // Абсолютные координаты
	virtual void eventOnClose();

public:
	IPanel();
	IPanel(Params* params, IPanel* parent);
	virtual ~IPanel();

	virtual int getMinWight() const;
	virtual int getMinHeight() const;
	const Rect& getRect() const; // Абсолютные координаты
	IPanel* getParent() const;

	bool isNeedRender();
	void setRender(); // установить флаг, просящий перерисовать окно
	void render(cairo_t* cairo);

	bool reshape(const Rect& newRect);
	bool mouse(const EventMouse& event); // Абсолютные координаты
	bool keyboard(const EventKeyboard& event);
	virtual void setFocus(IPanel* panel); // По умолчанию - вызывать эту же функцию у родителя
};
