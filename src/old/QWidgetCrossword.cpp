
#include "QWidgetCrossword.h"


QWidgetCrossword::QWidgetCrossword( QWidget *parent)
	: QWidget(parent)
{
	//ui.setupUi(this);
	
	m_grid=NULL;
}

QWidgetCrossword::~QWidgetCrossword()
{
	//if (m_grid!=NULL)
	//{
	//	for (int i=0;i<m_grid->rowCount();i++)
	//	{
	//		for (int j=0;j<m_grid->columnCount();j++)
	//		{
	//			QLayoutItem* item = m_grid->itemAtPosition(i, j);
	//			m_grid->removeItem(item);
	//		}
	//	}
	//	delete m_grid;
	//}
	//for (int i=0;i<m_grid_info.size();i++) 
	//{   
	//	for (int j=0;j<m_grid_info[i].size();j++)
	//	{
	//		m_grid_info[i][j].childs.clear();
	//		m_grid_info[i][j].parents.clear();
	//	}
	//	m_grid_info[i].clear();
	//}
	//	m_grid_info.clear();
	
}
void QWidgetCrossword::deleteView()
{
	if (m_grid!=NULL)
	{
		for (int i=0;i<m_grid->rowCount();i++)
		{
			for (int j=0;j<m_grid->columnCount();j++)
			{
				QLayoutItem* item = m_grid->itemAtPosition(i, j);
				 m_grid->removeWidget(item->widget());
				//delete (QSquare*)(item->widget());
				 //m_grid->removeItem(item);
			}
		}
		delete layout();
//		delete m_grid;
		m_grid=NULL;
	}
}
void QWidgetCrossword::deleteData()
{
	QSquare *square;
	for (int i=0;i<m_rows;i++) 
	{   
		for (int j=0;j<m_cols;j++)
		{
			square= getSquare(i,j);
			square->m_info.childs.clear();
			square->m_info.parents.clear();
		}
	}

}
void QWidgetCrossword::createGrid(int rows,int cols,int size)
{
	//-- remove Grid--//
	//if (m_grid!=NULL)
	//{ 
		//delete layout();
		//int nrows,ncols;
		//nrows = m_grid->rowCount();
		//ncols = m_grid->columnCount();
		//
		//for (int i=0;i<nrows;i++)
		//{
		//	for (int j=0;j<ncols;j++)
		//	{
		//		QLayoutItem* item = m_grid->itemAtPosition(i, j);
		//		if (item) m_grid->removeItem(item);
		//	}
		//}
//		delete m_grid;
	//}
	//else 
	if (m_grid==NULL)
	{
			m_grid = new QGridLayout(this);
			//this->setLayout(m_grid);
	}	
	
	printf ("   Creating new Grid %dx%d \n      *Square_size: %d\n", rows,cols,size);
	
	m_grid->setContentsMargins(0,0,0,0);
	//m_grid.setHorizontalSpacing(0);
	//m_grid.setVerticalSpacing(0);
	//m_grid.setMargin(0);
	
	m_grid->setSpacing(0);
	m_rows = rows;
	m_cols = cols;


	for (int i=0;i<rows;i++)
	{
		for (int j=0;j<cols;j++)
		{
			
			if (i==rows-1 && j == cols-1)  m_grid->addWidget(new QSquare(3,size,i,j),i,j);
			else if (j == cols-1)  m_grid->addWidget(new QSquare(1,size,i,j),i,j);
			else if (i==rows-1) m_grid->addWidget(new QSquare(2,size,i,j),i,j);
			else m_grid->addWidget(new QSquare(size,i,j),i,j);
		
		}
	}
	
	
}
void QWidgetCrossword::resizeGridSquares(int size)
{
	for (int i=0;i<m_grid->rowCount();i++)
	{
		for (int j=0;j<m_grid->columnCount();j++)
		{
			QLayoutItem* item = m_grid->itemAtPosition(i, j);
			((QSquare*)(item->widget()))->rescale(size);
		}
	}
}

