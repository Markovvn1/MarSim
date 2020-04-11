// 1. Добавить проверку версий

#include <X11/Xutil.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include <X11/keysym.h>

#include <cairo/cairo.h>
#include <cairo/cairo-xlib.h>

#include <cstdlib>
#include <unistd.h>
#include <stdexcept>
#include <iostream>

#include "utils/utils.hpp"
#include "window/event/event.hpp"
#include "window/panel.hpp"

#include "panelMain.hpp"
#include "rescue_line/rescue_line.hpp"

using namespace std;

struct WindowData
{
	Display* display = NULL;
	int screen = 0;
	Visual* visual = NULL;
	Window window = 0;

	cairo_surface_t* surface = NULL;
	cairo_t* cairo = NULL;

	Atom deleteWindowEvent;
};



void startXlib(WindowData& data, const IPanel* panel)
{
	data.display = XOpenDisplay(getenv("DISPLAY"));

	if (data.display == NULL)
	{
	   throw runtime_error(string("MartWindowBase::MartWindowBase(): Can't connect X server (display ") + string(getenv("DISPLAY")) + string(")"));
	   exit(EXIT_FAILURE);
	}

	data.screen = DefaultScreen(data.display);
	data.visual = DefaultVisual(data.display, data.screen);


	Rect rect;
	rect.width = max(panel->getMinWight(), DisplayWidth(data.display, data.screen) * 2 / 3);
	rect.height = max(panel->getMinHeight(), DisplayHeight(data.display, data.screen) * 2 / 3);
	if (rect.width < 0 || rect.height < 0) throw runtime_error("startXlib(): width < 0 || height < 0");
	rect.x = (DisplayWidth(data.display, data.screen) - rect.width) / 2;
	rect.y = (DisplayHeight(data.display, data.screen) - rect.height) / 2;
	data.window = XCreateSimpleWindow(data.display, RootWindow(data.display, data.screen),
									 rect.x, rect.y, rect.width, rect.height,
									 0, BlackPixel(data.display, data.screen), 0x474747);
	XStoreName(data.display, data.window, "MarSim | by Markovvn1");

	// Для корректного завершения
	data.deleteWindowEvent = XInternAtom(data.display, "WM_DELETE_WINDOW", 0);
	XSetWMProtocols(data.display, data.window, &data.deleteWindowEvent, 1);

	// Установка минимульного размера окна
	XSizeHints sizeHints;
	sizeHints.flags = PMinSize;
	sizeHints.min_width = rect.width;
	sizeHints.min_height = rect.height;
	XSetWMNormalHints(data.display, data.window, &sizeHints);

	XSelectInput(data.display, data.window,
			ExposureMask |
			KeyPressMask | KeyReleaseMask |
			ButtonPressMask | ButtonReleaseMask |
			PointerMotionMask |
			EnterWindowMask | LeaveWindowMask |
			StructureNotifyMask);
	XMapWindow(data.display, data.window);

	data.surface = cairo_xlib_surface_create(data.display, data.window, data.visual, rect.width, rect.height);
	data.cairo = cairo_create(data.surface);
}

void stopXlib(WindowData& data, IPanel& panelMain)
{
	cairo_destroy(data.cairo);
	cairo_surface_destroy(data.surface);

	XCloseDisplay(data.display);
}

int main()
{
	setlocale(LC_CTYPE, "");

	WindowData data;

	PanelMain panelMain;
	RescueLine rescueLine(&panelMain);
	panelMain.setCore(&rescueLine);

	startXlib(data, &panelMain);
	panelMain.reshape(Rect(0, 0, panelMain.getMinWight(), panelMain.getMinHeight()));

	XEvent event;
	EventMouse eventMouse;
	EventMouseMemory eventMouseMemory;
	EventKeyboard eventKeyboard;

	bool drowing = false;

	bool alive = true;
	while (alive)
	{
		if (panelMain.isNeedRender() && !drowing)
		{
			drowing = true;
			event.type = Expose;
			XSendEvent(data.display, data.window, 0, 0, &event);
		}

		// Если нет никаких событий
		if (XPending(data.display) <= 0)
		{
			usleep(1000);
			continue;
		}

		XNextEvent(data.display, &event);

		switch (event.type)
		{
		case Expose:
		{
			drowing = false;
			panelMain.render(data.cairo);
			cairo_surface_flush(data.surface);

			break;
		}
		case EnterNotify:
		case LeaveNotify:
		case ButtonPress:
		case ButtonRelease:
		case MotionNotify:
		{
			if (!EventMouse::fromXEvent(event, eventMouse, eventMouseMemory)) break;

			do
				panelMain.mouse(eventMouse);
			while (EventMouse::fromMemory(eventMouse, eventMouseMemory));

			break;
		}
		case KeyPress:
		case KeyRelease:
		{
			if (!EventKeyboard::fromXEvent(event, eventKeyboard)) break;

			panelMain.keyboard(eventKeyboard);

			break;
		}
		case ConfigureNotify:
		{
			XConfigureEvent& e = event.xconfigure;
			if (e.send_event) break; // Игнорируем сообщения о перемещении окна (ибо зачем нам это знать?)

			cairo_xlib_surface_set_size(data.surface, e.width, e.height);
			panelMain.reshape(Rect(0, 0, e.width, e.height));

			break;
		}
		case ClientMessage:
		{
			if (*(Atom*)event.xclient.data.l == data.deleteWindowEvent) alive = false;
			break;
		}
		default:
//			wprintf(L"Strange event: %d\n", event.type);
			break;
		}
	}

	rescueLine.stop();

	stopXlib(data, panelMain);

	return 0;
}
