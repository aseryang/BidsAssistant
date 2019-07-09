#pragma once
#include <QThread>
#include "GlobalDefine.h"

class LoginHelper: public QThread
{
	Q_OBJECT
private:
	LoginHelper(){}
public:
	~LoginHelper(){}
	static LoginHelper* instance()
	{
		static LoginHelper* handle = new LoginHelper;
		return handle;
	}
	void DoLogin(QString ipstr);
	void run();
signals:
	void loginResultSignal(eLoginResult result);
protected:
private:
	QString ip;
};