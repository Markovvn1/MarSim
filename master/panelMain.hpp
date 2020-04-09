#pragma once

#include "core/core.hpp"
#include "utils/utils.hpp"
#include "window/panel.hpp"

#include "panelControl.hpp"

class PanelMain : public IPanel
{
private:
	ICore* core;
	PanelControl panelControl;
	IPanel* focus = NULL;

	// Double-buffering
	int doubleW, doubleH;
	cairo_surface_t* doubleSurface;
	cairo_t* doubleCairo;

protected:
	virtual void eventRender(cairo_t* cairo);
	virtual void eventMouse(const EventMouse& event); // Абсолютные координаты
	virtual void eventKeyboard(const EventKeyboard& event);
	virtual void eventReshape(const Rect& newRect); // Абсолютные координаты

public:
	PanelMain();
	virtual ~PanelMain();

	void setCore(ICore* core);

	virtual int getMinWight() const;
	virtual int getMinHeight() const;

	virtual void setFocus(IPanel* panel); // По умолчанию - вызывать эту же функцию у родителя
};
