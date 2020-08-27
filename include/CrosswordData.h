#pragma once


#include <QWidget>
#include <QGridLayout>
#include <QtGui>
#include <iostream>
#include <QFileDialog>
#include "nlohmann/json.hpp"
#include "ext/ext_ocv.h"
#include "ext/ext.h"
#include "loguru-2.0.0/operativeLog.h"

CUSTOM_MAKE_ENUM(GAME_DIRECTION, VERTICAL, HORIZONTAL)
CUSTOM_MAKE_ENUM(DIRECTION, LEFT, RIGHT, UP, DOWN, NONE)
CUSTOM_MAKE_ENUM(START_POSITION, LEFT, RIGHT, TOP, BOTTOM, NOTILE)

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

	struct anchor_points_t
	{
		std::string text;
		cv::Point horizontal_key;
		cv::Point vertical_key;
		cv::Point first_horizontal_tile;
		cv::Point first_vertical_tile;
		cv::Point last_horizontal_tile;
		cv::Point last_vertical_tile;
		
		DIRECTION::itype arrow_at_left;
		DIRECTION::itype arrow_at_right;
		DIRECTION::itype arrow_at_top;
		DIRECTION::itype arrow_at_bottom;

		int isKey;
		bool isValid;
		anchor_points_t() { isKey = 0; isValid = false; arrow_at_left = arrow_at_right = arrow_at_top = arrow_at_bottom = DIRECTION::NONE; }
		void printout()
		{
			std::cout << " --------------------" << std::endl;
			std::cout << "    Text: " << text << std::endl;
			std::cout << "    horizontal_key: " << horizontal_key.x << " , " << horizontal_key.y << std::endl;
			std::cout << "    vertical_key: " << vertical_key.x << " , " << vertical_key.y << std::endl;
			std::cout << "    First_horizontal_tile: " << first_horizontal_tile.x << " , " << first_horizontal_tile.y << std::endl;
			std::cout << "    First_vertical_tile: " << first_vertical_tile.x << " , " << first_vertical_tile.y << std::endl;
			std::cout << "    last_horizontal_tile: " << last_horizontal_tile.x << " , " << last_horizontal_tile.y << std::endl;
			std::cout << "    last_vertical_tile: " << last_vertical_tile.x << " , " << last_vertical_tile.y << std::endl;
			std::cout << "    IsKey: " << isKey << std::endl;
			std::cout << "    IsValid: " << isValid << std::endl;
			std::cout << " --------------------" << std::endl;
		}
	};

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


	void fillWithContent(std::vector<std::vector<anchor_points_t> > &lcontent);



	/*!
	* \brief getKeySquare
	* Get key square info at icol irow position
	*/
	nlohmann::json getKeySquare(int icol, int irow);


protected:

	nlohmann::json m_data;

	
	
	
};

