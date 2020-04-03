#pragma once

#include "core/core.hpp"
#include "panelControl.hpp"
#include "panelWorkspace.hpp"

class RescueLine : public ICore
{
private:
	PanelWorkspace panelWorkspace;

public:
	RescueLine();

	// Системные сообщения (общие для всех подпрограмм)
	void start(); // Когда просят запустить программу
	void stop(); // Когда просят остановить программу

	// Пользовательские команды (подпрограмма решает как их интерпретирвать)
	int onMessage(uint type, int len, const char* data, char*& answer);
};
