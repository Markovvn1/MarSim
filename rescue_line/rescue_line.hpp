#pragma once

#include "core/core.hpp"
#include "panelControl.hpp"
#include "panelWorkspace.hpp"
#include "params.hpp"
#include "physics/physics.hpp"

class RescueLine : public ICore
{
private:
	Params params;
	PanelWorkspace panelWorkspace;

	void assertMsgLen(uint32_t len, uint32_t target);

protected:
	void onStart();
	void onStop();

	virtual uint32_t onMessage(uint16_t type, uint32_t len, const char* data, char*& answer);
	virtual void onFree(uint32_t len, char* answer);

public:
	RescueLine();
	RescueLine(IPanel* parent);

	virtual ~RescueLine();

	Params* getParams();
};
