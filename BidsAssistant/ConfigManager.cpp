#include "ConfigManager.h"
#include "GlobalDefine.h"
ConfigManager::ConfigManager() :isInit(false)
{
}
int ConfigManager::init()
{
	if (!isInit)
	{
		userConfig = new QSettings(USER_CONFIG, QSettings::IniFormat);
		userConfig->setIniCodec("UTF8");
		isInit = true;
	}
	return 0;
}
void ConfigManager::setIp(QString ipstr)
{

	userConfig->setValue("/mysql_ip", ipstr);
	userConfig->sync();
}
QString ConfigManager::getIP()
{
	return userConfig->value("/mysql_ip").toString();
}
UserConfig* ConfigManager::GetUserCfg()
{
	return &cfg;
}