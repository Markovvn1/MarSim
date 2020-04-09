#pragma once

#include <vector>

#include "sysMsg.hpp"
#include "window/panel.hpp"

class ICore : protected SysMsg
{
	friend void* server_thr(void* ptr);

private:
	bool active; // была команда старт
	IPanel* _workspace;
	IPanel* _info;
	std::vector<SysMsg*> sysMsgs;

protected:
	// Пользовательские команды (подпрограмма решает как их интерпретирвать)
	// удаление данных по адресу answer остается на ребенком
	virtual uint32_t onMessage(uint16_t type, uint32_t len, const char* data, char*& answer);
	// Вызывается после того как answer уже отправлен и данные можно освобождать
	virtual void onFree(uint32_t len, char* answer);

public:
	ICore();
	virtual ~ICore();

	void init(IPanel* workspace, IPanel* info);
	void addSysMsg(SysMsg* sysMsg); // Подписаться на получение системных сообщений

	// Системные сообщения (общие для всех подпрограмм)
	void start(); // Когда просят запустить программу
	void stop(); // Когда просят остановить программу
	bool isActive(); // бфла команда старт

	IPanel* getPanelWorkspace();
	IPanel* getPanelInfo();
};
