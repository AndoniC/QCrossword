#pragma once

#include <QFont>
#include <QToolBar>
#include <QWidget>
#include "ui_squareInfo.h"
#include "DataManager.h"

class QSquareInfo : public QWidget
{
	Q_OBJECT
	
public:
	QSquareInfo(  QWidget *parent = 0);
	~QSquareInfo();

	void fill(KeyTile *kt);
	int getIndex(QComboBox *c, QString text);


	Ui::SquareInfoWidget ui;
	
private:
	void enableDefs(bool f=false);
	void enableChar(bool f=false);
	
private slots:
	void on_groupBox_Defs_clicked();
	void on_groupBox_Char_clicked();
	void on_checkBox_Null_clicked();
	void on_pushButtonUpdate_pressed();
	void on_pushButton_Close_pressed();

signals:
	void update();


	
};