void QWidgetCrossword::create(int rows,int cols, int size)
{
	//-- Datos --//
	//m_grid_info.resize(rows);
	//for (int i=0;i<rows;i++)
	//	m_grid_info[i].resize(cols);
	
	//-- Vista --//
	createGrid(rows,cols,size);
	this->setLayout(m_grid);
	
}
void QWidgetCrossword::fillInfo(int row, int col, TSquareInfo &info)
{
	QSquare * square = getSquare(row,col);
	square->m_info.type=info.type;
	square->m_info.char_edit=info.char_edit;
	square->m_info.char_play=info.char_play;
	square->m_info.direction1=info.direction1;
	square->m_info.direction2=info.direction2;
	square->m_info.position1=info.position1;
	square->m_info.position2=info.position2;
}
void QWidgetCrossword::modifyInfo(int row, int col, TSquareInfo &info)
{
	QSquare * square = getSquare(row,col);
	square->m_info=info;
	updateBackground(row,col);
}
QSquare* QWidgetCrossword::getSquare(int row,int col)
{
	if (m_grid==NULL) return NULL;
	int w = m_grid->columnCount()-1;
	int h = m_grid->rowCount()-1;

	if (row <0 || row > h) return NULL;
	if (col <0 || col > w) return NULL;

	QLayoutItem* item = m_grid->itemAtPosition(row, col);
	return ((QSquare*)(item->widget()));
}
int QWidgetCrossword::rows()
{
	return m_rows;
}
int QWidgetCrossword::cols()
{
	return m_cols;
}

void QWidgetCrossword::desvincular(int row, int col)
{
	QSquare *square = getSquare(row,col);
	if (square->m_info.type==DEFINITION_SQUARE)
	{
		//-- Desvincular DEFINITION_SQUARE de CHARACTER_SQUARES en m_grid_info --//
		int ind_child_row, ind_child_col,ind_parent_row,ind_parent_col;
		DIRECTION dir,dir_aux;
		POSITION pos,pos_aux;
		
		for(int i =0;i<square->m_info.childs.size();i++)
		{
			ind_child_row = square->m_info.childs[i].y;
			ind_child_col = square->m_info.childs[i].x;
			dir =  square->m_info.childs[i].dir;
			pos =  square->m_info.childs[i].pos;
			
			QSquare *square_child = getSquare(ind_child_row,ind_child_col);
			for (int j=0;j<square_child->m_info.parents.size();)
			{
				pos_aux = square_child->m_info.parents[j].pos;
				ind_parent_row = square_child->m_info.parents[j].y;
				ind_parent_col = square_child->m_info.parents[j].x;

				if (pos == pos_aux	&& row == ind_parent_row && col == ind_parent_col)
				{
					square_child->m_info.parents.erase(square_child->m_info.parents.begin()+j);
					break;
				}
				else j++;
			}
			updateBackground(ind_parent_row, ind_parent_col);

		}

	
	}
	else if (square->m_info.type==CHARACTER_SQUARE)
	{
		//-- Desvincular CHARACTER_SQUARE de DEFINITION_SQUARES --//
		int ind_parent_row, ind_parent_col;
		DIRECTION dir;
		POSITION pos;
		
		for(int i =0;i<square->m_info.parents.size();i++)
		{
			ind_parent_row = square->m_info.parents[i].y;
			ind_parent_col = square->m_info.parents[i].x;
			dir =  square->m_info.parents[i].dir;
			pos =  square->m_info.parents[i].pos;

			QSquare *square_parent = getSquare(ind_parent_row,ind_parent_col);
			for (int j=0;j<square_parent->m_info.childs.size();j++)
			{
				if (pos == square_parent->m_info.childs[j].pos
					&& row == square_parent->m_info.childs[j].y &&
					 col == square_parent->m_info.childs[j].x)
				{
					square_parent->m_info.parents.erase(square_parent->m_info.parents.begin()+j);
					break;
				}
			}
		}
		updateBackground(ind_parent_row, ind_parent_col);

	}

	square->m_info.childs.clear();
	square->m_info.parents.clear();
}

