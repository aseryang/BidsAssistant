#include "CreateExportFile.h"
#include "DataExportHelper.h"
#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QDebug>

CreateExportFile::CreateExportFile(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.btnCreateFile->setEnabled(false);
	QDir tempDir;
	//临时保存程序当前路径
	QString currentDir = tempDir.currentPath();
	ui.fileDir->setText(currentDir);
	ui.btnChooseDir->setFlat(true);
	setWindowIcon(QIcon(":/Resources/icon1.png"));
	setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
}

CreateExportFile::~CreateExportFile()
{
}
int CreateExportFile::createFile(QString filePath, QString fileName)
{
	QDir tempDir;
	//临时保存程序当前路径
	QString currentDir = tempDir.currentPath();
	//如果filePath路径不存在，创建它
	if (!tempDir.exists(filePath))
	{
		qDebug() << "不存在该路径" << endl;
		tempDir.mkpath(filePath);
	}
	QFile *tempFile = new QFile;
	//将程序的执行路径设置到filePath下
	tempDir.setCurrent(filePath);
	qDebug() << tempDir.currentPath();
	//检查filePath路径下是否存在文件fileName,如果停止操作。
	if (tempFile->exists(fileName))
	{
		qDebug() << "文件存在";
		return 0;
	}
	//此时，路径下没有fileName文件，使用下面代码在当前路径下创建文件
	tempFile->setFileName(fileName);
	if (!tempFile->open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qDebug() << "打开失败";
		return 1;
	}
	tempFile->close();
	//将程序当前路径设置为原来的路径
	tempDir.setCurrent(currentDir);
	qDebug() << tempDir.currentPath();
	return 0;
}
void CreateExportFile::fileNameInputed()
{
	if (ui.fileName->text().isEmpty())
	{
		ui.btnCreateFile->setEnabled(false);
	}
	else
	{
		ui.btnCreateFile->setEnabled(true);
	}
}
void CreateExportFile::btnCreateFileClicked()
{
	if (ui.fileDir->text().isEmpty() ||
		ui.fileName->text().isEmpty())
	{
		return;
	}

	filePath = ui.fileDir->text() + "\\" + ui.fileName->text() + FILE_POSTFIX;
	filePath.replace("\\", "/");
	DataExportHelper::instance()->setFilePath(filePath);
	hide();
	emit doExportNow();
}
void CreateExportFile::btnChooseDirClicked()
{
	QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;

	QString directory = QFileDialog::getExistingDirectory(this,
		"选择文件夹",
		ui.fileDir->text(),
		options);
	if (!directory.isEmpty())
		ui.fileDir->setText(directory);
}
void CreateExportFile::btnCancleClicked()
{
	DataExportHelper::instance()->reset();
	hide();
}