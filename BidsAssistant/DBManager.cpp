#include "DBManager.h"
#include "GlobalDefine.h"
#include <QDebug>
#include "ConfigManager.h"
#include <QSQLError>
DBManager::~DBManager()
{
}
int DBManager::init(QString& ip)
{
	db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName(ip);
	db.setDatabaseName(DB_NAME);
	db.setPort(DB_PORT);
	db.setUserName(USER_NAME);
	db.setPassword(USER_PASSWORD);
	//设置mysql连接断开时，自动重连
	db.setConnectOptions("MYSQL_OPT_RECONNECT=1;\
		CLIENT_SSL=1;\
		CLIENT_IGNORE_SPACE=1;\
		SSL_KEY=./client-key.pem;\
		SSL_CERT=./client-cert.pem;\
		SSL_CA=./ca-cert.pem");
	if (!db.open())
	{
		qDebug() << "连接数据库失败！";
		return 1;
	}
	db.exec("set NAMES UTF8");
		
	model = new QSqlTableModel(this, db);
	return 0;
}
void DBManager::MysqlPing()
{
	if (db.isOpen())
	{
		qDebug() << "数据库open！";
	}
	if (db.isValid())
	{
		qDebug() << "数据库valid！";
	}
	if (db.isOpen())
	{
		QString testOnBorrowSql("SELECT 1+1 ");
		QSqlQuery query(testOnBorrowSql, db);
		if (query.lastError().type() != QSqlError::NoError && !db.open())
		{
			qDebug() << "Open datatabase error:" << db.lastError().text();
		}
		else
		{
			qDebug() << "数据库连接测试成功！";
		}
	}
}
QString DBManager::generalQueryConditon()
{
	QString filter;
	QStringList conditonList;
	UserConfig* cfg = ConfigManager::instance()->GetUserCfg();
	if (!cfg->companyname.isEmpty())
	{
		QString companyname;
		companyname.sprintf("companyname LIKE '%%%s%%'", cfg->companyname.toUtf8().data());
		conditonList.push_back(companyname);
	}
	if (!cfg->scoreBegin.isEmpty())
	{
		QString scoreBeing;
		scoreBeing.sprintf("Score >= '%s'", cfg->scoreBegin.toUtf8().data());
		conditonList.push_back(scoreBeing);
	}
	if (!cfg->scoreEnd.isEmpty())
	{
		QString scoreEnd;
		scoreEnd.sprintf("Score <= '%s'", cfg->scoreEnd.toUtf8().data());
		conditonList.push_back(scoreEnd);
	}
	if (!cfg->projectName.isEmpty())
	{
		QString projectName;
		projectName.sprintf("project_name LIKE '%%%s%%'", cfg->projectName.toUtf8().data());
		conditonList.push_back(projectName);
	}
	if (!cfg->gczt.isEmpty() && cfg->gczt.compare("-请选择-") != 0)
	{
		QString gczt;
		gczt.sprintf("gczt = '%s'", cfg->gczt.toUtf8().data());
		conditonList.push_back(gczt);
	}
	if (!cfg->priceBegin.isEmpty())
	{
		QString priceBegin;
		priceBegin.sprintf("zbprice >= %s", cfg->priceBegin.toUtf8().data());
		conditonList.push_back(priceBegin);
	}
	if (!cfg->priceEnd.isEmpty())
	{
		QString priceEnd;
		priceEnd.sprintf("zbprice <= '%s'", cfg->priceEnd.toUtf8().data());
		conditonList.push_back(priceEnd);
	}
	if (!cfg->zbDateBegin.isEmpty() && cfg->zbDateBegin.compare("2000-1-1") != 0)
	{
		QString zbDateBegin;
		zbDateBegin.sprintf("STR_TO_DATE(zbtime,'%%Y-%%m-%%d') >= STR_TO_DATE('%s', '%%Y-%%m-%%d')", cfg->zbDateBegin.toUtf8().data());
		conditonList.push_back(zbDateBegin);
	}
	if (!cfg->zbDateEnd.isEmpty() && cfg->zbDateEnd.compare("2000-1-1") != 0)
	{
		QString zbDateEnd;
		zbDateEnd.sprintf("STR_TO_DATE(zbtime,'%%Y-%%m-%%d') <= STR_TO_DATE('%s', '%%Y-%%m-%%d')", cfg->zbDateEnd.toUtf8().data());
		conditonList.push_back(zbDateEnd);
	}
	if (!cfg->ptyj.isEmpty() && cfg->ptyj.compare("-请选择-") != 0)
	{
		QString ptyj;
		ptyj.sprintf("types = '%s'", cfg->ptyj.toUtf8().data());
		conditonList.push_back(ptyj);
	}
	if (!cfg->projectmgr.isEmpty())
	{
		QString projectmgr;
		projectmgr.sprintf("manager = '%s'", cfg->projectmgr.toUtf8().data());
		conditonList.push_back(projectmgr);
	}
	if (!cfg->mgrstatus.isEmpty() && cfg->mgrstatus.compare("-请选择-") != 0)
	{
		QString mgrstatus;
		mgrstatus.sprintf("manager_status = '%s'", cfg->mgrstatus.toUtf8().data());
		conditonList.push_back(mgrstatus);
	}
	if (!cfg->zc.isEmpty() && cfg->zc.compare("-请选择-") != 0)
	{
		QString zc;
		zc.sprintf("zhicheng = '%s'", cfg->zc.toUtf8().data());
		conditonList.push_back(zc);
	}
	if (!cfg->jzs.isEmpty() && cfg->jzs.compare("-请选择-") != 0)
	{
		QString jzs;
		if (cfg->jzs.compare("市政公用工程二级") == 0)
		{
			jzs = "shizheng_level = '二级'";
		}
		else if (cfg->jzs.compare("市政公用工程一级") == 0)
		{
			jzs = "shizheng_level = '一级'";
		}
		else if (cfg->jzs.compare("建筑工程二级") == 0)
		{
			jzs = "tujian_level = '二级'";
		}
		else if (cfg->jzs.compare("建筑工程一级") == 0)
		{
			jzs = "tujian_level = '一级'";
		}
		conditonList.push_back(jzs);
	}
	if (!cfg->mianjiBegin.isEmpty())
	{
		QString mianjiBegin;
		mianjiBegin.sprintf("mianji >= '%s'", cfg->mianjiBegin.toUtf8().data());
		conditonList.push_back(mianjiBegin);
	}
	if (!cfg->mianjiEnd.isEmpty())
	{
		QString mianjiEnd;
		mianjiEnd.sprintf("mianji <= '%s'", cfg->mianjiEnd.toUtf8().data());
		conditonList.push_back(mianjiEnd);
	}
	if (!cfg->jgDateBegin.isEmpty() && cfg->jgDateBegin.compare("2000-1-1") != 0)
	{
		QString jgDateBegin;
		jgDateBegin.sprintf("STR_TO_DATE(jgtime,'%%Y-%%m-%%d') >= STR_TO_DATE('%s', '%%Y-%%m-%%d')", cfg->jgDateBegin.toUtf8().data());
		conditonList.push_back(jgDateBegin);
	}
	if (!cfg->jgDateEnd.isEmpty() && cfg->jgDateEnd.compare("2000-1-1") != 0)
	{
		QString jgDateEnd;
		jgDateEnd.sprintf("STR_TO_DATE(jgtime,'%%Y-%%m-%%d') <= STR_TO_DATE('%s', '%%Y-%%m-%%d')", cfg->jgDateEnd.toUtf8().data());
		conditonList.push_back(jgDateEnd);
	}
	if (!cfg->ysDateBegin.isEmpty() && cfg->ysDateBegin.compare("2000-1-1") != 0)
	{
		QString ysDateBegin;
		ysDateBegin.sprintf("STR_TO_DATE(ystime,'%%Y-%%m-%%d') >= STR_TO_DATE('%s', '%%Y-%%m-%%d')", cfg->ysDateBegin.toUtf8().data());
		conditonList.push_back(ysDateBegin);
	}
	if (!cfg->ysDateEnd.isEmpty() && cfg->ysDateEnd.compare("2000-1-1") != 0)
	{
		QString ysDateEnd;
		ysDateEnd.sprintf("STR_TO_DATE(ystime,'%%Y-%%m-%%d') <= STR_TO_DATE('%s', '%%Y-%%m-%%d')", cfg->ysDateEnd.toUtf8().data());
		conditonList.push_back(ysDateEnd);
	}
	if (!cfg->projectKind.isEmpty() && cfg->projectKind.compare("-请选择-") != 0)
	{
		QString projectKind;
		if (cfg->projectKind.compare("景观绿化") == 0)
		{
			projectKind = "project_name regexp'景观|绿化|环境'";
		}
		else
		{
			projectKind = "project_name not regexp'景观|绿化|环境'";
		}
		conditonList.push_back(projectKind);
	}
	if (!cfg->qiyezizhi.isEmpty() && cfg->qiyezizhi.compare("-请选择-") != 0)
	{
		QString qiyezizhi;
		qiyezizhi.sprintf("qiyezizhi LIKE '%%%s%%'", cfg->qiyezizhi.toUtf8().data());
		conditonList.push_back(qiyezizhi);
	}

	for (int i = 0; i < conditonList.length(); ++i)
	{
		if (i != 0)
		{
			filter.append(" and ");
		}
		filter.append(conditonList.at(i));
	}
	return filter;
}
DBData* DBManager::queryPerformance()
{
	MysqlPing();
	lastQueryTable = TBL_COMPANY_PERFORMANCE;
	model->setTable("CompanyPerformance");
	model->setEditStrategy(QSqlTableModel::OnFieldChange);
	model->setFilter(generalQueryConditon());
	model->select();
	model->setHeaderData(0, Qt::Horizontal, "企业名称");
	model->setHeaderData(1, Qt::Horizontal, "企业资质");
	model->setHeaderData(2, Qt::Horizontal, "信用分");
	model->setHeaderData(3, Qt::Horizontal, "信用分（旧）");
	model->setHeaderData(5, Qt::Horizontal, "项目名称");
	model->setHeaderData(7, Qt::Horizontal, "业绩在建情况");
	model->setHeaderData(9, Qt::Horizontal, "竣工时间");
	model->setHeaderData(12, Qt::Horizontal, "中标价（万元）");
	model->setHeaderData(13, Qt::Horizontal, "项目经理");
	model->setHeaderData(15, Qt::Horizontal, "职称（高工、工程师）");
	model->setHeaderData(16, Qt::Horizontal, "市政建造师");
	model->setHeaderData(17, Qt::Horizontal, "土建建造师");
	model->setHeaderData(18, Qt::Horizontal, "类别");
	model->setHeaderData(20, Qt::Horizontal, "主要指标");
	model->setHeaderData(21, Qt::Horizontal, "验收证明图片链接");
	model->setHeaderData(22, Qt::Horizontal, "更新日期");
	dataInfo.model = model;
	dataInfo.rowCount = model->rowCount();
	return &dataInfo;
}
DBData* DBManager::queryStatus()
{
	MysqlPing();
	lastQueryTable = TBL_SPIDERRUNNINGSTATUS;
	model->setTable("SpiderRunningStatus");
	model->setEditStrategy(QSqlTableModel::OnManualSubmit);
	model->setSort(3, Qt::DescendingOrder);//第3列降序
	model->select();
	model->setHeaderData(0, Qt::Horizontal, "企业名称");
	model->setHeaderData(1, Qt::Horizontal, "信息");
	model->setHeaderData(2, Qt::Horizontal, "状态");
	model->setHeaderData(3, Qt::Horizontal, "更新日期");
	model->setHeaderData(4, Qt::Horizontal, "最近一次更新成功的日期");
	dataInfo.model = model;
	dataInfo.rowCount = model->rowCount();
	return &dataInfo;
}
LastDBQueryTable DBManager::getLastQueryTable()
{
	return lastQueryTable;
}
AddCompanyResult DBManager::addCompany()
{
	MysqlPing();
	AddCompanyResult ret = RET_SQL_EXCUTE_FAILED;
	if (db.isOpen())
	{
		QSqlQuery query;
		QString sql;
		sql.sprintf("insert into SpiderRunningStatus(companyname) values('%s')",
			ConfigManager::instance()->GetUserCfg()->companyname.toUtf8().data());
		bool success = query.exec(sql);
		if (!success)
		{
			ret = RET_SQL_EXCUTE_FAILED;
			return ret;
		}
		ret = RET_OK;
	}
	else
	{
		ret = RET_DB_NOT_CONNECTED;
	}
	return ret;
}
AddCompanyResult DBManager::checkCompanyIsExist()
{
	MysqlPing();
	AddCompanyResult ret = RET_NOT_EXIST;
	if (db.isOpen())
	{
		QSqlQuery query;
		QString sql;
		sql.sprintf("select count(*) from SpiderRunningStatus where companyname LIKE '%%%s%%'",
			ConfigManager::instance()->GetUserCfg()->companyname.toUtf8().data());
		bool success = query.exec(sql);
		if (!success)
		{
			ret = RET_SQL_EXCUTE_FAILED;
			return ret;
		}
		while (query.next())
		{
			if (query.isActive())
			{
				if (query.value(0).toInt() > 0)
				{
					ret = RET_ALREADY_EXIST;
					return ret;
				}
			}
		}
	}
	else
	{
		ret = RET_DB_NOT_CONNECTED;
	}
	return ret;
}