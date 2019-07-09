#pragma once

#include <QWidget>
#include "ui_Login.h"
#include "GlobalDefine.h"
#include <QKeyEvent>

class Login : public QWidget
{
	Q_OBJECT

public:
	Login(QWidget *parent = Q_NULLPTR);
	~Login();
	void keyPressEvent(QKeyEvent * event);
public slots:
	void btnLoginSlot();
	void onLoginSlot(eLoginResult result);
signals:
	void showMainWndSignal();

private:
	Ui::Login ui;
};
