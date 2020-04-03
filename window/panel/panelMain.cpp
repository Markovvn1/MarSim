#include "panelMain.hpp"

#include "../colors.hpp"
#include <iostream>

using namespace std;

PanelMain::PanelMain() : IPanel()
{

}

PanelMain::PanelMain(Params* params) : IPanel(params, NULL)
{
	panelControl = PanelControl(params, this);
	panelWorkspace = PanelWorkspace(params, this);
}

PanelMain::~PanelMain()
{

}

void PanelMain::eventRender(cairo_t* cairo)
{
	panelControl.render(cairo);
	panelWorkspace.render(cairo);
}

void PanelMain::eventMouse(const EventMouse& event)
{
	if (panelWorkspace.mouse(event)) return;
	if (panelControl.mouse(event)) return;
//	wcout << event << endl;
}

void PanelMain::eventKeyboard(const EventKeyboard& event)
{
	if (focus != this && focus != NULL) focus->keyboard(event);

//	wcout << event << endl;
}

void PanelMain::eventReshape(const Rect& newRect)
{
	int cx = panelWorkspace.sizeX();
	int cy = panelWorkspace.sizeY();
	int sz = min((newRect.width - 10 - 10) / cx, (newRect.height - 10 - panelControl.getMinHeight() - 10 - 10) / cy);

	panelWorkspace.reshape({10, 10 + panelControl.getMinHeight() + 10, sz * cx, sz * cy});
	panelControl.reshape({10, 10, sz * cx, panelControl.getMinHeight()});
}





int PanelMain::getMinWight() const
{
	return 10 + max(panelWorkspace.getMinWight(), panelControl.getMinWight()) + 10;
}

int PanelMain::getMinHeight() const
{
	return 10 + panelControl.getMinHeight() + 10 + panelWorkspace.getMinHeight() + 10;
}

void PanelMain::setFocus(IPanel* panel)
{
	this->focus = panel;
}
