#pragma once


#include <QWidget>
#include <QGridLayout>
#include <QtGui>
#include <iostream>
#include <QFileDialog>
#include "nlohmann/json.hpp"
class CrossWordData 
{
public:
	static bool contains(nlohmann::json &j, const std::string &node)
	{
		if (j.contains<std::string>(node.c_str())) return true;
		else return false;
	}

	static bool contains(nlohmann::json &j, const std::string &parent, const std::string &child)
	{
		if (j.contains<std::string>(parent.c_str()) &&
			j[parent].contains<std::string>(child.c_str())) return true;
		else return false;
	}
	static bool contains(nlohmann::json &j, const std::string &parent, const std::string &child, const std::string &subchild)
	{
		if (j.contains<std::string>(parent.c_str()) &&
			j[parent].contains<std::string>(child.c_str()) &&
			j[parent][child].contains<std::string>(subchild.c_str())) return true;
		else return false;
	}

	static bool contains(nlohmann::json &j, const std::string &parent, const std::string &child, const std::string &subchild, const std::string &subsubchild)
	{
		if (j.contains<std::string>(parent.c_str()) &&
			j[parent].contains<std::string>(child.c_str()) &&
			j[parent][child].contains<std::string>(subchild.c_str()) &&
			j[parent][child][subchild].contains<std::string>(subsubchild.c_str())) return true;
		else return false;
	}


public:

	

	struct crossword_desc_t
	{
		std::string title;
		int cols;
		int rows;
		std::string topic;
		int unit;

		crossword_desc_t() { cols = 0; rows = 0; unit = -1; }
	};

	struct crossword_key_t
	{
		std::string first_point;
		std::string direction;
		std::string def;
		std::string topic;
		std::string short_def;
		std::vector< std::string > clues;
		std::vector<std::string> answers;

		crossword_key_t() { }
	};

	CrossWordData();
	~CrossWordData();
	
	void loadCrosswordData(std::string _json_file_name, int format=0);
	void saveCrosswordData(std::string _json_file_name, int format = 0);


	void createCrosswordData(crossword_desc_t &desc);
	void addKey(int row, int col, crossword_key_t &desc);
	void removeKey(int row, int col);
	crossword_desc_t getDescription();

	/*!
	* \brief getKeySquare
	* Get key square info at icol irow position
	*/
	nlohmann::json getKeySquare(int icol, int irow);


protected:

	nlohmann::json m_data;

	
	
	
};

