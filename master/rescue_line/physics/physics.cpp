#include "physics.hpp"

#include <unistd.h>
#include <pthread.h>
#include <stdexcept>
#include <iostream>

#include "utils/utils.hpp"
#include "../rescue_line.hpp"


pthread_t physics_thr_id;
bool physics_thr_active = false;

void* physics_thr(void* ptr)
{
	RescueLine& core = *((RescueLine*)ptr);

	while (physics_thr_active)
	{
		if (!core.isActive()) usleep(10000);

		Rate rate(100);
		double t = 1. / 100;
		while (core.isActive())
		{
			core.getParams()->robot.update(t);
			if (!rate.sleep()) wprintf(L"[WARN] Physics: processing too slow\n");
		}
	}

	return NULL;
}

void physics_start(RescueLine* core)
{
	if (physics_thr_active) throw std::runtime_error("server_start(): server already started");
	physics_thr_active = true;
	pthread_create(&physics_thr_id, NULL, physics_thr, core);
}

void physics_stop(RescueLine* core)
{
	physics_thr_active = false;
	pthread_join(physics_thr_id, NULL);
}