void QWidgetCrossword::updateBackground(int row, int col)
{
	QSquare * s =  getSquare(row,col);
	if (s!=NULL) s->updateBackground();
}

void QWidgetCrossword::vincular(int row, int col)
{
	
	QSquare *square = getSquare(row,col);
	//square->m_info.printSquare();
	if (square->m_info.type==DEFINITION_SQUARE)
	{ //-- Vincular DEFINITION_SQUARE con CHARACTER_SQUARES --//
		
		//-- en las casillas de definición la posición indica en que lugar estará la primera letra de la palabra con respecto a la casilla de definicion--//
		//-- y la dirección hacia donde avanzará la palabra --//
		//-- Creación de nueva casilla de definición --//
	
		int ind_child_x, ind_child_y;
		

		if (!square->m_info.title1.isEmpty())
		{
			if (square->m_info.position1==LEFT)
			{
				ind_child_y = row;
				ind_child_x = col-1;
			}
			if (square->m_info.position1==RIGHT)
			{
				ind_child_y = row;
				ind_child_x = col+1;
			}
			if (square->m_info.position1==TOP)
			{
				ind_child_y =row-1;
				ind_child_x = col;
			}
			if (square->m_info.position1==BOTTOM)
			{
				ind_child_y =row+1;
				ind_child_x = col;
			}

			if (ind_child_x >=0 || ind_child_y>=0)
			{
				QSquare *square_child = getSquare(ind_child_y,ind_child_x);
				//--Añadir el padre a la lista de padres del hijo --//
				square_child->m_info.parents.push_back(SQUARE_LINK(row,col,square->m_info.direction1,square->m_info.position1));
		
				//--Añadir el hijo a la lista de hijos del padre --//ç
				square->m_info.childs.push_back(SQUARE_LINK(ind_child_y,ind_child_x,square->m_info.direction1,square->m_info.position1));

				//-- Actualizar hijo --//
				square_child->m_info.type =CHARACTER_SQUARE;
			
				updateBackground(ind_child_y,ind_child_x);
			}
						
		}



		if (!square->m_info.title2.isEmpty())
		{
			if (square->m_info.position2==LEFT)
			{
				ind_child_y = row;
				ind_child_x = col-1;
			}
			if (square->m_info.position2==RIGHT)
			{
				ind_child_y = row;
				ind_child_x = col+1;
			}
			if (square->m_info.position2==TOP)
			{
				ind_child_y = row-1;
				ind_child_x = col;
			}
			if (square->m_info.position2==BOTTOM)
			{
				ind_child_y = row+1;
				ind_child_x = col;
			}
			QSquare *square_child = getSquare(ind_child_y,ind_child_x);

			if (square_child->m_info.type == DEFINITION_SQUARE)
			{ 
				//-- en esa dirección nos encontramos con un DEFINICION_SQUARE --//
				//-- lo eliminamos --//
				desvincular(ind_child_y,ind_child_x);
				square_child->m_info.borrar();
			}

			//--Añadir el padre a la lista de padres del hijo --//
			square_child->m_info.parents.push_back(SQUARE_LINK(row,col,square->m_info.direction2,square->m_info.position2));
			//--Añadir el hijo a la lista de hijos del padre --//ç
			square->m_info.childs.push_back(SQUARE_LINK(ind_child_y,ind_child_x,square->m_info.direction2,square->m_info.position2));

			//-- Actualizar hijo --//
			
			square_child->m_info.type =CHARACTER_SQUARE;

			updateBackground(ind_child_y,ind_child_x);
		}

	}
	else if (square->m_info.type==CHARACTER_SQUARE)
	{
		//-- Vincular CHARACTER_SQUARE con DEFINITION_SQUARES --//
		//-- En principio no se puede vincular un CHARACTER_SQUARE con un DEFINITION_SQUARE --//
	}
}

