#pragma once
#include <QSettings>
#include "GlobalDefine.h"

class ConfigManager
{
private:
	ConfigManager();
public:
	static ConfigManager* instance()
	{
		static ConfigManager* handle = new ConfigManager;
		return handle;
	}
	~ConfigManager();
	int init();
	void setIp(QString ipstr);
	QString getIP();
	UserConfig* GetUserCfg();

protected:
private:
	QSettings *userConfig;
	UserConfig cfg;
	bool isInit;
};