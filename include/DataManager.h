/**
* \defgroup  JSonLoader
* JSonLoader
*
* Copyright (C)
* (Spain) all rights reserved.
*/
#ifndef DATAMANAGER_H_
#define DATAMANAGER_H_

// Fingerguide Project
#include "ext/ext_ocv.h"
#include "ext/ext_fs.h"
#include "opencv2/calib3d.hpp"
#include "nlohmann/json.hpp"
#include "loguru-2.0.0/operativeLog.h"
#include "CrosswordData.h"
#include "QCrosswordSquare.h"
#include "ext/ext.h"

#include "enum.h"

class QCrosswordSquare;
/*!
* \brief This class represents a interface to manage JSON input parameters
*/

//BETTER_ENUM(GAME_DIRECTION, int,  VERTICAL, HORIZONTAL)
//BETTER_ENUM(DIRECTION, int, LEFT, RIGHT, UP, DOWN, NONE)
//BETTER_ENUM(START_POSITION, int, LEFT_TILE, RIGHT_TILE, TOP_TILE, BOTTOM_TILE, NOTILE)

CUSTOM_MAKE_ENUM(GAME_DIRECTION, VERTICAL, HORIZONTAL)
CUSTOM_MAKE_ENUM(DIRECTION, LEFT, RIGHT, UP, DOWN, NONE)
CUSTOM_MAKE_ENUM(START_POSITION, LEFT_TILE, RIGHT_TILE, TOP_TILE, BOTTOM_TILE, NOTILE)
//enum GAME_DIRECTION { VERTICAL = 0, HORIZONTAL = 1 };
//enum DIRECTION { LEFT = 0, RIGHT = 1, UP = 2, DOWN = 3, NONE_DIR = -1 };
//enum START_POSITION { LEFT_TILE = 0, RIGHT_TILE = 1, TOP_TILE = 2, BOTTOM_TILE = 3, NOTILE = -1 };



class DataManager {

	DataManager() { current_direction = GAME_DIRECTION::VERTICAL; };
public:

		

	struct crossword_data_t
	{
		// definition of crossword key tiles
		CrossWordData data;  

		std::vector<std::vector< QCrosswordSquare * > > graphical_data;
		std::vector< std::vector <CrossWordData::anchor_points_t> > map_square_key;  // this matrix maps one square with the positions of the key square to whose definitions it belongs.
																	// if square is not key string with its corresponent text will also be stored
		std::vector<std::vector<std::string> > playing_crossword;  // crossword which will store words when playing
	};


	

	static DataManager &instance()
	{
		static DataManager jl;
		return jl;
	}
	~DataManager() {};
	
	/*!
	* \brief load
	* This function loads a crossword from a json 
	* returns the position of the current added crossword
	*/
	static int load(std::string _file) { return DataManager::instance().loadLocal(_file); }

	/*!
	* \brief save
	* This function saves a crossword to a json file
	*/
	static void save(const char* file, int index=-1) {  DataManager::instance().saveLocal(file,index); }




	/*!
	* \brief clear
	* clear all crosswords content, but not graphical content
	*/
	static void clear() { DataManager::instance().clearLocal(); }


	/*!
	* \brief remove
	* remove pattern calibration 
	*/
	static void remove(int idx) { DataManager::instance().removeLocal(idx); }


	static int createCrossword(int cols, int rows) {
		return DataManager::instance().createCrosswordLocal(cols, rows); }

	static crossword_data_t * getCrossword(int idx=-1) {
		return DataManager::instance().getCrosswordLocal(idx);
	}


	static std::string getSquareText(int row, int col, int idx=-1) {
		return DataManager::instance().getSquareTextLocal(row,col,idx);
	}
	static std::string getSquarePlayingText(int row, int col, int idx = -1) {
		return DataManager::instance().getSquarePlayingTextLocal(row, col, idx);
	}
	

