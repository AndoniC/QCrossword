#pragma once


#include <QWidget>
#include <QGridLayout>
#include <QtGui>
#include <iostream>
#include <QFileDialog>
class QWidgetCrossword : public QWidget
{
	Q_OBJECT

public:
	QWidgetCrossword( QWidget *parent = 0);
	~QWidgetCrossword();
	
	void createGrid(int rows,int cols,int size);
	void create(int rows,int cols, int size);
	void fillInfo(int row, int col, TSquareInfo &info);
	void modifyInfo(int row, int col, TSquareInfo &info);
	int rows();
	int cols();
	QSquare *getSquare(int row,int col);
	TSquareInfo& getInfo(int rows,int cols);
	//void resizegrid(int rows,int cols,int size);
	void resizeGridSquares(int size);
	void write(QString file_name);
	void load(QString file_name);
	void readSquareElement(QXmlStreamReader &r);
	void desvincular(int row, int col);
	void vincular(int row, int col);
	int compare();
	void updateBackground(int row, int col);
	int numberCharacters();
	void clear();
	void clearCharacters(GAME_STATE mode=GAME_STATE::PLAY);
	void changeState(GAME_STATE st);

	void deleteView();
	void deleteData();
	

	//std::vector < std::vector <TSquareInfo> > m_grid_info;

	QGridLayout *m_grid;
	int m_rows;
	int m_cols;
	
	
signals:
	void newCrossWord(int rows, int cols);
	
};

