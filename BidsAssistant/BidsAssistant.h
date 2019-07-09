#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_BidsAssistant.h"
#include "Login.h"
#include "GlobalDefine.h"
#include "CreateExportFile.h"
#include <QKeyEvent>

class BidsAssistant : public QMainWindow
{
	Q_OBJECT

public:
	BidsAssistant(QWidget *parent = Q_NULLPTR);
	void UIInit();
	void enterStatus(eAppStatus newStatus);
	void UpdateAppStatus(eAppStatus status);
	void GetUserConfig();
public:
	void keyPressEvent(QKeyEvent * event);
public slots:
	void showMainWndSlot();
	void btnQueryClickedSlot();
	void btnCheckStatusSlot();
	void btnExportDataSlot();
	void btnAddCompanySlot();
	void btnResetSlot();
	void jobFinished();
	void onUpdateJobStatus(int status, QString info);
	void onUpdateUISlot(void* data, eMsgID id);
	void onExportNowSlot();
	void handleAddCompanyResult(AddCompanyResult result);
	

private:
	Ui::BidsAssistantClass ui;
	Login loginWnd;
	CreateExportFile exportWnd;
	eAppStatus curAppStatus;
};