	static GAME_DIRECTION::itype getCurrentDirection() { return DataManager::instance().getCurrentDirectionLocal(); }
	static void changeDirection() {
		DataManager::instance().changeDirectionLocal();
	}

	static	void setCurrentTile(int idx, int idy) {
		DataManager::instance().setCurrentTileLocal(idx,idy);
	}

	static cv::Point getCurrentTile() { return DataManager::instance().getCurrentTileLocal(); }

	static void removeSemiSelected(int idx=-1) { return DataManager::instance().removeSemiSelectedLocal(idx); }


	static void clearSquareText(int row=-1, int col=-1, int idx_cw=-1) {  DataManager::instance().clearSquareTextLocal(row,col,idx_cw); }
	static void appendSquareText(std::string t, int  row = -1, int col = -1, int idx_cw = -1) {  DataManager::instance().appendSquareTextLocal(t,row, col, idx_cw); }
	static void setSquareText(std::string t, int row = -1, int col = -1, int idx_cw = -1) {  DataManager::instance().setSquareTextLocal(t, row, col, idx_cw); }

	static void updateBwdCurrentTile(int idc_cw=-1) { DataManager::instance().updateBwdCurrentTileLocal(idc_cw); }
	static void updateFwdCurrentTile(int idc_cw = -1) { DataManager::instance().updateFwdCurrentTileLocal(idc_cw); }

	static void addKey(CrossWordData::crossword_key_t & info, int col = -1, int row = -1, int idx_cw = -1) { DataManager::instance().addKeyLocal(info, col, row, idx_cw); }

	static bool  isKey(int col = -1, int row = -1, int idx_cw = -1) { return DataManager::instance().isKeyLocal( col, row, idx_cw); }
	static void clearTile(int col = -1, int row = -1, int idx_cw = -1) { DataManager::instance().clearTileLocal( col, row, idx_cw); }

private:
	void clearLocal() {  m_crossword_list.clear(); }
	void removeLocal(int idx) {
		m_crossword_list.erase(m_crossword_list.begin() + idx);
	}

	int createCrosswordLocal(int cols, int rows)
	{
		crossword_data_t t;
		CrossWordData::crossword_desc_t desc;
		desc.rows = rows;
		desc.cols = cols;

		t.data.createCrosswordData(desc);
		t.map_square_key.clear();
		t.playing_crossword.clear();
		t.map_square_key.resize(rows);
		t.playing_crossword.resize(rows);
		for (int i = 0; i < rows; i++)
		{
			t.map_square_key[i].resize(cols);
			t.playing_crossword[i].resize(cols);
		}

		m_crossword_list.push_back(t);
		int idx = m_crossword_list.size() - 1;
		int createCrosswordMaps(idx);
		return (int)m_crossword_list.size() - 1;
	}

	int loadLocal(std::string _file)
	{
		crossword_data_t t;
		t.data.loadCrosswordData(_file);
		m_crossword_list.push_back(t);
		int idx = m_crossword_list.size() - 1;
		createCrosswordMaps(idx);
	
		return (int)m_crossword_list.size() - 1;


	}

	void saveLocal(std::string file, int index)
	{
		if (index == -1) index = m_crossword_list.size() - 1;
		if (index < 0) return;

		crossword_data_t t;
		t = m_crossword_list[index];
		// fill crossword data with content in map_square_key
		t.data.fillWithContent(t.map_square_key);
		// save crossword
		t.data.saveCrosswordData(file);


	}

	crossword_data_t *getCrosswordLocal(int idx) {
		
		if (idx == -1) idx = m_crossword_list.size() - 1;

		if (idx >= 0 && idx < m_crossword_list.size())
			return &m_crossword_list[idx];

		return NULL;
	}


