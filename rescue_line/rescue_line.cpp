#include "rescue_line.hpp"

RescueLine::RescueLine() : ICore(&(IPanel)panelWorkspace, NULL)
{
1
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
