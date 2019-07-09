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
	gcztList.push_back("-��ѡ��-");
	gcztList.push_back("�ڽ�");
	gcztList.push_back("�ѿ���");
	ptyjList.push_back("-��ѡ��-");
	ptyjList.push_back("��ƽ̨");
	ptyjList.push_back("�Ա�");
	zcList.push_back("-��ѡ��-");
	zcList.push_back("�߼�����ʦ");
	zcList.push_back("����ʦ");
	zcList.push_back("������ʦ");
	jzsList.push_back("-��ѡ��-");
	jzsList.push_back("�������ù���һ��");
	jzsList.push_back("�������ù��̶���");
	jzsList.push_back("��������һ��");
	jzsList.push_back("�������̶���");
	//jzsList.push_back("ˮ��ˮ�繤��һ��");
	//jzsList.push_back("ˮ��ˮ�繤�̶���");
	gclbList.push_back("-��ѡ��-");
	gclbList.push_back("�����̻�");
	gclbList.push_back("����");
	qyzzList << "-��ѡ��-" << "����԰���̻�һ��" << "����԰���̻�����" << "����԰���̻�����";
	qyzzList << "�������ù����ؼ�" << "�������ù���һ��" << "�������ù��̶���" <<"�������ù�������";
	qyzzList << "����װ��װ�ι���һ��" << "����װ��װ�ι��̶���" << "����װ��װ�ι�������";
	qyzzList << "�Ž�������һ��" << "�Ž������̶���" << "�Ž�����������";
	qyzzList << "���������ؼ�" << "��������һ��" << "�������̶���" << "������������";
	qyzzList << "��������һ��" << "�������̶���" << "������������";
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
		QMessageBox::warning(NULL, "��ʾ", "δ������Ч����ҵ���ƣ�", "ȷ��"/*QMessageBox::Apply*/);
		return;
	}
	QString info;
	info.sprintf("�Ƿ�������ҵ��%s ?", ui.le_companyname->text().trimmed().toUtf8().data());
	if (QMessageBox::question(NULL, "��ʾ", info, "��", "��") == 0)
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
		//ui.statusBar->showMessage("�����¼��ɡ�");
		QMessageBox::information(this, "���", info);
		break;
	}
	case JOB_WARNING:
	{
		ui.statusBar->showMessage(info);
		QMessageBox::warning(NULL, "����", info, "ȷ��"/*QMessageBox::Apply*/);
		break;
	}
	case JOB_QUESTION:
	{
		ui.statusBar->showMessage("������¼��ɡ�");
		if (QMessageBox::question(NULL, "���", "�ļ��Ѿ��������Ƿ����ڴ򿪣�", "��", "��") == 0)
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
		info.sprintf("�� %d ����¼��", dataInfo->rowCount);
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
		info.sprintf("�� %d ����¼��", dataInfo->rowCount);
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
		QMessageBox::information(NULL, "�ɹ�", "������ҵ�ɹ���", "ȷ��");
		break;
	case RET_ALREADY_EXIST:
		QMessageBox::warning(NULL, "��ʾ", "�Ѵ��ڸ���ҵ��", "ȷ��"/*QMessageBox::Apply*/);
		break;
	case RET_DB_NOT_CONNECTED:
		QMessageBox::warning(NULL, "����", "���ݿ�����ʧЧ�������µ�¼��", "ȷ��"/*QMessageBox::Apply*/);
		break;
	case RET_SQL_EXCUTE_FAILED:
		QMessageBox::warning(NULL, "����", "�����ҵʧ�ܣ�SQLִ��ʧ�ܣ�", "ȷ��"/*QMessageBox::Apply*/);
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
		setWindowTitle("������ӹ�˾...- " + QString(APP_TITLE) + APP_VERSION);
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
		setWindowTitle("���ڲ�ѯ...- " + QString(APP_TITLE) + APP_VERSION);
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
		setWindowTitle("���ڻ�ȡ����״̬...- " + QString(APP_TITLE) + APP_VERSION);
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
		setWindowTitle("���ڵ���...- " + QString(APP_TITLE) + APP_VERSION);
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