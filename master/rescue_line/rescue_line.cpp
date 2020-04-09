#include "rescue_line.hpp"

#include <cmath>

RescueLine::RescueLine() : ICore()
{

}

RescueLine::RescueLine(IPanel* parent) : ICore()
{
	params.robot = Robot(CELL_SIZE * 1.5, CELL_SIZE * 1.5, M_PI, &params);
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

}

void RescueLine::onStop()
{

}

uint32_t RescueLine::onMessage(uint16_t type, uint32_t len, const char* data, char*& answer)
{
	answer = NULL;

	switch (type)
	{
	case 1024: // moveTo
	{
		assertMsgLen(len, 12);
		float* d = (float*)data;
		params.robot.moveTo(d[0], d[1], d[2]);
		return 0;
	}
	case 1025: // setSpeed
	{
		assertMsgLen(len, 8);
		float* speeds = (float*)data;
		params.robot.setSpeed(speeds[0], speeds[1]);
		return 0;
	}
	default:
		wprintf(L"[WARN] onMessage(%d, %d): unknown type\n", type, len);
		return 0;
	}
}

void RescueLine::onFree(uint32_t len, char* answer)
{
	if (answer) delete [] answer;
}

Params* RescueLine::getParams()
{
	return &params;
}
