#include "DataExportHelper.h"
#include "xlsx/xlsxdocument.h"
#include "xlsx/xlsxworkbook.h"
#include "DBManager.h"

void DataExportHelper::run()
{
	int sheet_num = 0;
	int column_offset = 2;
	int row_offset = 3;
	QAbstractItemModel *model = table->model();
	int colcount = model->columnCount();
	int rowcount = model->rowCount();
	const int max_table_field_counts = 23;
	if (DBManager::instance()->getLastQueryTable() == TBL_SPIDERRUNNINGSTATUS)
	{
		sheet_num = 1;
		column_offset = 1;
		row_offset = 1;
	}
	else if (max_table_field_counts == colcount)//防止多次重复导出，导致字段被误删
	{	
		model->removeColumn(4);
		model->removeColumn(5);
		model->removeColumn(6);
		model->removeColumn(7);
		model->removeColumn(7);
		model->removeColumn(9);
		model->removeColumn(13);
		const int deletedFieldTotalNums = 7;
		colcount = max_table_field_counts - deletedFieldTotalNums;
	}
	if (!savedFilePath.isEmpty())
	{
		QXlsx::Document xlsx(templateFilePath);
		QXlsx::Workbook *workBook = xlsx.workbook();
		if (!workBook)
		{
			emit updateJobStatusSignal((int)JOB_WARNING, QString("打开Template.xlsx失败。"));
			return;
		}
		QXlsx::Worksheet *workSheet = static_cast<QXlsx::Worksheet*>(workBook->sheet(sheet_num));
		if (!workSheet)
		{
			emit updateJobStatusSignal((int)JOB_WARNING, QString("打开Sheet页失败。"));
			return;
		}

		int i, j;
		QXlsx::Format format;                       //格式  
		format.setFont(QFont("宋体"));       //字体 
		format.setFontSize(9);
		format.setHorizontalAlignment(QXlsx::Format::AlignLeft);
		format.setVerticalAlignment(QXlsx::Format::AlignVCenter);
		format.setBorderStyle(QXlsx::Format::BorderThin);
		//QTableWidget 获取表格数据部分
		int seq_num = 1;
		for (i = 0; i < rowcount; i++)
		{
			//第一列序号
			workSheet->write(i + row_offset, 1, seq_num, format);
			++seq_num;
			for (j = 0; j < colcount; j++)
			{

				workSheet->write(i + row_offset, j + column_offset, model->data(model->index(i, j)), format);
			}
		}
		const double row_height = 25.5;
		workSheet->setRowHeight(row_offset, row_offset + rowcount - 1, row_height);
		xlsx.saveAs(savedFilePath);//保存至fileName

		emit updateJobStatusSignal((int)JOB_QUESTION, savedFilePath);
	}
}
void DataExportHelper::setFilePath(QString& path)
{
	savedFilePath = path;
	isFilePathSeted = true;
}
void DataExportHelper::reset()
{
	isFilePathSeted = false;
}
bool DataExportHelper::getFilePathSetStatus()
{
	return isFilePathSeted;
}
void DataExportHelper::DoExport(QTableView* tableView)
{
	table = tableView;
	start();
}