	int createCrosswordMaps(int idx)
	{
		// first check if data (json) was loaded
		crossword_data_t * data = getCrosswordLocal(idx);
		if (data)
		{
			CrossWordData::crossword_desc_t desc = data->data.getDescription();
			data->map_square_key.clear();
			data->map_square_key.resize(desc.rows);				
			data->playing_crossword.resize(desc.rows);

			
			for (int i = 0; i < desc.rows; i++)
			{
				data->map_square_key[i].resize(desc.cols);
				data->playing_crossword[i].resize(desc.cols);
			}

			for (int i = 0; i < (int) data->map_square_key.size(); i++)
			{
				
				for (int j = 0; j < (int) data->map_square_key[i].size(); j++)
				{
					nlohmann::json json_node = data->data.getKeySquare(j, i);
					if (!json_node.empty())
					{

						data->map_square_key[i][j].isKey = 1;
						for (auto& definition : json_node.items())
						{
							nlohmann::json one_def = definition.value();

							

							// key square
							cv::Point first_point,last_point;
							std::string fp = one_def["first_point"].get<std::string>();
							std::string dir = one_def["direction"].get<std::string>();
							
							if (!fp.compare("right")) first_point = cv::Point( j + 1,i);
							if (!fp.compare("left")) first_point = cv::Point( j - 1,i);
							if (!fp.compare("up")) first_point = cv::Point(j, i - 1);
							if (!fp.compare("down")) first_point = cv::Point(j, i + 1);

							// for each answer set text for that key square  def1;;def2;;def3
							if (data->map_square_key[i][j].text.empty())
								data->map_square_key[i][j].text = one_def["def"].get<std::string>();
							else
								data->map_square_key[i][j].text += ";;" + one_def["def"].get<std::string>();

							// iterate through squares
							cv::Point next_position = first_point;

							if (CrossWordData::contains(one_def, "answer"))
							{

								// calculate last point
								int nwords = one_def["answer"].size();
								if (!dir.compare("right")) last_point = first_point + cv::Point(nwords, 0);
								if (!dir.compare("left")) last_point = first_point +  cv::Point(-nwords, 0);
								if (!dir.compare("up")) last_point = first_point +  cv::Point(0, -nwords);
								if (!dir.compare("down")) last_point = first_point + cv::Point(0, nwords);

								for (auto& el : one_def["answer"].items())
								{
									


									// for each answer set text for each square belonging to the answer
									std::cout << "key: " << el.value() << '\n';
									std::string aux_text = el.value().get<std::string>();
									if (!data->map_square_key[next_position.y][next_position.x].text.empty() && aux_text.compare(data->map_square_key[next_position.y][next_position.x].text))
									{
										std::cout << "Error en casilla " << i << " , " << j << std::endl;
										std::cout << "Textos diferentes " << aux_text << " vs " << data->map_square_key[next_position.y][next_position.x].text << std::endl;
									}
									data->map_square_key[next_position.y][next_position.x].text = aux_text;
									
									data->map_square_key[next_position.y][next_position.x].isValid = true;
									data->map_square_key[next_position.y][next_position.x].isKey = 0;

									if (!dir.compare("right") || !dir.compare("left"))
									{
										data->map_square_key[next_position.y][next_position.x].horizontal_key = cv::Point(j, i);
										data->map_square_key[next_position.y][next_position.x].first_horizontal_tile = first_point;
										data->map_square_key[next_position.y][next_position.x].last_horizontal_tile = last_point;

									}
									if (!dir.compare("up") || !dir.compare("down"))
									{
										data->map_square_key[next_position.y][next_position.x].vertical_key = cv::Point(j, i);
										data->map_square_key[next_position.y][next_position.x].first_vertical_tile = first_point;
										data->map_square_key[next_position.y][next_position.x].last_vertical_tile = last_point;
									}

									if (!dir.compare("right")) next_position += cv::Point(1, 0);
									if (!dir.compare("left")) next_position += cv::Point(-1,0);
									if (!dir.compare("up")) next_position += cv::Point(0, -1);
									if (!dir.compare("down")) next_position += cv::Point(0, 1);
								}
							}

						}





					}
					

				}
			}

		}
		return 1;
	}

