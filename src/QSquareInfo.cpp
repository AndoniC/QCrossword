#include <QtGui>
#include "QSquareInfo.h"
#include "DataManager.h"

QSquareInfo::QSquareInfo( QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi( this );
	
	ui.comboBox_Def1_Dir->clear();
	ui.comboBox_Def2_Dir->clear();
	ui.comboBox_Def1_Pos->clear();
	ui.comboBox_Def2_Pos->clear();

	//ui.comboBox_Def1_Dir
	ui.comboBox_Def1_Pos->insertItem(0,START_POSITION::to_string(START_POSITION::TOP).c_str());
	ui.comboBox_Def1_Pos->insertItem(1, START_POSITION::to_string(START_POSITION::BOTTOM).c_str());
	ui.comboBox_Def1_Pos->insertItem(2, START_POSITION::to_string(START_POSITION::LEFT).c_str());
	ui.comboBox_Def1_Pos->insertItem(3, START_POSITION::to_string(START_POSITION::RIGHT).c_str());
	ui.comboBox_Def2_Pos->insertItem(0, START_POSITION::to_string(START_POSITION::TOP).c_str());
	ui.comboBox_Def2_Pos->insertItem(1, START_POSITION::to_string(START_POSITION::BOTTOM).c_str());
	ui.comboBox_Def2_Pos->insertItem(2, START_POSITION::to_string(START_POSITION::LEFT).c_str());
	ui.comboBox_Def2_Pos->insertItem(3, START_POSITION::to_string(START_POSITION::RIGHT).c_str());

	ui.comboBox_Def1_Dir->insertItem(0, DIRECTION::to_string(DIRECTION::UP).c_str());
	ui.comboBox_Def1_Dir->insertItem(1, DIRECTION::to_string(DIRECTION::DOWN).c_str());
	ui.comboBox_Def1_Dir->insertItem(2, DIRECTION::to_string(DIRECTION::LEFT).c_str());
	ui.comboBox_Def1_Dir->insertItem(3, DIRECTION::to_string(DIRECTION::RIGHT).c_str());
	ui.comboBox_Def2_Dir->insertItem(0, DIRECTION::to_string(DIRECTION::UP).c_str());
	ui.comboBox_Def2_Dir->insertItem(1, DIRECTION::to_string(DIRECTION::DOWN).c_str());
	ui.comboBox_Def2_Dir->insertItem(2, DIRECTION::to_string(DIRECTION::LEFT).c_str());
	ui.comboBox_Def2_Dir->insertItem(3, DIRECTION::to_string(DIRECTION::RIGHT).c_str());


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
void QSquareInfo::fill(KeyTile *kt)
{
	if (!kt) return;

	int cont = 0;
	for (int i=0;i<kt->defs.size();i++)
	{
		KeyTile::crossword_key_t & one_def = kt->defs[i];
		if (cont == 0)
		{
			//one_def.prin_out();

			ui.textEdit_Def1->setText(ext::string::wstring_to_utf8(one_def.def).c_str());
			ui.spinBox_ncasillas1->setValue(one_def.answers.size());
			std::string dir = ext::string::toUpperCase(one_def.direction);
			std::string pos = ext::string::toUpperCase(one_def.first_point);
			ui.comboBox_Def1_Dir->setCurrentText(dir.c_str());
			ui.comboBox_Def1_Pos->setCurrentText(pos.c_str());
		}
		else if (cont == 1)
		{
			
			
			ui.textEdit_Def2->setText(ext::string::wstring_to_utf8(one_def.def).c_str());
			ui.spinBox_ncasillas2->setValue(one_def.answers.size());
			std::string dir = ext::string::toUpperCase(one_def.direction);
			std::string pos = ext::string::toUpperCase(one_def.first_point);
			ui.comboBox_Def2_Dir->setCurrentText(dir.c_str());
			ui.comboBox_Def2_Pos->setCurrentText(pos.c_str());
		}

		cont++;
	}
	if (cont > 0)
		enableDefs(true);



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
	KeyTile::crossword_key_t info;
	
	if (ui.groupBox_Defs->isChecked())
	{
		// we have to remove the key from json structure and from map_square_key structure
		// in map_square_key we should also delete linked squares?
		DataManager::removeKey();

		if (!ui.textEdit_Def1->toPlainText().isEmpty())
		{

			info.def = ext::string::string_to_wstring(ui.textEdit_Def1->toPlainText().toLocal8Bit().data());
			info.answers.resize(ui.spinBox_ncasillas1->value());

			info.direction = ui.comboBox_Def1_Dir->currentText().toLocal8Bit().data();
			info.first_point = ui.comboBox_Def1_Pos->currentText().toLocal8Bit().data();
			if (info.answers.size() != ui.spinBox_ncasillas1->value())
			{
				ui.spinBox_ncasillas1->setValue(info.answers.size());
			}

		}
		
		DataManager::addKey(info);

		if (!ui.textEdit_Def2->toPlainText().isEmpty())
		{

			info.def = ext::string::string_to_wstring(ui.textEdit_Def2->toPlainText().toLocal8Bit().data());
			info.answers.resize(ui.spinBox_ncasillas2->value());
			info.direction = ui.comboBox_Def2_Dir->currentText().toLocal8Bit().data();
			info.first_point = ui.comboBox_Def2_Pos->currentText().toLocal8Bit().data();
			DataManager::addKey(info);
			if (info.answers.size() != ui.spinBox_ncasillas2->value())
			{
				ui.spinBox_ncasillas2->setValue(info.answers.size());
			}
		}
		
		// update anchor info in normal tiles
		DataManager::updateNormalTilesInfoFromKey();
		emit update();
	}
	else
	{
		if (DataManager::isKey())
			DataManager::removeKey();
		else DataManager::clearTile();
	}
	
}
void QSquareInfo::on_pushButton_Close_pressed()
{
	this->hide();

}