#include "../../utils/utils.hpp"

#include <X11/Xlib.h>
#include <iostream>

#define DOUBLE_CLICK_DELAY 400

enum WM_MOUSE_EVENT
{
	M_NONE = 0,

	M_BUTTON = 1,
	M_MOVE = 2,
	M_WHEEL = 3,

	// Тут позиция тоже есть, но зачастую она выходит за рамки области
	M_HOVER = 4,
	M_LEAVE = 5
};

enum WM_MOUSE_MASK
{
	M_EVENT_MASK			= 0b11111111,
	M_BUTTON_TYPE_MASK		= (0b0011 << 8), // none, left, middle, right
	M_BUTTON_EVENT_MASK		= (0b1100 << 8), // none, down, up, dbclk

};

enum WM_MOUSE_BUTTON_TYPE
{
	M_BUTTON_LEFT		= (0b0001 << 8),
	M_BUTTON_MIDDLE		= (0b0010 << 8),
	M_BUTTON_RIGHT		= (0b0011 << 8),
};

enum WM_MOUSE_BUTTON_EVENT
{
	M_BUTTON_EVENT_DOWN		= (0b0100 << 8),
	M_BUTTON_EVENT_UP		= (0b1000 << 8),
	M_BUTTON_EVENT_DBCLK	= (0b1100 << 8),
};

enum WM_MOUSE_BUTTON
{
	M_BUTTON_L_DOWN		= M_BUTTON_EVENT_DOWN  | M_BUTTON_LEFT   | M_BUTTON,
	M_BUTTON_L_UP		= M_BUTTON_EVENT_UP    | M_BUTTON_LEFT   | M_BUTTON,
	M_BUTTON_L_DBCLK	= M_BUTTON_EVENT_DBCLK | M_BUTTON_LEFT   | M_BUTTON,

	M_BUTTON_M_DOWN		= M_BUTTON_EVENT_DOWN  | M_BUTTON_MIDDLE | M_BUTTON,
	M_BUTTON_M_UP		= M_BUTTON_EVENT_UP    | M_BUTTON_MIDDLE | M_BUTTON,
	M_BUTTON_M_DBCLK	= M_BUTTON_EVENT_DBCLK | M_BUTTON_MIDDLE | M_BUTTON,

	M_BUTTON_R_DOWN		= M_BUTTON_EVENT_DOWN  | M_BUTTON_RIGHT  | M_BUTTON,
	M_BUTTON_R_UP		= M_BUTTON_EVENT_UP    | M_BUTTON_RIGHT  | M_BUTTON,
	M_BUTTON_R_DBCLK	= M_BUTTON_EVENT_DBCLK | M_BUTTON_RIGHT  | M_BUTTON
};

struct EventMouseMemory
{
	bool doubleActive = false;

	// Last event
	int type = M_NONE;

	unsigned int time = 0;
	int x = -1, y = -1;
};

struct EventMouse
{
	int type;

	int x = 0, y = 0;
	int wheel = 0;
	unsigned int time;

	static bool fromXEvent(const XEvent& event, EventMouse& res, EventMouseMemory& memory);
	static bool fromMemory(EventMouse& res, EventMouseMemory& memory); // Такие события, как двойное нажание

	WM_MOUSE_EVENT getEvent() const; // button, move, wheel, hover, leave
	WM_MOUSE_BUTTON getButton() const;
	WM_MOUSE_BUTTON_TYPE getButtonType() const; // right, middle, left
	WM_MOUSE_BUTTON_EVENT getButtonEvent() const; // down, up, dbclk
	bool inRect(const Rect& rect) const;

	friend std::wostream& operator <<(std::wostream& out, const EventMouse& event);
};
