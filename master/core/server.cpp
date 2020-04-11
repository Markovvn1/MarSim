#include "server.hpp"
#include "CrossSocket/socket.hpp"

#include <unistd.h>
#include <pthread.h>

pthread_t server_thr_id;
bool server_thr_active = false;

void* server_thr(void* ptr)
{
	ICore& core = *((ICore*)ptr);

	ServerSocket server;
	server.open();
	server.bind(12406);
	server.listen(1);


	while (server_thr_active)
	{
		AcceptedClient client = server.accept();

		if (!client.isActive())
		{
			usleep(10000);
			continue;
		}

		while (server_thr_active)
		{
			uint16_t type; client.recv((char*)&type, 2);
			uint32_t len; client.recv((char*)&len, 4);
			if (!client.isActive()) break;

			char* data = NULL;
			if (len > 0)
			{
				data = new char[len];
				if (!client.recv(data, len))
				{
					delete [] data;
					break;
				}
			}

			char* answer = NULL;

			if (type < 1024)
			{
				// Системные сообщения
				switch (type)
				{
				case 1: // start
					core.start();
					len = 0; answer = NULL;
					break;
				case 2: // stop
					core.stop();
					len = 0; answer = NULL;
					break;
				case 3: // isActive
					len = 1; answer = new char[1];
					answer[0] = core.isActive();
					break;
				}

				client.send((const char*)&len, 4);
				client.send(answer, len);

				if (answer) delete [] answer;
			}
			else
			{
				// Пользовательские сообщения
				len = core.onMessage(type, len, data, answer);

				client.send((const char*)&len, 4);
				client.send(answer, len);

				core.onFree(len, answer);
			}

			if (data) delete [] data;
		}

		client.close();
	}

	server.close();

	return NULL;
}

void server_start(ICore* core)
{
	if (server_thr_active) throw std::runtime_error("server_start(): server already started");
	server_thr_active = true;
	pthread_create(&server_thr_id, NULL, server_thr, core);
}

void server_stop(ICore* core)
{
	server_thr_active = false;
	pthread_join(server_thr_id, NULL);
}
