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
	//QTԶ������mysql��û���κβ���������һ��ʱ��������Զ��Ͽ���ͨ��ִ��һ����sql�ķ�ʽ����������
	void MysqlPing();
private:
	QSqlDatabase db;
	QSqlTableModel* model;
	DBData dataInfo;
	LastDBQueryTable lastQueryTable;
};