#include "BidsAssistant.h"
#include "ConfigManager.h"
#include "DBManager.h"
#include <QSqlTableModel>
#include "DBHelper.h"
#include "DataExportHelper.h"
#include <QMessageBox>
#include <QDesktopServices>
#include <QDir>
#include <QUrl>

BidsAssistant::BidsAssistant(QWidget *parent)
	: QMainWindow(parent), curAppStatus(STAT_NORMAL)
{
	ui.setupUi(this);
	UIInit();
	ConfigManager::instance()->init();
	connect(&loginWnd, SIGNAL(showMainWndSignal()), this, SLOT(showMainWndSlot()));
	connect(&exportWnd, SIGNAL(doExportNow()), this, SLOT(onExportNowSlot()));
	connect(DBHelper::instance(), SIGNAL(jobFinishSignal()), this, SLOT(jobFinished()));
	qRegisterMetaType<eMsgID>("eMsgID");
	connect(DBHelper::instance(), SIGNAL(updateUI(void*, eMsgID)), this, SLOT(onUpdateUISlot(void*, eMsgID)));
	connect(DataExportHelper::instance(), SIGNAL(updateJobStatusSignal(int, QString)), this, SLOT(onUpdateJobStatus(int, QString)));
	loginWnd.setVisible(true);
	if (!ConfigManager::instance()->getIP().trimmed().isEmpty())
	{
		loginWnd.btnLoginSlot();
	}
}
void BidsAssistant::UIInit()
{
	ui.dateedit_zb_start->setCalendarPopup(true);
	ui.dateedit_zb_end->setCalendarPopup(true);
	ui.dateedit_jg_start->setCalendarPopup(true);
	ui.dateedit_jg_end->setCalendarPopup(true);
	ui.dateedit_ys_start->setCalendarPopup(true);
	ui.dateedit_ys_end->setCalendarPopup(true);
	QStringList gcztList, ptyjList, zcList, jzsList, gclbList, qyzzList;
	gcztList.push_back("-请选择-");
	gcztList.push_back("在建");
	gcztList.push_back("已竣工");
	ptyjList.push_back("-请选择-");
	ptyjList.push_back("市平台");
	ptyjList.push_back("自备");
	zcList.push_back("-请选择-");
	zcList.push_back("高级工程师");
	zcList.push_back("工程师");
	zcList.push_back("助理工程师");
	jzsList.push_back("-请选择-");
	jzsList.push_back("市政公用工程一级");
	jzsList.push_back("市政公用工程二级");
	jzsList.push_back("建筑工程一级");
	jzsList.push_back("建筑工程二级");
	//jzsList.push_back("水利水电工程一级");
	//jzsList.push_back("水利水电工程二级");
	gclbList.push_back("-请选择-");
	gclbList.push_back("景观绿化");
	gclbList.push_back("其他");
	qyzzList << "-请选择-" << "城市园林绿化一级" << "城市园林绿化二级" << "城市园林绿化三级";
	qyzzList << "市政公用工程特级" << "市政公用工程一级" << "市政公用工程二级" <<"市政公用工程三级";
	qyzzList << "建筑装修装饰工程一级" << "建筑装修装饰工程二级" << "建筑装修装饰工程三级";
	qyzzList << "古建筑工程一级" << "古建筑工程二级" << "古建筑工程三级";
	qyzzList << "建筑工程特级" << "建筑工程一级" << "建筑工程二级" << "建筑工程三级";
	qyzzList << "环保工程一级" << "环保工程二级" << "环保工程三级";
	ui.cb_gczt->addItems(gcztList);
	ui.cb_zjqk->addItems(gcztList);
	ui.cb_ptyj->addItems(ptyjList);
	ui.cb_jzs->addItems(jzsList);
	ui.cb_gclb->addItems(gclbList);
	ui.cb_zc->addItems(zcList);
	ui.cb_qiyezizhi->addItems(qyzzList);
	QRegExp rx("[0-9.]+$");
	QRegExpValidator *validator = new QRegExpValidator(rx, this);
	ui.le_mianji_begin->setValidator(validator);
	ui.le_mianji_end->setValidator(validator);
	ui.le_price_begin->setValidator(validator);
	ui.le_price_end->setValidator(validator);
}
void BidsAssistant::showMainWndSlot()
{
	setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
	show();
}
void BidsAssistant::btnResetSlot()
{
	ui.le_companyname->clear();
	ui.le_mianji_begin->clear();
	ui.le_mianji_end->clear();
	ui.le_price_begin->clear();
	ui.le_price_end->clear();
	ui.le_projectmgr_name->clear();
	ui.le_projectname->clear();
	ui.cb_gclb->setCurrentIndex(0);
	ui.cb_gczt->setCurrentIndex(0);
	ui.cb_jzs->setCurrentIndex(0);
	ui.cb_ptyj->setCurrentIndex(0);
	ui.cb_zc->setCurrentIndex(0);
	ui.cb_zjqk->setCurrentIndex(0);
	ui.cb_qiyezizhi->setCurrentIndex(0);
	QDate date = QDate(2000, 1, 1);
	ui.dateedit_jg_start->setDate(date);
	ui.dateedit_jg_end->setDate(date);
	ui.dateedit_ys_end->setDate(date);
	ui.dateedit_ys_start->setDate(date);
	ui.dateedit_zb_end->setDate(date);
	ui.dateedit_zb_start->setDate(date);
}
void BidsAssistant::keyPressEvent(QKeyEvent * event)
{
	if (event->key() == Qt::Key_Return && ui.btn_query->isEnabled())
	{
		enterStatus(STAT_QUERYING);
	}
}
void BidsAssistant::btnQueryClickedSlot()
{
	enterStatus(STAT_QUERYING);
}
void BidsAssistant::btnCheckStatusSlot()
{
	enterStatus(STAT_CHECKING);
}
void BidsAssistant::btnExportDataSlot()
{
	DataExportHelper::instance()->reset();	
	exportWnd.setWindowModality(Qt::ApplicationModal);
	exportWnd.show();

}
void BidsAssistant::onExportNowSlot()
{
	if (DataExportHelper::instance()->getFilePathSetStatus())
	{
		enterStatus(STAT_EXPORTING);
	}
}
void BidsAssistant::btnAddCompanySlot()
{
	if (ui.le_companyname->text().trimmed().isEmpty())
	{
		QMessageBox::warning(NULL, "提示", "未填入有效的企业名称！", "确定"/*QMessageBox::Apply*/);
		return;
	}
	QString info;
	info.sprintf("是否增加企业：%s ?", ui.le_companyname->text().trimmed().toUtf8().data());
	if (QMessageBox::question(NULL, "提示", info, "是", "否") == 0)
	{
		enterStatus(STAT_ADD_COMPANY);
	}
	else
	{
		return;
	}
}
void BidsAssistant::jobFinished()
{
	enterStatus(STAT_NORMAL);
}
void BidsAssistant::onUpdateJobStatus(int status, QString info)
{
	switch (status)
	{
	case JOB_FINISH:
	{
		//ui.statusBar->showMessage("导入记录完成。");
		QMessageBox::information(this, "完成", info);
		break;
	}
	case JOB_WARNING:
	{
		ui.statusBar->showMessage(info);
		QMessageBox::warning(NULL, "错误", info, "确定"/*QMessageBox::Apply*/);
		break;
	}
	case JOB_QUESTION:
	{
		ui.statusBar->showMessage("导出记录完成。");
		if (QMessageBox::question(NULL, "完成", "文件已经导出，是否现在打开？", "是", "否") == 0)
		{
			QDesktopServices::openUrl(QUrl("file:///" + QDir::toNativeSeparators(info)));
		}
		break;
	}
	}
	enterStatus(STAT_NORMAL);
}
void BidsAssistant::onUpdateUISlot(void* data, eMsgID id)
{
	switch (id)
	{
	case ID_UDPATE_QUERY_RESULT:
	{
		DBData* dataInfo = (DBData*)data;
		ui.tv_tabledata->setModel(dataInfo->model);
		ui.tv_tabledata->hideColumn(4);
		ui.tv_tabledata->hideColumn(6);
		ui.tv_tabledata->hideColumn(8);
		ui.tv_tabledata->hideColumn(10);
		ui.tv_tabledata->hideColumn(11);
		ui.tv_tabledata->hideColumn(14);
		ui.tv_tabledata->hideColumn(19);
		ui.tv_tabledata->resizeColumnsToContents();
		ui.tv_tabledata->setColumnWidth(1, 150);
		ui.tv_tabledata->setColumnWidth(5, 450);
		ui.tv_tabledata->setColumnWidth(20, 400);
		ui.tv_tabledata->setColumnWidth(21, 400);
		QString info;
		info.sprintf("共 %d 条记录。", dataInfo->rowCount);
		ui.statusBar->showMessage(info);
		break;
	}
	case ID_UDPATE_CHECK_RESULT:
	{
		DBData* dataInfo = (DBData*)data;
		ui.tv_tabledata->setModel(dataInfo->model);
		//ui.tv_tabledata->show();
		ui.tv_tabledata->resizeColumnsToContents();
		ui.tv_tabledata->setColumnWidth(1, 580);
		ui.tv_tabledata->setColumnWidth(2, 60);
		ui.tv_tabledata->setColumnWidth(3, 180);
		ui.tv_tabledata->setColumnWidth(4, 180);
		QString info;
		info.sprintf("共 %d 条记录。", dataInfo->rowCount);
		ui.statusBar->showMessage(info);
		break;
	}
	case ID_ADD_COMPANY_RESULT:
	{
		handleAddCompanyResult(*((AddCompanyResult*)data));
		break;
	}
	default:
		break;
	}
}
void BidsAssistant::handleAddCompanyResult(AddCompanyResult result)
{
	switch (result)
	{
	case RET_OK:
		QMessageBox::information(NULL, "成功", "增加企业成功。", "确定");
		break;
	case RET_ALREADY_EXIST:
		QMessageBox::warning(NULL, "提示", "已存在该企业。", "确定"/*QMessageBox::Apply*/);
		break;
	case RET_DB_NOT_CONNECTED:
		QMessageBox::warning(NULL, "错误", "数据库连接失效，请重新登录！", "确定"/*QMessageBox::Apply*/);
		break;
	case RET_SQL_EXCUTE_FAILED:
		QMessageBox::warning(NULL, "错误", "添加企业失败，SQL执行失败！", "确定"/*QMessageBox::Apply*/);
		break;
	default:
		break;
	}
}
void BidsAssistant::GetUserConfig()
{
	UserConfig* cfg = ConfigManager::instance()->GetUserCfg();
	cfg->companyname = ui.le_companyname->text();
	cfg->projectName = ui.le_projectname->text();
	cfg->gczt = ui.cb_gczt->currentText();
	cfg->priceBegin = ui.le_price_begin->text();
	cfg->priceEnd = ui.le_price_end->text();
	cfg->zbDateBegin = ui.dateedit_zb_start->date().toString("yyyy-M-d");
	cfg->zbDateEnd = ui.dateedit_zb_end->date().toString("yyyy-M-d");
	cfg->ptyj = ui.cb_ptyj->currentText();
	cfg->projectmgr = ui.le_projectmgr_name->text();
	cfg->mgrstatus = ui.cb_zjqk->currentText();
	cfg->zc = ui.cb_zc->currentText();
	cfg->jzs = ui.cb_jzs->currentText();
	cfg->mianjiBegin = ui.le_mianji_begin->text();
	cfg->mianjiEnd = ui.le_mianji_end->text();
	cfg->jgDateBegin = ui.dateedit_jg_start->date().toString("yyyy-M-d");
	cfg->jgDateEnd = ui.dateedit_jg_end->date().toString("yyyy-M-d");
	cfg->ysDateBegin = ui.dateedit_ys_start->date().toString("yyyy-M-d");
	cfg->ysDateEnd = ui.dateedit_ys_end->date().toString("yyyy-M-d");
	cfg->projectKind = ui.cb_gclb->currentText();
	cfg->qiyezizhi = ui.cb_qiyezizhi->currentText();
}
void BidsAssistant::enterStatus(eAppStatus newStatus)
{
	if (curAppStatus == newStatus)
	{
		return;
	}
	GetUserConfig();
	switch (newStatus)
	{
	case STAT_NORMAL:
		break;
	case STAT_ADD_COMPANY:
		DBHelper::instance()->AddCompany();
		break;
	case STAT_QUERYING:
		ui.tv_tabledata->setUpdatesEnabled(true);
		ui.tv_tabledata->setModel(NULL);
		DBHelper::instance()->Query();
		break;
	case STAT_CHECKING:
		ui.tv_tabledata->setUpdatesEnabled(true);
		ui.tv_tabledata->setModel(NULL);
		DBHelper::instance()->CheckStatus();
		break;
	case STAT_EXPORTING:
		ui.tv_tabledata->setUpdatesEnabled(false);
		DataExportHelper::instance()->DoExport(ui.tv_tabledata);
		break;
	default:
		break;
	}
	curAppStatus = newStatus;
	UpdateAppStatus(curAppStatus);
}
void BidsAssistant::UpdateAppStatus(eAppStatus status)
{
	switch (status)
	{
	case STAT_NORMAL:
	{
		setWindowTitle(QString(APP_TITLE) + APP_VERSION);
		ui.btn_add->setEnabled(true);
		ui.btn_checkstatus->setEnabled(true);
		ui.btn_export->setEnabled(true);
		ui.btn_query->setEnabled(true);
		ui.btn_reset->setEnabled(true);
		break;
	}
	case STAT_ADD_COMPANY:
	{
		setWindowTitle("正在添加公司...- " + QString(APP_TITLE) + APP_VERSION);
		ui.statusBar->showMessage("");
		ui.btn_add->setEnabled(false);
		ui.btn_checkstatus->setEnabled(false);
		ui.btn_export->setEnabled(false);
		ui.btn_query->setEnabled(false);
		ui.btn_reset->setEnabled(false);
		break;
	}
	case STAT_QUERYING:
	{
		setWindowTitle("正在查询...- " + QString(APP_TITLE) + APP_VERSION);
		ui.statusBar->showMessage("");
		ui.btn_add->setEnabled(false);
		ui.btn_checkstatus->setEnabled(false);
		ui.btn_export->setEnabled(false);
		ui.btn_query->setEnabled(false);
		ui.btn_reset->setEnabled(false);
		break;
	}
	case STAT_CHECKING:
	{
		setWindowTitle("正在获取服务状态...- " + QString(APP_TITLE) + APP_VERSION);
		ui.statusBar->showMessage("");
		ui.btn_add->setEnabled(false);
		ui.btn_checkstatus->setEnabled(false);
		ui.btn_export->setEnabled(false);
		ui.btn_query->setEnabled(false);
		ui.btn_reset->setEnabled(false);
		break;
	}
	case STAT_EXPORTING:
	{
		setWindowTitle("正在导出...- " + QString(APP_TITLE) + APP_VERSION);
		ui.statusBar->showMessage("");
		ui.btn_add->setEnabled(false);
		ui.btn_checkstatus->setEnabled(false);
		ui.btn_export->setEnabled(false);
		ui.btn_query->setEnabled(false);
		ui.btn_reset->setEnabled(false);
		break;
	}
	default:
		break;
	}
}