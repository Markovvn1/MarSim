#include "eventKeyboard.hpp"

#include <X11/XKBlib.h>
#include <iomanip>

using namespace std;

const wchar_t* EventKeyboard_symToCyrillic = L"юабцдефгхийклмнопярстужвьызшэщчъЮАБЦДЕФГХИЙКЛМНОПЯРСТУЖВЬЫЗШЭЩЧЪ";

bool EventKeyboard::fromXEvent(const XEvent& event, EventKeyboard& res)
{
	if (event.type != KeyPress && event.type != KeyRelease) return false;

	const XKeyEvent& e = event.xkey;

	res.type = (event.type == KeyPress) ? K_KEYDOWN : K_KEYUP;
	res.time = e.time;

	XkbLookupKeySym(event.xkey.display, e.keycode, e.state, NULL, &res.keysym);

	res.set = (WM_KEYBOARD_SET)(res.keysym >> 8);
	res.sym = res.keysym & 0xff;

	// Only Latin 1, Cyrillic or symbols
	if (res.set != K_ENGLISH && res.set != K_RUSSIA && res.set != K_FUNCTIONS) return false;


	if (res.set == K_RUSSIA)
	{
		if (res.sym >= 0xc0)
		{
			res.sym = EventKeyboard_symToCyrillic[res.sym - 0xc0];
		}
		else
		{
			if (res.sym == 179) res.sym = L'Ё';
			else if (res.sym == 163) res.sym = L'ё';
			else return false;
		}
	}

	return true;
}

wostream& operator <<(wostream& out, const EventKeyboard& event)
{
	// <Event  src=Keyboard  type=Button(Down)  keysym=0x0034  set=English  char='f'>
	out << "<Event  src=Keyboard  type=Button(";
	out << (event.type == K_KEYDOWN ? "Down" : "Up  ");

	out << ")  keysym=0x" << hex;
	wchar_t fill_default = out.fill('0');
	streamsize width_default = out.width(4);
	out << event.keysym << dec;

	out << "  set=";
	if (event.set == K_ENGLISH) out << "English";
	else if (event.set == K_RUSSIA) out << "Russia";
	else out << "Functions";

	if (event.set != K_FUNCTIONS)
		out << "  char=\'" << event.sym << '\'';

	out << ">";

	printf("!!!%lc!!!", event.sym);

	out.fill(fill_default);
	out.width(width_default);

	return out;
}