void QWidgetCrossword::changeState(GAME_STATE st)
{
	QSquare *square;

	for (int i=0;i<m_rows;i++)
	{
		for (int j=0;j<m_cols;j++)
		{
			QSquare * s =  getSquare(i,j);
			if (s!=NULL) s->m_state=st;
		}
	}
	update();
}

int QWidgetCrossword::compare()
{
	int difs=0;
		
	QSquare *square;
	for (int i=0;i<m_rows;i++)
	{
		
		for (int j=0;j<m_cols;j++)
		{
			square = getSquare(i,j);
			if (square->m_info.type == CHARACTER_SQUARE)
			{
				if (square->m_info.char_play.compare(square->m_info.char_edit)!=0)
					difs++;

			}
		}
	}
	return difs;

}

int QWidgetCrossword::numberCharacters()
{
	QSquare *square;
	//int nrows,ncols;
	//nrows = m_grid_info.size();
	int cont = 0;
	for (int i=0;i<m_rows;i++)
	{
		//ncols = m_grid_info[i].size();
		for (int j=0;j<m_cols;j++)
		{
			square = getSquare(i,j);
			if (square->m_info.type == CHARACTER_SQUARE)
			{
				cont ++;
			}
		}
	}
	return cont;
}

void QWidgetCrossword::clear()
{
	QSquare *square;
	//int nrows,ncols;
	
	for (int i=0;i<m_rows;i++)
	{
		//ncols = m_grid_info[i].size();
		for (int j=0;j<m_cols;j++)
		{
			square = getSquare(i,j);
			square->m_info.clear();
			square->m_info.childs.clear();
			square->m_info.parents.clear();
			updateBackground(i,j);
		}
	}
	update();
	
}
void QWidgetCrossword::clearCharacters(GAME_STATE mode)
{
	QSquare *square;
	for (int i=0;i<m_rows;i++)
	{
		for (int j=0;j<m_cols;j++)
		{
			square = getSquare(i,j);
			if (square->m_info.type == CHARACTER_SQUARE)
			{
				square->m_info.char_play="";
				
				if (mode==GAME_STATE::EDITION) 
				{
					square->m_info.char_play="";
					
				}
			}
		}
	}
	update();
}

void QWidgetCrossword::write(QString file_name)
{
	QString filename;
	if (file_name.isEmpty())
	{
		filename = QFileDialog::getSaveFileName(this,
		                               tr("Save Xml"), ".",
										tr("Xml files (*.xml)"));
	}
	else filename = file_name;

	QByteArray bafn = file_name.toLocal8Bit();
	const char *sz = bafn.data();
	printf ("Writing crossword %s\n",sz);
	
	QFile file(filename);
	file.open(QIODevice::WriteOnly);

	QXmlStreamWriter xmlWriter(&file);
	xmlWriter.setAutoFormatting(true);
	xmlWriter.writeStartDocument();
	xmlWriter.writeStartElement("Crossword");
	xmlWriter.writeAttribute("rows", QString().setNum(m_grid->rowCount()));
	xmlWriter.writeAttribute("cols", QString().setNum(m_grid->columnCount()));
	

	for (int i=0;i<m_grid->rowCount();i++)
	{
		for (int j=0;j<m_grid->columnCount();j++)
		{
			QSquare *square = getSquare(i,j);
		  xmlWriter.writeStartElement("Square");
		  xmlWriter.writeAttribute("row", QString().setNum(i));
		  xmlWriter.writeAttribute("col", QString().setNum(j));
		  xmlWriter.writeAttribute("type", square->m_info.toStringType());


		  if (square->m_info.type==CHARACTER_SQUARE)
		  {
				xmlWriter.writeAttribute("text", square->m_info.char_edit);
				xmlWriter.writeAttribute("text_play", square->m_info.char_play);
				//xmlWriter.writeAttribute("is_inicial_char", m_grid_info[i][j].is_initial?"true":"false");
				//xmlWriter.writeAttribute("direction", m_grid_info[i][j].toStringDir1().c_str());
				//xmlWriter.writeAttribute("position", m_grid_info[i][j].toStringPos1().c_str());

		  }
		  else if (square->m_info.type==DEFINITION_SQUARE)
		  {
			  xmlWriter.writeStartElement("Def1");
			  xmlWriter.writeAttribute("text", square->m_info.title1 );
			  xmlWriter.writeAttribute("direction", square->m_info.toStringDir1());
			  xmlWriter.writeAttribute("position", square->m_info.toStringPos1());
			  xmlWriter.writeEndElement();
			  xmlWriter.writeStartElement("Def2");
			  xmlWriter.writeAttribute("text",square->m_info.title2);
			  xmlWriter.writeAttribute("direction", square->m_info.toStringDir2());
			  xmlWriter.writeAttribute("position", square->m_info.toStringPos2());
			  xmlWriter.writeEndElement();
		  }

		  xmlWriter.writeEndElement();
	}
  }
  xmlWriter.writeEndElement();
  xmlWriter.writeEndDocument();

  file.close();

}