	void removeSemiSelectedLocal(int idx)
	{
		if (idx < 0) idx = (int)m_crossword_list.size() - 1;
		if (idx >= 0 && idx < m_crossword_list.size())
		{
			for (int i = 0; i < m_crossword_list[idx].graphical_data.size(); i++)
			{
				for (int j = 0; j < m_crossword_list[idx].graphical_data[i].size(); i++)
				{
					// this doesnt work
					//QCrosswordSquare * qs = m_crossword_list[idx].graphical_data[i][j];
					//if (qs)
					//	qs->setSemiSelected(false);
				}
			}
		}
	}
	std::string getSquareTextLocal(int row, int col, int idx)
	{
		if (idx < 0) idx = (int)m_crossword_list.size() - 1;
		if (idx >=0 && idx < m_crossword_list.size())
			return m_crossword_list[idx].map_square_key[row][col].text;
		else return std::string();

	}


	std::string getSquarePlayingTextLocal(int row, int col, int idx)
	{
		if (idx < 0) idx = (int)m_crossword_list.size() - 1;
		if (idx >= 0 && idx < m_crossword_list.size())
		{
			if (!m_crossword_list[idx].map_square_key.empty())
			{
				if (m_crossword_list[idx].map_square_key[row][col].isKey)
					return m_crossword_list[idx].map_square_key[row][col].text;
				else
					return m_crossword_list[idx].playing_crossword[row][col];
			}
		}
		
		return std::string();
	}
	
	GAME_DIRECTION::itype getCurrentDirectionLocal() { return current_direction; }
	void changeDirectionLocal() { 
		if (current_direction == GAME_DIRECTION::VERTICAL) current_direction = GAME_DIRECTION::HORIZONTAL;
		else current_direction = GAME_DIRECTION::VERTICAL;
	}

	void setCurrentTileLocal(int idx, int idy) {
		current_tile = cv::Point(idx, idy);
	}

	cv::Point getCurrentTileLocal() { return current_tile; }

	void clearSquareTextLocal(int row, int col, int idx_cw) {
		if (idx_cw == -1) idx_cw = (int)m_crossword_list.size()-1;
		if (row == -1) row = current_tile.y;
		if (col == -1) col = current_tile.x;

		if (idx_cw >= 0)
		{
			if (StatusManager::is(StatusManager::ST_EDITING))
			{
				m_crossword_list[idx_cw].map_square_key[row][col].text.clear();
			}
			else if ( StatusManager::is(StatusManager::ST_PLAYING) || StatusManager::is(StatusManager::ST_PLAYING_WITH_CLUES))
			{
				m_crossword_list[idx_cw].playing_crossword[row][col].clear();

			}
			
		}
	}

	void appendSquareTextLocal(std::string t, int row, int col, int idx_cw) {
		if (idx_cw == -1) idx_cw = (int)m_crossword_list.size() - 1;
		if (row == -1) row = current_tile.y;
		if (col == -1) col = current_tile.x;

		if (idx_cw >= 0)
		{
			if (StatusManager::is(StatusManager::ST_EDITING))
			{
				m_crossword_list[idx_cw].map_square_key[row][col].text += t;
			}
			else if (StatusManager::is(StatusManager::ST_PLAYING) || StatusManager::is(StatusManager::ST_PLAYING_WITH_CLUES))
			{
				m_crossword_list[idx_cw].playing_crossword[row][col] += t;
			}

		}
	}

	void setSquareTextLocal(std::string t, int row, int col, int idx_cw) {
		if (idx_cw == -1) idx_cw = (int)m_crossword_list.size() - 1;
		if (row == -1) row = current_tile.y;
		if (col == -1) col = current_tile.x;

		if (idx_cw >= 0)
		{
			if (StatusManager::is(StatusManager::ST_EDITING))
			{
				m_crossword_list[idx_cw].map_square_key[row][col].text = t;
			}
			else if (StatusManager::is(StatusManager::ST_PLAYING) || StatusManager::is(StatusManager::ST_PLAYING_WITH_CLUES))
			{
				m_crossword_list[idx_cw].playing_crossword[row][col] = t;

			}

		}
	}


