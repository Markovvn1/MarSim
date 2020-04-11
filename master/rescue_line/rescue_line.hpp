#pragma once

// 1. Добавить не идеальность в симуляцию
// 2. Возможность менять размеры роботов и все что с этим связано
// 3. Возможность создавать больше однго робота
// 4. Добавить возможность получать координаты робота
// 5. Добавить панельку, на которой будет отображаться текущий цвет под датчиками

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
