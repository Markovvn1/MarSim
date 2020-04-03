#include "rescue_line.hpp"

RescueLine::RescueLine() : ICore()
{

}

RescueLine::RescueLine(IPanel* parent) : ICore()
{
	panelWorkspace = PanelWorkspace(parent, &params);
	ICore::init(&panelWorkspace, NULL);
}

void RescueLine::start()
{

}

void RescueLine::stop()
{

}

int RescueLine::onMessage(uint type, int len, const char* data, char*& answer)
{
	return 0;
}
