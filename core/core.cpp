#include "core.hpp"

ICore::ICore()
{
	this->_workspace = NULL;
	this->_info = NULL;
}

ICore::ICore(IPanel* workspace, IPanel* info)
{
	this->_workspace = workspace;
	this->_info = info;
}

ICore::~ICore()
{

}

void ICore::start() {}
void ICore::stop() {}

int ICore::onMessage(uint type, int len, const char* data, char*& answer) { return 0; }

IPanel* ICore::getPanelWorkspace() { return _workspace; }
IPanel* ICore::getPanelInfo() { return _info; }