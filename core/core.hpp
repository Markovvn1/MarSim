#pragma once

#include "window/panel.hpp"

class ICore
{
private:
	IPanel* _workspace;
	IPanel* _info;

public:
	ICore();
	virtual ~ICore();

	void init(IPanel* workspace, IPanel* info);

	// Системные сообщения (общие для всех подпрограмм)
	virtual void start(); // Когда просят запустить программу
	virtual void stop(); // Когда просят остановить программу

	// Пользовательские команды (подпрограмма решает как их интерпретирвать)
	virtual int onMessage(uint type, int len, const char* data, char*& answer);

	IPanel* getPanelWorkspace();
	IPanel* getPanelInfo();
};