	void updateBwdCurrentTileLocal(int idx_cw)
	{
		if (idx_cw == -1) idx_cw = (int)m_crossword_list.size() - 1;

		if (current_direction == GAME_DIRECTION::HORIZONTAL)
		{
			int first_x = m_crossword_list[idx_cw].map_square_key[current_tile.y][current_tile.x].first_horizontal_tile.x;
			if (current_tile.x > first_x)
			{
				current_tile.x--;
			}
		}
		else if (current_direction == GAME_DIRECTION::VERTICAL)
		{
			
			int first_y = m_crossword_list[idx_cw].map_square_key[current_tile.y][current_tile.x].first_vertical_tile.y;
			if (current_tile.y > first_y)
			{
				current_tile.y--;
			}
		}
	}

	void updateFwdCurrentTileLocal(int idx_cw)
	{
		if (idx_cw == -1) idx_cw = (int)m_crossword_list.size() - 1;

		if (current_direction == GAME_DIRECTION::HORIZONTAL)
		{
			int last_x = m_crossword_list[idx_cw].map_square_key[current_tile.y][current_tile.x].last_horizontal_tile.x;
			if ((current_tile.x+1) < last_x)
			{
				current_tile.x++;
			}
		}
		else if (current_direction == GAME_DIRECTION::VERTICAL)
		{

			int last_y = m_crossword_list[idx_cw].map_square_key[current_tile.y][current_tile.x].last_vertical_tile.y;
			if ((current_tile.y +1) < last_y)
			{
				current_tile.y++;
			}
		}
	}

	void addKeyLocal(CrossWordData::crossword_key_t & info, int col,  int row , int idx_cw ) {
		if (idx_cw == -1) idx_cw = (int)m_crossword_list.size() - 1;
		if (col == -1) col = current_tile.x;
		if (row == -1) row = current_tile.y;

		m_crossword_list[idx_cw].data.addKey(row, col, info);
		m_crossword_list[idx_cw].map_square_key[row][col].isKey = true;
		m_crossword_list[idx_cw].map_square_key[row][col].isValid=true;
		// can not access to QCrosswordSquare
		//if (m_crossword_list[idx_cw].graphical_data[row][col]) 
		//	m_crossword_list[idx_cw].graphical_data[row][col]->isKey = true;
	}

	void clearTileLocal(int col, int row, int idx_cw) {
		if (idx_cw == -1) idx_cw = (int)m_crossword_list.size() - 1;
		if (col == -1) col = current_tile.x;
		if (row == -1) row = current_tile.y;

		m_crossword_list[idx_cw].data.removeKey(row, col);
		m_crossword_list[idx_cw].map_square_key[row][col].isKey = false;
		m_crossword_list[idx_cw].map_square_key[row][col].isValid = false;
		// can not access to QCrosswordSquare
		//if (m_crossword_list[idx_cw].graphical_data[row][col])
		//	m_crossword_list[idx_cw].graphical_data[row][col]->isKey = false;
	}
	
	bool isKeyLocal(int col = -1, int row = -1, int idx_cw = -1) { 
		CrossWordData::crossword_key_t k;
		if (idx_cw == -1) idx_cw = (int)m_crossword_list.size() - 1;
		if (col == -1) col = current_tile.x;
		if (row == -1) row = current_tile.y;
		return 	m_crossword_list[idx_cw].map_square_key[row][col].isKey;
	
	}


private:
	
	std::vector<crossword_data_t> m_crossword_list;  // one image calibration set for each video that means for each distance

	GAME_DIRECTION::itype current_direction;

	cv::Point current_tile;

};
#endif  // MODULES_PARAMETERS_ARGSBASEJSON_H_
