#pragma once
#include <QThread>
#include <QTableView>
#include <ActiveQt/QAxObject>
#include <QCoreApplication>
#include "GlobalDefine.h"
class DataExportHelper: public QThread
{
	Q_OBJECT
private:
	DataExportHelper():isFilePathSeted(false), table(NULL)
	{
		templateFilePath.append(QCoreApplication::applicationDirPath());
		templateFilePath += "/Template.xlsx";
	}
public:
	~DataExportHelper(){}
	static DataExportHelper* instance()
	{
		static DataExportHelper* handle = new DataExportHelper;
		return handle;
	}
	void setFilePath(QString& path);
	void reset();
	bool getFilePathSetStatus();
	void DoExport(QTableView* tableView);
	void run();
signals:
	void updateJobStatusSignal(int, QString);
protected:
private:
	QString savedFilePath;
	QString templateFilePath;
	bool isFilePathSeted;
	QTableView* table;
};