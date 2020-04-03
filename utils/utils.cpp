#include "utils.hpp"

Rect::Rect()
{

}

Rect::Rect(int x, int y, int width, int height): x(x), y(y), width(width), height(height)
{

}

bool Rect::operator ==(const Rect& r)
{
	return r.x == x && r.y == y && r.width == width && r.height == height;
}