void QWidgetCrossword::load(QString file_name)
{
	QString filename;
	if (file_name.isEmpty())
	{
		filename = QFileDialog::getOpenFileName(this,
	                               tr("Open Xml"), ".",
	                               tr("Xml files (*.xml)"));
	}
	else filename = file_name;

	QByteArray ba = filename.toLocal8Bit();
	const char *sz = ba.data();
	printf("   Name: %s\n",sz);

	QFile file(filename);
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
	    std::cout << "   Error: Cannot read file " << qPrintable(filename)
	     << ": " << qPrintable(file.errorString())
	     << std::endl;
	}

	QXmlStreamReader Rxml;
	Rxml.setDevice(&file);
	
	while(!Rxml.atEnd())
	{
		 /* Read next element.*/
        QXmlStreamReader::TokenType token = Rxml.readNext();
        /* If token is just StartDocument, we'll go to next.*/
     
		if(token == QXmlStreamReader::StartDocument) {
            continue;
        }
		 /* If token is StartElement, we'll see if we can read it.*/
        if(token == QXmlStreamReader::StartElement) {
          
			QString t = Rxml.name().toString();
			t=t.toUpper();
            if(t == "CROSSWORD") {
				//-- leer filas y columnas --//
				/* Let's get the attributes for person */
				QXmlStreamAttributes attributes = Rxml.attributes();
				/* Let's check that person has id attribute. */
				
				if(attributes.hasAttribute("rows")) {
					 /* We'll add it to the map. */
					m_rows = attributes.value("rows").toString().toInt();
				}
				if(attributes.hasAttribute("cols")) {
					 /* We'll add it to the map. */
					m_cols = attributes.value("cols").toString().toInt();
				}
				
			//	if (m_rows !=0 && m_cols!=0) sltNew(m_rows,m_cols);

				if (m_rows !=0 && m_cols!=0) 
				{
					createGrid(m_rows,m_cols,10);
					/*	m_grid_info.resize(m_rows);
						for (int i=0;i< m_rows;i++)
							m_grid_info[i].resize(m_cols);*/
				}

                continue;
            }
            /* If it's named person, we'll dig the information from there.*/
            if(t == "SQUARE") {
                readSquareElement(Rxml);

            }
        }
	
		
   }


   file.close();

   printf("   Dims (rows x cols): %d x %d\n",m_rows,m_cols);

  if (Rxml.hasError())
  {
          std::cerr << "   Error: Failed to parse file "
         << qPrintable(filename) << ": "
         << qPrintable(Rxml.errorString()) << std::endl;
  }
  else if (file.error() != QFile::NoError)
  {
      std::cerr << "   Error: Cannot read file " << qPrintable(filename)
               << ": " << qPrintable(file.errorString())
               << std::endl;
  }

}
void QWidgetCrossword::readSquareElement(QXmlStreamReader &r)
{
	 /* Let's check that we're really getting a person. */
    if(r.tokenType() != QXmlStreamReader::StartElement &&
            r.name().toString().toUpper() == "SQUARE") 
	{
		return;
    }
	int x,y;

		/* Let's get the attributes for person */
		QXmlStreamAttributes attributes = r.attributes();
		/* Let's check that person has id attribute. */
		QString type;
		if(attributes.hasAttribute("type")) {
			 /* We'll add it to the map. */
			type = attributes.value("type").toString();
		}
		if(attributes.hasAttribute("row")) {
			 /* We'll add it to the map. */
			y = attributes.value("row").toString().toInt();
		}
		if(attributes.hasAttribute("col")) {
			 /* We'll add it to the map. */
			x = attributes.value("col").toString().toInt();
		}
		
		QSquare *square = getSquare(y,x);

		if (type=="DEFINITION_SQUARE")
		{
			square->m_info.type=DEFINITION_SQUARE;
			QXmlStreamReader::TokenType token= r.readNext();
			/*
			 * We're going to loop over the things because the order might change.
			 * We'll continue the loop until we hit an EndElement named person.
			 */
			while(!(r.tokenType() == QXmlStreamReader::EndElement &&
					r.name().toString().toUpper() == "SQUARE")) 
			{
			
				
				if(token == QXmlStreamReader::StartElement) 
				{
					
					QString nodeName = r.name().toString();
					if(nodeName.toUpper() == "DEF1") 
							{
							/* Let's get the attributes for person */
							QXmlStreamAttributes def_attributes = r.attributes();
							/* Let's check that person has id attribute. */
							QString text,direction,position;
							if(def_attributes.hasAttribute("text")) {
								 /* We'll add it to the map. */
								text = def_attributes.value("text").toString();
							}
							if(def_attributes.hasAttribute("direction")) {
								 /* We'll add it to the map. */
								direction = def_attributes.value("direction").toString();
							}
							if(def_attributes.hasAttribute("position")) {
								 /* We'll add it to the map. */
								position = def_attributes.value("position").toString();
							}
							QSquare *square = getSquare(y,x);
							square->m_info.direction1 = resolveDirection(direction);
							square->m_info.position1 = resolvePosition(position);
							square->m_info.title1 = text;
				
							
					}
					
					if(nodeName.toUpper() == "DEF2") 
					{
							QXmlStreamAttributes def_attributes = r.attributes();
							/* Let's check that person has id attribute. */
							QString text,direction,position;
							if(def_attributes.hasAttribute("text")) {
								 /* We'll add it to the map. */
								text = def_attributes.value("text").toString();
							}
							if(def_attributes.hasAttribute("direction")) {
								 /* We'll add it to the map. */
								direction = def_attributes.value("direction").toString();
							}
							if(def_attributes.hasAttribute("position")) {
								 /* We'll add it to the map. */
								position =  def_attributes.value("position").toString();
							}
							
							square->m_info.direction2 = resolveDirection(direction);
							square->m_info.position2 = resolvePosition(position);
							square->m_info.title2 = text;

							

					}
				}
				
				token= r.readNext();
			}
			vincular(y,x);
			updateBackground(y,x);
		}
		if (type=="CHARACTER_SQUARE")
		{
				//if(attributes.hasAttribute("is_inicial_char")) 
				//{
					/* We'll add it to the map. */
					//type = attributes.value("is_inicial_char").toString();
				//}
			square->m_info.type=CHARACTER_SQUARE;
			if(attributes.hasAttribute("text"))
			{
				QString text = attributes.value("text").toString();
				square->m_info.char_edit=text;
				
			}
			if(attributes.hasAttribute("text_play"))
			{
				QString text = attributes.value("text_play").toString();
				square->m_info.char_play=text;
			}

		}
		if (type=="NULL_SQUARE")
		{
		}
		if (type=="NEW_SQUARE")
		{
		}




}
TSquareInfo& QWidgetCrossword::getInfo(int rows,int cols)
{
	if (rows>=0 && rows < m_rows && cols >=0 && cols < m_cols)
	{
		return getSquare(rows,cols)->m_info;
	}
	else return TSquareInfo();
}