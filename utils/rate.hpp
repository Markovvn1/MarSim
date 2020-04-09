/*
 *  Project: dynamixel_ros
 *  File: rate.cpp
 *
 *  Date: 20.02.2020
 *  Author: Markovvn1
 */

#pragma once

#include <stdint.h>

uint64_t getCTimeMicrosecond();

class Rate
{
private:
	bool first;
	uint64_t lastTime;
	uint64_t delta;

public:
	Rate();
	explicit Rate(double rate);
	bool sleep();
	uint64_t getDelta();
};
