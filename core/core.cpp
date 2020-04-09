#include "core.hpp"

#include "server.hpp"

ICore::ICore()
{
	this->active = false;
	this->_workspace = NULL;
	this->_info = NULL;

	server_start(this);
}

ICore::~ICore()
{
	server_stop(this);
}

void ICore::init(IPanel* workspace, IPanel* info)
{
	this->_workspace = workspace;
	this->_info = info;
}

void ICore::addSysMsg(SysMsg* sysMsg)
{
	sysMsgs.push_back(sysMsg);
}

void onStart() {};
void onStop() {};

uint32_t ICore::onMessage(uint16_t type, uint32_t len, const char* data, char*& answer) { return 0; }
void ICore::onFree(uint32_t len, char* answer) {}


void ICore::start()
{
	if (isActive()) return;
	active = true;

	onStart();
	for (SysMsg*& sysMsg : sysMsgs) sysMsg->onStart();
}

void ICore::stop()
{
	if (!isActive()) return;
	active = false;

	onStop();
	for (SysMsg*& sysMsg : sysMsgs) sysMsg->onStop();
}

bool ICore::isActive()
{
	return active;
}

IPanel* ICore::getPanelWorkspace() { return _workspace; }
IPanel* ICore::getPanelInfo() { return _info; }
