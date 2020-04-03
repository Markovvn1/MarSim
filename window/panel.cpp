#include "panel.hpp"

#include <stdexcept>
#include <cstdio>

#include "utils/utils.hpp"

using namespace std;

IPanel::IPanel()
{
	this->parent = NULL;
	this->mouseActive = false;
	this->needRender = false;
}

IPanel::IPanel(IPanel* parent)
{
	this->parent = parent;
	this->mouseActive = false;
	this->needRender = false;
}

IPanel::~IPanel() {}

void IPanel::eventRender(cairo_t* cairo) {}
void IPanel::eventMouse(const EventMouse& event) {}
void IPanel::eventKeyboard(const EventKeyboard& event) {}
void IPanel::eventReshape(const Rect& newRect) {}
void IPanel::eventOnClose() {}

const Rect& IPanel::getRect() const {return rect;}
IPanel* IPanel::getParent() const {return parent;}

bool IPanel::isNeedRender()
{
	return needRender;
}

void IPanel::setRender()
{
	needRender = true;
	if (parent != NULL) parent->setRender();
}

int IPanel::getMinWight() const { return -1; }
int IPanel::getMinHeight() const { return -1; }

void IPanel::render(cairo_t* cairo)
{
	needRender = false;
	eventRender(cairo);
}

bool IPanel::reshape(const Rect& newRect)
{
	if (newRect.height < 0 || newRect.width < 0) throw runtime_error("IPanel::reshape(): attempt to set negative size");
	if (newRect.height < getMinHeight() || newRect.width < getMinWight()) throw runtime_error("IPanel::reshape(): attempt to set too small size");
	if (rect == newRect) return false;

	eventReshape(newRect);
	rect = newRect;

	return true;
}

bool IPanel::mouse(const EventMouse& event)
{
	bool mouseActive = (event.getEvent() != M_LEAVE) && event.inRect(rect);

	if (this->mouseActive != mouseActive)
	{
		this->mouseActive = mouseActive;

		EventMouse newEvent;
		newEvent.type = mouseActive ? M_HOVER : M_LEAVE;
		newEvent.x = event.x;
		newEvent.y = event.y;
		newEvent.time = event.time;

		eventMouse(newEvent);
	}

	// Основное сообщение не для нас
	if (!mouseActive) return false;
	if (event.getEvent() == M_HOVER || event.getEvent() == M_LEAVE) return false;


	eventMouse(event);

	return true;
}

bool IPanel::keyboard(const EventKeyboard& event)
{
	eventKeyboard(event);
	return true;
}

void IPanel::setFocus(IPanel* panel)
{
	if (parent == NULL) return;
	parent->setFocus(panel);
}
