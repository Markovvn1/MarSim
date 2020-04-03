#include "panelMain.hpp"

#include "colors.hpp"

using namespace std;

PanelMain::PanelMain() : IPanel()
{
	core = NULL;
}

PanelMain::PanelMain(ICore* core) : IPanel(NULL)
{
	if (core) throw runtime_error("PanelMain::PanelMain(): core is NULL");
	if (!core->getPanelWorkspace()) throw runtime_error("PanelMain::PanelMain(): workspace is NULL");
	this->core = core;
	panelControl = PanelControl(this);
}

PanelMain::~PanelMain()
{

}

void PanelMain::eventRender(cairo_t* cairo)
{
	core->getPanelWorkspace()->render(cairo);
	if (!core->getPanelInfo()) core->getPanelInfo()->render(cairo);
}

void PanelMain::eventMouse(const EventMouse& event)
{
	if (core->getPanelWorkspace()->mouse(event)) return;
	if (!core->getPanelInfo())
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





int PanelMain::getMinWight() const
{
	return 10 + max(core->getPanelWorkspace()->getMinWight(), panelControl.getMinWight()) + \
			(core->getPanelInfo() ? (core->getPanelInfo()->getMinWight() + 10) : 0) + 10;
}

int PanelMain::getMinHeight() const
{
	return 10 + max(panelControl.getMinHeight() + 10 + core->getPanelWorkspace()->getMinHeight(),
			(core->getPanelInfo() ? core->getPanelInfo()->getMinHeight() : 0)) + 10;
}

void PanelMain::setFocus(IPanel* panel)
{
	this->focus = panel;
}
