#include "panelMain.hpp"

#include "colors.hpp"

using namespace std;

PanelMain::PanelMain() : IPanel(NULL)
{
	core = NULL;
	panelControl = PanelControl(this);
}

PanelMain::~PanelMain()
{

}

void PanelMain::eventRender(cairo_t* cairo)
{
	if (!core) throw runtime_error("PanelMain::eventRender(): core is NULL");
	panelControl.render(cairo);
	core->getPanelWorkspace()->render(cairo);
	if (core->getPanelInfo()) core->getPanelInfo()->render(cairo);
}

void PanelMain::eventMouse(const EventMouse& event)
{
	if (!core) throw runtime_error("PanelMain::eventMouse(): core is NULL");
	if (panelControl.mouse(event)) return;
	if (core->getPanelWorkspace()->mouse(event)) return;
	if (core->getPanelInfo())
		if (core->getPanelInfo()->mouse(event)) return;

//	wcout << event << endl;
}

void PanelMain::eventKeyboard(const EventKeyboard& event)
{
	if (focus != this && focus != NULL) focus->keyboard(event);

//	wcout << event << endl;
}

void PanelMain::eventReshape(const Rect& newRect)
{
	if (!core) throw runtime_error("PanelMain::eventReshape(): core is NULL");
	if (core->getPanelInfo())
	{
		throw runtime_error("PanelMain::eventReshape(): not realized yet");
	}
	else
	{
		int sy = 10 + panelControl.getMinHeight() + 10;
		core->getPanelWorkspace()->reshape({10, sy, newRect.width - 10 - 10, newRect.height - sy - 10});
		panelControl.reshape({10, 10, newRect.width - 10 - 10, panelControl.getMinHeight()});
	}
}



void PanelMain::setCore(ICore* core)
{
	if (!core) throw runtime_error("PanelMain::setCore(): core is NULL");
	if (!core->getPanelWorkspace()) throw runtime_error("PanelMain::setCore(): workspace is NULL");
	this->core = core;
}

int PanelMain::getMinWight() const
{
	if (!core) throw runtime_error("PanelMain::getMinWight(): core is NULL");
	return 10 + max(core->getPanelWorkspace()->getMinWight(), panelControl.getMinWight()) + \
			(core->getPanelInfo() ? (core->getPanelInfo()->getMinWight() + 10) : 0) + 10;
}

int PanelMain::getMinHeight() const
{
	if (!core) throw runtime_error("PanelMain::getMinHeight(): core is NULL");
	return 10 + max(panelControl.getMinHeight() + 10 + core->getPanelWorkspace()->getMinHeight(),
			(core->getPanelInfo() ? core->getPanelInfo()->getMinHeight() : 0)) + 10;
}

void PanelMain::setFocus(IPanel* panel)
{
	this->focus = panel;
}
