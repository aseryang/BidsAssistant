#include "LoginHelper.h"
#include "DBManager.h"

void LoginHelper::DoLogin(QString ipstr)
{
	ip = ipstr;
	start();
}
void LoginHelper::run()
{
	if (DBManager::instance()->init(ip))
	{
		emit loginResultSignal(LOGIN_FAILED);
	}
	else
	{
		emit loginResultSignal(LOGIN_SUCCEED);
	}
}