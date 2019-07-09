#include "DBHelper.h"
#include "DBManager.h"

void DBHelper::AddCompany()
{
	curStatus = STAT_ADD_COMPANY;
	start();
}
void DBHelper::Query()
{
	curStatus = STAT_QUERYING;
	start();
}
void DBHelper::CheckStatus()
{
	curStatus = STAT_CHECKING;
	start();
}

void DBHelper::run()
{
	switch (curStatus)
	{
	case STAT_ADD_COMPANY:
		doAdd();
		break;
	case STAT_QUERYING:
		doQuery();
		break;
	case STAT_CHECKING:
		doCheckStatus();
		break;
	default:
		break;
	}
	emit jobFinishSignal();
}
void DBHelper::doAdd()
{
	addResult = DBManager::instance()->checkCompanyIsExist();
	if (RET_NOT_EXIST == addResult)
	{
		addResult = DBManager::instance()->addCompany();
	}
	emit updateUI(&addResult, ID_ADD_COMPANY_RESULT);
}
void DBHelper::doQuery()
{
	DBData* data = DBManager::instance()->queryPerformance();
	emit updateUI(data, ID_UDPATE_QUERY_RESULT);
}
void DBHelper::doCheckStatus()
{
	DBData* data = DBManager::instance()->queryStatus();
	emit updateUI(data, ID_UDPATE_CHECK_RESULT);
}