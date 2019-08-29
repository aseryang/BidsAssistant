#pragma once
#pragma execution_character_set("utf-8") //设置文件编码格式为UTF-8
#define USER_CONFIG "UserConfig.ini"
#define DB_NAME "mydb"
#if 0
	#define USER_NAME "ssl_test"
	#define USER_PASSWORD "123456"
#else
	#define USER_NAME "jiashengjingguan"
	#define USER_PASSWORD "Qwe123!@#"
#endif
#define DB_PORT 3306
#define APP_VERSION "V1.1"
#define APP_TITLE "南京嘉盛景观投标助手 "
#define FILE_POSTFIX ".xlsx"


#include <QSqlTableModel>
enum eAppStatus{
	STAT_NORMAL,
	STAT_LOGIN,
	STAT_ADD_COMPANY,
	STAT_QUERYING,
	STAT_CHECKING,
	STAT_EXPORTING,
};
enum eMsgID{
	ID_UDPATE_QUERY_RESULT,
	ID_UDPATE_CHECK_RESULT,
	ID_ADD_COMPANY_RESULT,
};
enum JobStatus
{
	JOB_FINISH,
	JOB_WARNING,
	JOB_QUESTION,
};
enum AddCompanyResult{
	RET_OK,
	RET_DB_NOT_CONNECTED,
	RET_ALREADY_EXIST,
	RET_NOT_EXIST,
	RET_SQL_EXCUTE_FAILED,
};
enum LastDBQueryTable
{
	TBL_COMPANY_PERFORMANCE,
	TBL_SPIDERRUNNINGSTATUS,
};
struct DBData
{
	QSqlTableModel* model;
	int				rowCount;
	DBData() :model(nullptr), rowCount(0){}
};

enum eLoginResult{
	LOGIN_SUCCEED,
	LOGIN_FAILED,
};

struct UserConfig
{
	QString companyname;
	QString scoreBegin;
	QString scoreEnd;
	QString projectName;
	QString gczt;
	QString priceBegin;
	QString priceEnd;
	QString zbDateBegin;
	QString zbDateEnd;
	QString ptyj;
	QString projectmgr;
	QString mgrstatus;
	QString zc;
	QString jzs;
	QString mianjiBegin;
	QString mianjiEnd;
	QString jgDateBegin;
	QString jgDateEnd;
	QString ysDateBegin;
	QString ysDateEnd;
	QString projectKind;
	QString qiyezizhi;
};