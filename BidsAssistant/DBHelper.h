#pragma once
#include <QThread>
#include "GlobalDefine.h"
class DBHelper: public QThread
{
	Q_OBJECT
private:
	DBHelper(){}
public:
	~DBHelper(){}
	static DBHelper* instance()
	{
		static DBHelper* handle = new DBHelper;
		return handle;
	}
	void AddCompany();
	void Query();
	void CheckStatus();
	void run();
signals:
	void jobFinishSignal();
	void updateUI(void*, eMsgID);
private:
	void doAdd();
	void doQuery();
	void doCheckStatus();
private:
	eAppStatus curStatus;
	AddCompanyResult addResult;
};