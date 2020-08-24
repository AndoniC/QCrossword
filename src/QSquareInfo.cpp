#include <QtGui>
#include "QSquareInfo.h"


QSquareInfo::QSquareInfo( QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi( this );
	
}
QSquareInfo::~QSquareInfo()
{
}

int QSquareInfo::getIndex(QComboBox *c, QString text)
{
	
	text= text.toUpper();
	for (int i=0;i<c->count();i++)
	{
		c->setCurrentIndex(i);
		if (c->currentText().compare(text)==0) return i;
	}
}
void QSquareInfo::fill(nlohmann::json &info)
{

}

void QSquareInfo::enableDefs(bool f)
{
	ui.comboBox_Def1_Dir->setDisabled(!f);
	ui.comboBox_Def1_Pos->setDisabled(!f);
	ui.textEdit_Def1->setDisabled(!f);
	ui.textEdit_Def2->setDisabled(!f);
	ui.comboBox_Def2_Dir->setDisabled(!f);
	ui.comboBox_Def2_Pos->setDisabled(!f);
}
void QSquareInfo::enableChar(bool f)
{

}
void QSquareInfo::on_groupBox_Defs_clicked()
{


}

void QSquareInfo::on_groupBox_Char_clicked()
{
	

}
void QSquareInfo::on_checkBox_Null_clicked()
{

	
}
void QSquareInfo::on_pushButtonUpdate_pressed()
{
	CrossWordData::crossword_key_t info;
	
	if (ui.groupBox_Defs->isChecked())
	{

		if (!ui.textEdit_Def1->toPlainText().isEmpty())
		{

			info.def = ui.textEdit_Def1->toPlainText().toLocal8Bit().data();
			info.answers.resize(ui.spinBox_ncasillas1->value());
			info.direction = ui.comboBox_Def1_Dir->currentText().toLocal8Bit().data();
			info.first_point = ui.comboBox_Def1_Pos->currentText().toLocal8Bit().data();
		}

		DataManager::addKey(info);

		if (!ui.textEdit_Def2->toPlainText().isEmpty())
		{

			info.def = ui.textEdit_Def2->toPlainText().toLocal8Bit().data();
			info.direction = ui.comboBox_Def2_Dir->currentText().toLocal8Bit().data();
			info.first_point = ui.comboBox_Def2_Pos->currentText().toLocal8Bit().data();
		}
		DataManager::addKey(info);
		emit update();
	}
	else
		DataManager::clearTile();
	
}
void QSquareInfo::on_pushButton_Close_pressed()
{
	this->hide();

}