#include "Login.h"
#include "ConfigManager.h"
#include "DBManager.h"
#include "GlobalDefine.h"
#include <QMessageBox>
#include "LoginHelper.h"

Login::Login(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	QRegExp rx("[0-9.]+$");
	QRegExpValidator *validator = new QRegExpValidator(rx, this);
	ui.le_ip->setValidator(validator);
	ConfigManager::instance()->init();
	QString ipstr = ConfigManager::instance()->getIP();
	ui.le_ip->setText(ipstr);
	qRegisterMetaType<eLoginResult>("eLoginResult");
	connect(LoginHelper::instance(), SIGNAL(loginResultSignal(eLoginResult)), this, SLOT(onLoginSlot(eLoginResult)));
	setWindowIcon(QIcon(":/Resources/icon1.png"));
	setFixedSize(width(), height());
}

Login::~Login()
{
}
void Login::keyPressEvent(QKeyEvent * event)
{
	if (event->key() == Qt::Key_Return && ui.btn_login->isEnabled())
	{
		btnLoginSlot();
	}	
}
void Login::btnLoginSlot()
{
	QString ipstr = ui.le_ip->text().trimmed();
	if (ipstr.isEmpty())
	{
		QMessageBox::information(this, "提示", "请输入正确的服务器IP地址！", "确定");
		return;
	}
	setWindowTitle("正在登录...");
	ui.le_ip->setEnabled(false);
	ui.btn_login->setEnabled(false);
	LoginHelper::instance()->DoLogin(ipstr);
	ConfigManager::instance()->setIp(ipstr);
}
void Login::onLoginSlot(eLoginResult result)
{
	ui.btn_login->setEnabled(true);
	ui.le_ip->setEnabled(true);
	if (result == LOGIN_SUCCEED)
	{
		emit showMainWndSignal();
		setVisible(false);
	}
	else
	{
		setWindowTitle("登录");
		QMessageBox::warning(this, "提示", "登录失败，请输入正确的服务器IP地址！", "确定");
	}
}
