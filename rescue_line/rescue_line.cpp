// 1. Добавить не идеальность в симуляцию

#include "rescue_line.hpp"

#include <cmath>

RescueLine::RescueLine() : ICore()
{

}

RescueLine::RescueLine(IPanel* parent) : ICore()
{
	params.robot.moveTo(CELL_SIZE * 1.5, CELL_SIZE * 1.5, M_PI);
	panelWorkspace = PanelWorkspace(this, parent);
	ICore::init(&panelWorkspace, NULL);
	addSysMsg(&panelWorkspace);

	physics_start(this);
}

RescueLine::~RescueLine()
{
	physics_stop(this);
}

void RescueLine::assertMsgLen(uint32_t len, uint32_t target)
{
	if (len == target) return;
	throw std::runtime_error("Invalid format of protocol (incorrect length of message)");
}

void RescueLine::onStart()
{
	printf("onStart\n");
}

void RescueLine::onStop()
{
	printf("onStop\n");
}

uint32_t RescueLine::onMessage(uint16_t type, uint32_t len, const char* data, char*& answer)
{
	answer = NULL;

	switch (type)
	{
	case 1024:
	{
		assertMsgLen(len, 4);
		int16_t* speeds = (int16_t*)data;
		printf("setSpeed(%d, %d)\n", speeds[0], speeds[1]);
		break;
	}
	default:
		printf("onMessage(%d, %d): unknown type\n", type, len);
		break;
	}

	return 0;
}

void RescueLine::onFree(uint32_t len, char* answer)
{
	if (answer) delete [] answer;
}

Params* RescueLine::getParams()
{
	return &params;
}
