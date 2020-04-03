#pragma once

#include "../../utils/utils.hpp"
#include "panel-interface.hpp"

#include "panel.hpp"

class PanelMain : public IPanel
{
private:
	IPanel* focus = NULL;

	PanelControl panelControl;
	PanelWorkspace panelWorkspace;

protected:
	virtual void eventRender(cairo_t* cairo);
	virtual void eventMouse(const EventMouse& event); // Абсолютные координаты
	virtual void eventKeyboard(const EventKeyboard& event);
	virtual void eventReshape(const Rect& newRect); // Абсолютные координаты

public:
	PanelMain();
	PanelMain(Params* params);
	virtual ~PanelMain();

	virtual int getMinWight() const;
	virtual int getMinHeight() const;

	virtual void setFocus(IPanel* panel); // По умолчанию - вызывать эту же функцию у родителя
};
