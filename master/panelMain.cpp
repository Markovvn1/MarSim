#include "panelMain.hpp"

#include "colors.hpp"

using namespace std;

PanelMain::PanelMain() : IPanel(NULL)
{
	core = NULL;
	panelControl = PanelControl(this);

	doubleSurface = NULL;
	doubleCairo = NULL;
	doubleW = doubleH = 0;
}

PanelMain::~PanelMain()
{
	if (doubleCairo) cairo_destroy(doubleCairo);
	if (doubleSurface) cairo_surface_destroy(doubleSurface);
}

void PanelMain::eventRender(cairo_t* cairo)
{
	if (!core) throw runtime_error("PanelMain::eventRender(): core is NULL");

	panelControl.render(doubleCairo);
	core->getPanelWorkspace()->render(doubleCairo);
	if (core->getPanelInfo()) core->getPanelInfo()->render(doubleCairo);

	cairo_set_source_surface(cairo, doubleSurface, 0, 0);
	cairo_paint(cairo);
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

	// Не нужно при каждом чихе создавать новый холст
	if (newRect.width > doubleW || newRect.width * 2 < doubleW ||
			newRect.height > doubleH || newRect.height * 2 < doubleH)
	{
		if (doubleCairo) cairo_destroy(doubleCairo);
		if (doubleSurface) cairo_surface_destroy(doubleSurface);

		doubleW = newRect.width + newRect.width / 2;
		doubleH = newRect.height + newRect.height / 2;
		doubleSurface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, doubleW, doubleH);
		doubleCairo = cairo_create(doubleSurface);
	}

	cairo_set_source_rgb(doubleCairo, COLOR_PANEL_MAIN);
	cairo_paint(doubleCairo);
}



void PanelMain::setCore(ICore* core)
{
	if (!core) throw runtime_error("PanelMain::setCore(): core is NULL");
	if (!core->getPanelWorkspace()) throw runtime_error("PanelMain::setCore(): workspace is NULL");
	panelControl.setCore(core);
	core->addSysMsg(&panelControl);
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
