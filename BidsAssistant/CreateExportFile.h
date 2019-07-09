#pragma once

#include <QWidget>
#include "ui_CreateExportFile.h"
#include "GlobalDefine.h"

class CreateExportFile : public QWidget
{
	Q_OBJECT

public:
	CreateExportFile(QWidget *parent = Q_NULLPTR);
	~CreateExportFile();
protected:
	int createFile(QString filePath, QString fileName);
private slots:
	void btnCreateFileClicked();
	void btnChooseDirClicked();
	void btnCancleClicked();
	void fileNameInputed();
signals:
	void doExportNow();

private:
	Ui::CreateExportFile ui;
	QString filePath;
};
