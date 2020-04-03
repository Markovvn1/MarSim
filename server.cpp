#include "server.hpp"
#include "CrossSocket/socket.hpp"

#include <unistd.h>
#include <pthread.h>

pthread_t server_thr_id;
bool server_thr_active = true;

void* server_thr(void* ptr)
{
	ICore& params = *((ICore*)ptr);

	while (server_thr_active)
	{
//		while (params.physics_active)
//		{
//			usleep(100);
//		}
	}

	return NULL;
}

void server_start(ICore* core)
{
	server_thr_active = true;
	pthread_create(&server_thr_id, NULL, server_thr, core);
}

void server_stop(ICore* core)
{
	server_thr_active = false;
	pthread_join(server_thr_id, NULL);
}
