#pragma once

#include <cmath>

#ifndef NULL
	#define NULL 0
#endif

struct Rect
{
	int x = 0, y = 0;
	int width = 0, height = 0;

	Rect();
	Rect(int x, int y, int width, int height);

	bool operator ==(const Rect& r);
};
