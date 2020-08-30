#include <QtGui>
#include "QRowsColsWidget.h"


QRowsColsWidget::QRowsColsWidget( QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi( this );
	ui.spinBoxRows->setValue(17);
	ui.spinBoxCols->setValue(12);
}
QRowsColsWidget::~QRowsColsWidget()
{
}

void QRowsColsWidget::on_pushButton_Ok_clicked()
{
	emit update(ui.spinBoxRows->value(), ui.spinBoxCols->value(),ui.title->text().toLocal8Bit().data(),ui.topic->text().toLocal8Bit().data(), ui.unit->currentText().toLocal8Bit().data());
	this->close();
}
void QRowsColsWidget::on_pushButton_Cancel_clicked()
{
	this->close();
}