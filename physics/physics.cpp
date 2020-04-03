#include "physics.hpp"

#include <unistd.h>
#include <pthread.h>

pthread_t physics_thr_id;
bool physics_thr_active = true;

void* physics_thr(void* ptr)
{
	Params& params = *((Params*)ptr);

	while (physics_thr_active)
	{
		while (params.physics_active)
		{
			usleep(100);
		}
	}

	return NULL;
}

void physics_start(Params* params)
{
	physics_thr_active = true;
	pthread_create(&physics_thr_id, NULL, physics_thr, params);
}

void physics_stop(Params* params)
{
	physics_thr_active = false;
	params->physics_active = false;
	pthread_join(physics_thr_id, NULL);
}
