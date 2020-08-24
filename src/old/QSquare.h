#pragma once

#include <QFont>

#include <QWidget>

enum GAME_STATE { PLAY = 0, EDITION = 1};

enum SQUARE_TYPE { DEFINITION_SQUARE = 0, CHARACTER_SQUARE = 1, NULL_SQUARE = 2, NEW_SQUARE = -1};
	typedef struct SQUARE_LINK
	{
		int x,y;
		DIRECTION dir;
		POSITION pos;
		SQUARE_LINK(){x=-1;y=-1;dir = NONE_DIR; pos = NONE;}
		SQUARE_LINK(int _y,int _x, DIRECTION _dir, POSITION _pos){x=_x;y=_y;dir = _dir; pos = _pos;}
	}SQUARE_LINK;
	typedef struct TSquareInfo
	{
		SQUARE_TYPE type;
		//-- DEF --//
		QString title1;
		QString title2;
		DIRECTION direction1;
		POSITION position1;
		DIRECTION direction2;
		POSITION position2;
		std::vector< SQUARE_LINK > childs;
		std::vector< SQUARE_LINK > parents;

		//-- CHAR --//
		//std::string szCharacter;
		QString char_edit;
		QString char_play;

	
		TSquareInfo(){type = NEW_SQUARE; direction1=NONE_DIR;position1=NONE;direction2=NONE_DIR;position2=NONE;};
		TSquareInfo(SQUARE_TYPE t,QString c,QString cp, DIRECTION dir, POSITION pos){type = t; direction1=dir;position1=pos;direction2=NONE_DIR;position2=NONE; char_edit=c;char_play=cp;};
		TSquareInfo(SQUARE_TYPE t,QString c, DIRECTION dir, POSITION pos){type = t; direction1=dir;position1=pos;direction2=NONE_DIR;position2=NONE; char_edit=c;};
		void clear(){type = NEW_SQUARE; direction1=NONE_DIR;position1=NONE;direction2=NONE_DIR;position2=NONE; };
		QString toStringDir1() {
			if (direction1==LEFT_DIR) return QString("LEFT");
			else if (direction1==RIGHT_DIR) return QString("RIGHT");
			else if (direction1==UP) return QString("UP");
			else if (direction1==DOWN) return QString("DOWN");
			else if (direction1==NONE_DIR) return QString("NONE");
		};
		QString toStringDir2() {
			if (direction2==LEFT_DIR) return QString("LEFT");
			else if (direction2==RIGHT_DIR) return QString("RIGHT");
			else if (direction2==UP) return QString("UP");
			else if (direction2==DOWN) return QString("DOWN");
			else if (direction2==NONE_DIR) return QString("NONE");
		};
		QString toStringPos1() {
			if (position1==LEFT) return QString("LEFT");
			else if (position1==RIGHT) return QString("RIGHT");
			else if (position1==TOP) return QString("TOP");
			else if (position1==BOTTOM) return QString("BOTTOM");
			else if (position1==NONE) return QString("NONE");
		};
		QString toStringPos2() {
			if (position2==LEFT) return QString("LEFT");
			else if (position2==RIGHT) return QString("RIGHT");
			else if (position2==TOP) return QString("TOP");
			else if (position2==BOTTOM) return QString("BOTTOM");
			else if (position2==NONE) return QString("NONE");
		};
		QString toStringDir()
		{
			int vdir[4]={0,0,0,0};
			for (int i=0;i<parents.size();i++)
			{
				if (parents[i].dir == LEFT_DIR) vdir[0]=1;
				if (parents[i].dir == RIGHT_DIR) vdir[1]=1;
				if (parents[i].dir == UP) vdir[2]=1;
				if (parents[i].dir == DOWN) vdir[3]=1;
			}
			QString dir;
			bool guion=false;
			if (vdir[0]) {dir +="LEFT"; guion=true;}
			
			if (vdir[1]) {if (guion) {dir+="_"; guion=false;}dir +="RIGHT";guion=true;}
			
			if (vdir[2]) {if (guion) {dir+="_"; guion=false;}dir +="UP";guion=true;}
			
			if (vdir[3]) {if (guion) {dir+="_"; guion=false;}dir +="DOWN";}

			return dir;
		}
		QString toStringType() {
			if (type==SQUARE_TYPE::DEFINITION_SQUARE) return QString("DEFINITION_SQUARE");
			else if (type==SQUARE_TYPE::CHARACTER_SQUARE) return QString("CHARACTER_SQUARE");
			else if (type==SQUARE_TYPE::NULL_SQUARE) return QString("NULL_SQUARE");
			else if (type==SQUARE_TYPE::NEW_SQUARE) return QString("NEW_SQUARE");
			else return QString("SQUARE_DESCONOCIDO");
		};
		void printSquare()
		{
			printf("   Square \n");
			QByteArray ba= toStringType().toLocal8Bit();
			const char *t = ba.data();
			printf("		type : %s \n",t);
			ba= title1.toLocal8Bit();
			const char *d1 = ba.data();
			printf("		def 1: %s \n",d1);
			ba= title2.toLocal8Bit();
			const char *d2 = ba.data();
			printf("		def 2: %s \n",d2);
			ba= toStringDir1().toLocal8Bit();
			const char *dir1 = ba.data();
			printf("		dir 1: %s ",dir1);
			ba= toStringPos1().toLocal8Bit();
			const char *pos1 = ba.data();
			printf(" pops 1: %s\n ",pos1);
			ba= toStringDir2().toLocal8Bit();
			const char *dir2 = ba.data();
			printf("		dir 2: %s ",dir2);
			ba= toStringPos2().toLocal8Bit();
			const char *pos2 = ba.data();
			printf(" pops 1: %s\n ",pos2);
		}
		void borrar()
		{	type = NEW_SQUARE; 	direction1=NONE_DIR;position1=NONE;direction2=NONE_DIR;position2=NONE; childs.clear();parents.clear();	}
	}TSquareInfo; 

DIRECTION resolveDirection(QString dir);

POSITION resolvePosition(QString pos);



class QSquare : public QWidget
{
	Q_OBJECT
	
public:
	QSquare( int size,int row,int col, TSquareInfo info=TSquareInfo(), QWidget *parent = 0);
	QSquare( int pos , int size,int row,int col, TSquareInfo info=TSquareInfo(), QWidget *parent = 0);
	~QSquare();

	


	void create(int pos, int size,int row,int col, TSquareInfo &info);
	void modify(TSquareInfo &info);
	void copy(TSquareInfo &info);
	void setBackground(std::string path);
	void setPosAndDir(DIRECTION d1,POSITION p1,DIRECTION d2, POSITION p2);
	void selected(int sel);
	void rescale(int size);
	void updateBackground();

	TSquareInfo m_info;
	
	GAME_STATE m_state;
	int m_selected; 
	QFont sansFont;
	QFont titleFont;



	int place;
	int m_size;
	QPixmap m_background;

	int m_row,m_col;

	//-- events --//
	virtual void paintEvent(QPaintEvent *event);
	void mousePressEvent ( QMouseEvent * event );
	void mouseMoveEvent ( QMouseEvent * event );

signals:
	void press(int row,int col,TSquareInfo &,int where_clicked);
	void over(int row,int col,TSquareInfo &);
	
};

