#include <X11/Xlib.h>
#include <iostream>

enum WM_KEYBOARD_TYPE
{
	K_KEYDOWN,
	K_KEYUP
};

enum WM_KEYBOARD_SET
{
	K_ENGLISH = 0x00,
	K_RUSSIA = 0x06,
	K_FUNCTIONS = 0xff
};

struct EventKeyboard
{
	WM_KEYBOARD_TYPE type; // up, down
	WM_KEYBOARD_SET set; // russia, english, function
	KeySym keysym; // number of symbol
	wchar_t sym; // symbol

	unsigned int time = 0;

	static bool fromXEvent(const XEvent& event, EventKeyboard& res);

	friend std::wostream& operator <<(std::wostream& out, const EventKeyboard& event);
};
