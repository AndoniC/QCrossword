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
CUSTOM_MAKE_ENUM(UNITS, LETTER, SYLLABLE)
CUSTOM_MAKE_ENUM(TILETYPE, KEY, NORMAL)

class Tile
{
public:

	Tile() { isValid = false; type = TILETYPE::NORMAL; };

	void setValid(bool f) { isValid = f; }
	void setType(TILETYPE::itype t) { type = t; }
	void setText(std::wstring &t) { text = t; }
	virtual void reset() {}
	virtual void reset(cv::Point) {}
	virtual void resetInDirection(cv::Point p, DIRECTION::itype dir) {};


	virtual void printout()
	{
		std::wcout << " --------------------" << std::endl;
		std::wcout << "    Text: " << text << std::endl;
		std::wcout << "    Type: " << TILETYPE::to_wstring(type) << std::endl;
		std::wcout << "    IsValid: " << isValid << std::endl;
		std::wcout << " --------------------" << std::endl;
	}
public:
	std::wstring text;
	TILETYPE::itype type;
	bool isValid;
};
class KeyTile : public Tile
{
public:
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

	KeyTile(crossword_key_t &desc)
	{
		defs.push_back(desc);
	}
	void push_back(crossword_key_t &desc)
	{
		defs.push_back(desc);
	}

public:
	std::vector <crossword_key_t> defs;
	

};

class NormalTile : public Tile
{
public:
	NormalTile()
	{
		arrow_at_left = arrow_at_right = arrow_at_top = arrow_at_bottom = DIRECTION::NONE;
		break_at_left = break_at_right = break_at_top = break_at_bottom = DIRECTION::NONE;
		horizontal_key = cv::Point(-1, -1);
		vertical_key = cv::Point(-1, -1);
		surround_square = false;
		circle_number = 0;
	}

	void reset() {
		text.clear(); type = TILETYPE::NORMAL; isValid = false; 
		arrow_at_left = arrow_at_right = arrow_at_top = arrow_at_bottom = DIRECTION::NONE;
		break_at_left = break_at_right = break_at_top = break_at_bottom = DIRECTION::NONE;
		horizontal_key = cv::Point(-1, -1);
		vertical_key = cv::Point(-1, -1);
		surround_square = false;
		circle_number = 0;
	}

	void reset(cv::Point p) {
		text.clear();  type = TILETYPE::NORMAL; isValid = false; arrow_at_left = arrow_at_right = arrow_at_top = arrow_at_bottom = DIRECTION::NONE;
		horizontal_key = vertical_key = first_horizontal_tile = first_vertical_tile = last_horizontal_tile = last_vertical_tile = p;
		horizontal_key = cv::Point(-1, -1);
		vertical_key = cv::Point(-1, -1);

	}

	void resetInDirection(cv::Point p, DIRECTION::itype dir) {
		if (dir == DIRECTION::LEFT || dir == DIRECTION::RIGHT)
		{
			// we are erasing key's horizontal content
			if (vertical_key != cv::Point(-1, -1))
			{
				//There is a vertical key
				horizontal_key = cv::Point(-1, -1);
				first_horizontal_tile = last_horizontal_tile = p;

			}
			else
			{
				text.clear(); type = TILETYPE::NORMAL; isValid = false; arrow_at_left = arrow_at_right = arrow_at_top = arrow_at_bottom = DIRECTION::NONE;
				horizontal_key = vertical_key = first_horizontal_tile = first_vertical_tile = last_horizontal_tile = last_vertical_tile = p;
				horizontal_key = vertical_key = cv::Point(-1, -1);
			}
		}
		else
		{
			// we are erasing key's horizontal content
			if (horizontal_key != cv::Point(-1, -1))
			{
				//There is a horizontal key
				//There is a vertical key
				vertical_key = cv::Point(-1, -1);
				first_vertical_tile = last_vertical_tile = p;
			}
			else
			{
				text.clear(); type = TILETYPE::NORMAL; isValid = false; arrow_at_left = arrow_at_right = arrow_at_top = arrow_at_bottom = DIRECTION::NONE;
				horizontal_key = vertical_key = first_horizontal_tile = first_vertical_tile = last_horizontal_tile = last_vertical_tile = p;
				horizontal_key = vertical_key = cv::Point(-1, -1);
			}

		}

	}

	void printout()
	{
		std::wcout << " --------------------" << std::endl;
		std::wcout << "    Text: " << text << std::endl;
		std::wcout << "    horizontal_key: " << horizontal_key.x << " , " << horizontal_key.y << std::endl;
		std::wcout << "    vertical_key: " << vertical_key.x << " , " << vertical_key.y << std::endl;
		std::wcout << "    First_horizontal_tile: " << first_horizontal_tile.x << " , " << first_horizontal_tile.y << std::endl;
		std::wcout << "    First_vertical_tile: " << first_vertical_tile.x << " , " << first_vertical_tile.y << std::endl;
		std::wcout << "    last_horizontal_tile: " << last_horizontal_tile.x << " , " << last_horizontal_tile.y << std::endl;
		std::wcout << "    last_vertical_tile: " << last_vertical_tile.x << " , " << last_vertical_tile.y << std::endl;
		std::wcout << "    IsKey: " << TILETYPE::to_wstring(type) << std::endl;
		std::wcout << "    IsValid: " << isValid << std::endl;
		std::wcout << " --------------------" << std::endl;
	}
public:
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

	// indicates if there is a break at direction
	DIRECTION::itype break_at_left;
	DIRECTION::itype break_at_right;
	DIRECTION::itype break_at_top;
	DIRECTION::itype break_at_bottom;

	// surround square with a circle and a number below
	bool surround_square;
	int circle_number;

	

};


class CrossWord 
{
	
public:
	struct crossword_description_t
	{
		std::wstring title;
		int cols;
		int rows;
		std::wstring topic;
		UNITS::itype unit;
		std::vector<std::wstring> clues;
		crossword_description_t() {
			cols = 0; rows = 0; unit = UNITS::LETTER;
		}
		crossword_description_t(int c, int r, UNITS::itype t) {
			cols = c; rows = r; unit = t;
		}
	};

	CrossWord();
	~CrossWord();
	
	void createCrosswordData(crossword_description_t &desc);
	crossword_description_t getDescription();
	void loadCrosswordData(std::string _json_file_name, int format=0);
	void saveCrosswordData(std::string _json_file_name, int format = 0);


	int addKey(int row, int col, KeyTile::crossword_key_t &desc);
	int removeKey(int row, int col);
	
	/*!
	* \brief getKeySquare
	* Returns null if tile irow,icol is not key
	*/
	KeyTile * getKeySquare(int icol, int irow);



protected:

	crossword_description_t m_crossword_description;
	
	std::vector<std::vector< Tile *> > m_crossword_content;
	
};

