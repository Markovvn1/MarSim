#include "eventMouse.hpp"

#include "../../utils/utils.hpp"

#define XBUTTON_LEFT Button1
#define XBUTTON_MIDDLE Button2
#define XBUTTON_RIGHT Button3
#define XBUTTON_WHEEL_UP Button4
#define XBUTTON_WHEEL_DOWN Button5

using namespace std;

bool EventMouse::fromXEvent(const XEvent& event, EventMouse& res, EventMouseMemory& memory)
{
	switch (event.type)
	{
	case ButtonPress:
	case ButtonRelease:
	{
		const XButtonEvent& e = event.xbutton;

		res.x = e.x;
		res.y = e.y;
		res.time = e.time;

		if (e.button == XBUTTON_WHEEL_UP || e.button == XBUTTON_WHEEL_DOWN)
		{
			// Бесполезное событие у колесика мышки
			if (event.type == ButtonRelease) return false;

			res.type = M_WHEEL;
			res.wheel = (e.button == XBUTTON_WHEEL_UP ? 1 : -1);
		}
		else
		{
			res.type = M_BUTTON;
			if (e.button == XBUTTON_LEFT) res.type |= M_BUTTON_LEFT;
			if (e.button == XBUTTON_MIDDLE) res.type |= M_BUTTON_MIDDLE;
			if (e.button == XBUTTON_RIGHT) res.type |= M_BUTTON_RIGHT;

			res.type |= (event.type == ButtonPress ? M_BUTTON_EVENT_DOWN : M_BUTTON_EVENT_UP);

			if (event.type == ButtonPress)
			{
				if (memory.type == res.type && memory.x == res.x && memory.y == res.y && (res.time - memory.time <= DOUBLE_CLICK_DELAY))
				{
					memory.doubleActive = true;
					memory.time = res.time;
				}
				else
				{
					memory.type = res.type;
					memory.time = res.time;
					memory.x = res.x;
					memory.y = res.y;
				}

			}
		}

		break;
	}
	case EnterNotify:
	case LeaveNotify:
	{
		const XCrossingEvent& e = event.xcrossing;
//		if (e.mode != NotifyNormal) return false;
//		if (e.detail != NotifyNonlinear) return false;

		res.x = e.x;
		res.y = e.y;
		res.time = e.time;

		res.type = (event.type == EnterNotify ? M_HOVER : M_LEAVE);

		break;
	}
	case MotionNotify:
	{
		const XMotionEvent& e = event.xmotion;
		res.type = M_MOVE;
		res.time = e.time;
		res.x = e.x;
		res.y = e.y;

		break;
	}
	default:
		return false;
//		throw runtime_error("EventMouse::fromXEvent(): type of event is unknown");
	}

	return true;
}

bool EventMouse::fromMemory(EventMouse& res, EventMouseMemory& memory)
{
	if (memory.doubleActive)
	{
		memory.doubleActive = false;

		res.type = memory.type | M_BUTTON_EVENT_DBCLK;
		res.time = memory.time;
		res.x = memory.x;
		res.y = memory.y;

		memory.type = M_NONE;

		return true;
	}

	return false;
}



WM_MOUSE_EVENT EventMouse::getEvent() const
{
	return (WM_MOUSE_EVENT)(type & M_EVENT_MASK);
}

WM_MOUSE_BUTTON EventMouse::getButton() const
{
	return (WM_MOUSE_BUTTON)(type);
}

WM_MOUSE_BUTTON_TYPE EventMouse::getButtonType() const
{
	return (WM_MOUSE_BUTTON_TYPE)(type & M_BUTTON_TYPE_MASK);
}

WM_MOUSE_BUTTON_EVENT EventMouse::getButtonEvent() const
{
	return (WM_MOUSE_BUTTON_EVENT)(type & M_BUTTON_EVENT_MASK);
}

bool EventMouse::inRect(const Rect& rect) const
{
	return (x >= rect.x) && (x < rect.x + rect.width ) &&
		   (y >= rect.y) && (y < rect.y + rect.height);
}


wostream& operator <<(wostream& out, const EventMouse& event)
{
	// <Event  src=Mouse  type=Button(Left, Down)  pos=( 234,  875)>
	out << "<Event  src=Mouse  type=";


	switch (event.getEvent())
	{
	case M_BUTTON:
		out << "Button(";
		if (event.getButtonType() == M_BUTTON_LEFT) out << "Left";
		if (event.getButtonType() == M_BUTTON_MIDDLE) out << "Middle";
		if (event.getButtonType() == M_BUTTON_RIGHT) out << "Right";
		out << ", ";
		if (event.getButtonEvent() == M_BUTTON_EVENT_UP) out << "Up";
		if (event.getButtonEvent() == M_BUTTON_EVENT_DOWN) out << "Down";
		if (event.getButtonEvent() == M_BUTTON_EVENT_DBCLK) out << "DoubleClick";
		out << ")";
		break;

	case M_MOVE:
		out << "Move";
		break;

	case M_WHEEL:
		out << "Wheel(";
		out.width(2);
		out << event.wheel << ")";
		break;

	case M_HOVER:
		out << "Hover";
		break;

	case M_LEAVE:
		out << "Leave";
		break;

	default:
		out << "Undefined >";
		return out;
	}

	out << "  pos=(";

	out.width(4);
	out << event.x << ", " << event.y;
	out << ") >";


	return out;
}
