/*
 *  Project: dynamixel_ros
 *  File: rate.cpp
 *
 *  Date: 20.02.2020
 *  Author: Markovvn1
 */

#include "rate.hpp"

#include <unistd.h>
#include <ctime>

uint64_t getCTimeMicrosecond()
{
	timespec time;
	clock_gettime(CLOCK_REALTIME, &time);
	return (uint64_t)(time.tv_sec) * 1000000 + time.tv_nsec / 1000;
}

Rate::Rate() : Rate(1)
{

}

Rate::Rate(double rate)
{
	first = true;
	lastTime = getCTimeMicrosecond();
	delta = 1000000 / rate;
}

bool Rate::sleep()
{
	uint64_t currentTime = getCTimeMicrosecond();

	if (first)
	{
		first = false;
		lastTime = currentTime - delta + 1;
	}

	int count = (currentTime - lastTime) / delta;
	if (count > 0)
	{
		lastTime += count * delta;
		return false;
	}
	else
	{
		lastTime += delta;
		usleep(lastTime - currentTime);
		return true;
	}
}

uint64_t Rate::getDelta()
{
	return delta;
}
