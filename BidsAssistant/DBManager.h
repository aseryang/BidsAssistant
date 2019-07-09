#pragma once
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include "GlobalDefine.h"
class DBManager:public QObject
{
	Q_OBJECT
private:
	DBManager() :model(nullptr), lastQueryTable(TBL_COMPANY_PERFORMANCE){}
public:
	static DBManager* instance()
	{
		static DBManager* handle = new DBManager;
		return handle;
	}
	~DBManager();
	int init(QString& ip);
	DBData* queryPerformance();
	QString generalQueryConditon();
	DBData* queryStatus();
	AddCompanyResult checkCompanyIsExist();
	AddCompanyResult addCompany();
	LastDBQueryTable getLastQueryTable();

protected:
	//QT远程连接mysql，没有任何操作，持续一段时间后，连接自动断开。通过执行一个简单sql的方式，进行重连
	void MysqlPing();
private:
	QSqlDatabase db;
	QSqlTableModel* model;
	DBData dataInfo;
	LastDBQueryTable lastQueryTable;
};