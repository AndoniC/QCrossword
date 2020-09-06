#pragma once

#include <QFont>

#include <QWidget>
#include <QDialog>

#include "ui_rows_cols_widget.h"
#include "ext/ext_string.h"


class QRowsColsWidget : public QDialog
{
	Q_OBJECT
	
public:
	QRowsColsWidget(QWidget *parent = 0);
	~QRowsColsWidget();


	Ui::rowsColsWidget ui;
	
private:
	
private slots:
	void on_pushButton_Ok_clicked();
	void on_pushButton_Cancel_clicked();

signals:
	void update(int rows, int cols, std::wstring title, std::wstring topic, std::string units);


	
};

