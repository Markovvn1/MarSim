#pragma once

#include <stdlib.h>
#include <stdint.h>

void gaussianBlur(uint8_t* data, int w, int h, uint stride, float r);
