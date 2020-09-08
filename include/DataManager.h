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

#include "Crossword.h"
#include "QCrosswordSquare.h"
#include "ext/ext.h"

#include "enum.h"

class QCrosswordSquare;
/*!
* \brief This class represents a interface to manage JSON input parameters
*/


class DataManager {

	DataManager() { current_direction = GAME_DIRECTION::VERTICAL; };
	~DataManager() {};
public:

		

	struct crossword_data_t
	{
		// definition of crossword key tiles
		//CrossWordData data;  
		//std::vector< std::vector <CrossWordData::anchor_points_t> > map_square_key;  // this matrix maps one square with the positions of the key square to whose definitions it belongs.
																					 // if square is not key string with its corresponent text will also be stored

		CrossWord data;
		std::vector<std::vector< QCrosswordSquare * > > graphical_data;
		std::vector<std::vector<std::wstring> > playing_crossword;  // crossword which will store words when playing
	};


	

	static DataManager &instance()
	{
		static DataManager jl;
		return jl;
	}
	
	
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
	* \brief savegame
	* This function saves a crossword to a json file
	*/
	static void savegame(const char* file, int index = -1) { DataManager::instance().savegameLocal(file, index); }


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


	static int createCrossword(int cols, int rows, std::wstring title=std::wstring(), std::wstring topic=std::wstring(), std::string unit=std::string()) {
		return DataManager::instance().createCrosswordLocal(cols, rows, title,topic,unit); }

	static crossword_data_t * getCrossword(int idx=-1) {
		return DataManager::instance().getCrosswordLocal(idx);
	}


	static std::wstring getSquareText(int row, int col, int idx=-1) {
		return DataManager::instance().getSquareTextLocal(row,col,idx);
	}
	static std::wstring getSquarePlayingText(int row, int col, int idx = -1) {
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
	static void appendSquareText(std::wstring t, int  row = -1, int col = -1, int idx_cw = -1) {  DataManager::instance().appendSquareTextLocal(t,row, col, idx_cw); }
	static void setSquareText(std::wstring t, int row = -1, int col = -1, int idx_cw = -1) {  DataManager::instance().setSquareTextLocal(t, row, col, idx_cw); }

	static void updateBwdCurrentTile(int idc_cw=-1) { DataManager::instance().updateBwdCurrentTileLocal(idc_cw); }
	static void updateFwdCurrentTile(int idc_cw = -1) { DataManager::instance().updateFwdCurrentTileLocal(idc_cw); }

	static void addKey(KeyTile::crossword_key_t & info, int col = -1, int row = -1, int idx_cw = -1) { DataManager::instance().addKeyLocal(info, col, row, idx_cw); }

	static bool  isKey(int col = -1, int row = -1, int idx_cw = -1) { return DataManager::instance().isKeyLocal( col, row, idx_cw); }
	static void clearTile(int col = -1, int row = -1, int idx_cw = -1) { DataManager::instance().clearTileLocal( col, row, idx_cw); }
	static void removeKey(int col = -1, int row = -1, int idx_cw = -1) { DataManager::instance().removeKeyLocal(col, row, idx_cw); }


	static void updateNormalTilesInfoFromKey(int col = -1, int row = -1, int idx_cw = -1){ DataManager::instance().updateNormalTilesInfoFromKeyLocal(col, row, idx_cw); }
private:
	void clearLocal() {  m_crossword_list.clear(); }
	void removeLocal(int idx) {
		m_crossword_list.erase(m_crossword_list.begin() + idx);
	}

	int createCrosswordLocal(int cols, int rows, std::wstring title, std::wstring topic, std::string unit)
	{
		crossword_data_t t;
		CrossWord::crossword_description_t desc;
		desc.rows = rows;
		desc.cols = cols;
		//desc.title = ext::string::string_to_wstring(title);
		desc.title = title;
		//desc.topic = ext::string::string_to_wstring(topic);
		desc.topic = topic;
		desc.unit = UNITS::to_itype(unit);

		t.data.createCrosswordData(desc);
		//t.map_square_key.clear();
		//t.map_square_key.resize(rows);
		t.playing_crossword.clear();
		t.playing_crossword.resize(rows);
		for (int i = 0; i < rows; i++)
		{
			//t.map_square_key[i].resize(cols);
			t.playing_crossword[i].resize(cols);
		}

		m_crossword_list.push_back(t);
		int idx = m_crossword_list.size() - 1;
		//createCrosswordMaps(idx);
		return (int)m_crossword_list.size() - 1;
	}

	int loadLocal(std::string _file)
	{
		crossword_data_t t;
		t.data.loadCrosswordData(_file, 1);
		m_crossword_list.push_back(t);
		int idx = m_crossword_list.size() - 1;

	
		t.playing_crossword.clear();
		t.playing_crossword.resize(t.data.getDescription().rows);
		for (int i = 0; i < t.data.getDescription().rows; i++)
		{
			t.playing_crossword[i].resize(t.data.getDescription().cols);
		}


		//createCrosswordMaps(idx);
	
		return (int)m_crossword_list.size() - 1;


	}

	void saveLocal(std::string file, int index)
	{
		if (index == -1) index = m_crossword_list.size() - 1;
		if (index < 0) return;

		crossword_data_t t;
		t = m_crossword_list[index];
		// fill crossword data with content in map_square_key
		//t.data.fillWithContent(t.map_square_key);
		// save crossword
		t.data.saveCrosswordData(file,1);
		
	

	}


	void savegameLocal(std::string file, int index)
	{
		if (index == -1) index = m_crossword_list.size() - 1;
		if (index < 0) return;

		if (!ext::fs::fexists(file)) saveLocal(file, index);

		std::string path, fname, ext;
		ext::mix::splitFileName(file, path, fname, ext);

		crossword_data_t t;
		t = m_crossword_list[index];
		nlohmann::json json_playing_info;

		json_playing_info["source"] = file;

		// save crossword
		json_playing_info["content"] = nlohmann::json::array();
		for (int i = 0; i < t.playing_crossword.size(); i++)
		{
			for (int j = 0; j < t.playing_crossword[i].size(); j++)
			{
				nlohmann::json node;
				if (!t.playing_crossword[i][j].empty())
				{
					node[std::to_string(i)][std::to_string(j)] = t.playing_crossword[i][j];
					json_playing_info["content"].push_back(node);
				}
			}
		}

		std::string fname_game = path + fname + "_game." + ext;
		std::ofstream o(fname_game);
		o << std::setw(4) << json_playing_info << std::endl;
		o.close();
	}

	crossword_data_t *getCrosswordLocal(int idx) {
		
		if (idx == -1) idx = m_crossword_list.size() - 1;

		if (idx >= 0 && idx < m_crossword_list.size())
			return &m_crossword_list[idx];

		return NULL;
	}

	
	int createCrosswordMaps(int idx)
	{
		//// first check if data (json) was loaded
		//crossword_data_t * data = getCrosswordLocal(idx);
		//if (data)
		//{
		//	// get crossword info
		//	CrossWordData::crossword_desc_t desc = data->data.getDescription();
		//	data->map_square_key.clear();
		//	data->map_square_key.resize(desc.rows);				
		//	data->playing_crossword.resize(desc.rows);

		//	{
		//		LOG_SCOPE_F(INFO, "Loading Crossword: ...");
		//		LOG_F(INFO, "Size: %d x %d", desc.rows, desc.cols);
		//		LOG_F(INFO, "Topic : %s ", desc.topic.c_str());
		//		LOG_F(INFO, "Units : %s ", (desc.unit)?"Sylabes":"Letters");
		//	}
		//	
		//	for (int i = 0; i < desc.rows; i++)
		//	{
		//		data->map_square_key[i].resize(desc.cols);
		//		data->playing_crossword[i].resize(desc.cols);
		//		for (int j = 0; j < desc.cols; j++)
		//		{
		//			data->map_square_key[i][j].first_horizontal_tile = cv::Point(j, i);
		//			data->map_square_key[i][j].first_vertical_tile = cv::Point(j, i);
		//			data->map_square_key[i][j].last_horizontal_tile = cv::Point(j, i);
		//			data->map_square_key[i][j].last_vertical_tile = cv::Point(j, i);
		//		}
		//	}

		//	for (int i = 0; i < (int) data->map_square_key.size(); i++)
		//	{
		//		
		//		for (int j = 0; j < (int) data->map_square_key[i].size(); j++)
		//		{

		//			// for each key tile

		//			nlohmann::json json_node = data->data.getKeySquare(j, i);
		//			if (!json_node.empty())
		//			{

		//				data->map_square_key[i][j].isKey = 1;
		//				for (auto& definition : json_node.items())
		//				{
		//					// for each key tile's def
		//					nlohmann::json &one_def = definition.value();

		//					
		//				
		//					// key square
		//					cv::Point first_point,last_point;
		//					std::string fp_aux = one_def["first_point"].get<std::string>();
		//					std::string dir_aux = one_def["direction"].get<std::string>();
		//					START_POSITION::itype fp = START_POSITION::to_itype(fp_aux);
		//					DIRECTION::itype dir = DIRECTION::to_itype(dir_aux);
		//					
		//					if (fp == START_POSITION::RIGHT) first_point = cv::Point( j + 1,i);
		//					else if (fp == START_POSITION::LEFT) first_point = cv::Point( j - 1,i);
		//					else if (fp == START_POSITION::TOP) first_point = cv::Point(j, i - 1);
		//					else if (fp == START_POSITION::BOTTOM) first_point = cv::Point(j, i + 1);
		//					else 
		//						std::cout << "Error parsing" << std::endl;

		//					// set arrow indicator
		//					if (fp == START_POSITION::RIGHT) data->map_square_key[first_point.y][first_point.x].arrow_at_left = dir;
		//					else if (fp == START_POSITION::LEFT) data->map_square_key[first_point.y][first_point.x].arrow_at_right = dir;
		//					else if (fp == START_POSITION::TOP) data->map_square_key[first_point.y][first_point.x].arrow_at_bottom = dir;
		//					else if (fp == START_POSITION::BOTTOM) data->map_square_key[first_point.y][first_point.x].arrow_at_top = dir;
		//					else
		//						std::cout << "Error parsing" << std::endl;
		//					

		//					// for each answer set text for that key square  def1;;def2;;def3
		//					if (data->map_square_key[i][j].text.empty())
		//						data->map_square_key[i][j].text = one_def["def"].get<std::string>();
		//					else
		//						data->map_square_key[i][j].text += ";;" + one_def["def"].get<std::string>();

		//					// iterate through squares
		//					cv::Point next_position = first_point;

		//					if (CrossWordData::contains(one_def, "answer"))
		//					{

		//						// calculate last point
		//						int nwords = one_def["answer"].size();
		//						if (dir == DIRECTION::RIGHT) last_point = first_point + cv::Point(nwords, 0);
		//						else if (dir == DIRECTION::LEFT) last_point = first_point +  cv::Point(-nwords, 0);
		//						else if (dir == DIRECTION::UP) last_point = first_point +  cv::Point(0, -nwords);
		//						else if (dir == DIRECTION::DOWN) last_point = first_point + cv::Point(0, nwords);
		//						else 
		//							std::cout << "Error parsing" << std::endl;
		//						for (auto& el : one_def["answer"].items())
		//						{
		//						
		//							// for each answer set text for each square belonging to the answer
		//						//	std::cout << "key: " << el.value() << '\n';
		//							std::string aux_text = el.value().get<std::string>();
		//							if (!data->map_square_key[next_position.y][next_position.x].text.empty() && aux_text.compare(data->map_square_key[next_position.y][next_position.x].text))
		//							{
		//								cv::Point conflicting_key;
		//								if (dir == DIRECTION::RIGHT || dir == DIRECTION::LEFT)
		//									conflicting_key = data->map_square_key[next_position.y][next_position.x].vertical_key;
		//								else conflicting_key = data->map_square_key[next_position.y][next_position.x].horizontal_key;

		//								LOG_F(ERROR," Error between tiles (%d, %d) and (%d, %d)", i,j, conflicting_key.y, conflicting_key.x);
		//								LOG_F(ERROR, "Textos diferentes %s vs %s", aux_text.c_str(),data->map_square_key[next_position.y][next_position.x].text.c_str());
		//							}
		//							data->map_square_key[next_position.y][next_position.x].text = aux_text;
		//							
		//							data->map_square_key[next_position.y][next_position.x].isValid = true;
		//							data->map_square_key[next_position.y][next_position.x].isKey = 0;

		//							if (dir == DIRECTION::RIGHT || dir == DIRECTION::LEFT)
		//							{
		//								data->map_square_key[next_position.y][next_position.x].horizontal_key = cv::Point(j, i);
		//								data->map_square_key[next_position.y][next_position.x].first_horizontal_tile = first_point;
		//								data->map_square_key[next_position.y][next_position.x].last_horizontal_tile = last_point;

		//							}
		//							if (dir == DIRECTION::UP || dir == DIRECTION::DOWN)
		//							{
		//								data->map_square_key[next_position.y][next_position.x].vertical_key = cv::Point(j, i);
		//								data->map_square_key[next_position.y][next_position.x].first_vertical_tile = first_point;
		//								data->map_square_key[next_position.y][next_position.x].last_vertical_tile = last_point;
		//							}

		//							if (dir == DIRECTION::RIGHT) next_position += cv::Point(1, 0);
		//							if (dir == DIRECTION::LEFT) next_position += cv::Point(-1,0);
		//							if (dir == DIRECTION::UP) next_position += cv::Point(0, -1);
		//							if (dir == DIRECTION::DOWN) next_position += cv::Point(0, 1);
		//						}
		//					}

		//				}





		//			}
		//			

		//		}
		//	}

		//}
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
	std::wstring getSquareTextLocal(int row, int col, int idx)
	{
		if (m_crossword_list.empty()) return std::wstring();
		if (idx < 0) idx = (int)m_crossword_list.size() - 1;
		if (idx >= 0 && idx < m_crossword_list.size())
		{
			return m_crossword_list[idx].data.getText(row,col);
//			return m_crossword_list[idx].map_square_key[row][col].text;
		}
		else return std::wstring();

	}

	void removeKeyLocal(int row, int col, int idx)
	{
		if (m_crossword_list.empty()) return;
		if (idx < 0) idx = (int)m_crossword_list.size() - 1;
		if (idx >= 0 && idx < m_crossword_list.size())
		{
			if (row == -1) row = DataManager::getCurrentTile().y;
			if (col == -1) col = DataManager::getCurrentTile().x;

			//remove from map
			m_crossword_list[idx].data.removeKey(row, col);
			//	nlohmann::json json_node = m_crossword_list[idx].data.getKeySquare(col, row);
			//	if (!json_node.empty())
			//	{

			//		for (auto& definition : json_node.items())
			//		{
			//			// for each key tile's def
			//			nlohmann::json &one_def = definition.value();



			//			// key square
			//			cv::Point first_point, last_point;
			//			std::string fp_aux = one_def["first_point"].get<std::string>();
			//			std::string dir_aux = one_def["direction"].get<std::string>();
			//			START_POSITION::itype fp = START_POSITION::to_itype(fp_aux);
			//			DIRECTION::itype dir = DIRECTION::to_itype(dir_aux);

			//			if (fp == START_POSITION::RIGHT) first_point = cv::Point(col + 1, row);
			//			else if (fp == START_POSITION::LEFT) first_point = cv::Point(col - 1, row);
			//			else if (fp == START_POSITION::TOP) first_point = cv::Point(col, row - 1);
			//			else if (fp == START_POSITION::BOTTOM) first_point = cv::Point(col, row + 1);
			//			else
			//				std::cout << "Error parsing" << std::endl;

			//			// set arrow indicator
			//			if (fp == START_POSITION::RIGHT) m_crossword_list[idx].map_square_key[first_point.y][first_point.x].arrow_at_left = DIRECTION::NONE;
			//			else if (fp == START_POSITION::LEFT) m_crossword_list[idx].map_square_key[first_point.y][first_point.x].arrow_at_right = DIRECTION::NONE;
			//			else if (fp == START_POSITION::TOP) m_crossword_list[idx].map_square_key[first_point.y][first_point.x].arrow_at_bottom = DIRECTION::NONE;
			//			else if (fp == START_POSITION::BOTTOM) m_crossword_list[idx].map_square_key[first_point.y][first_point.x].arrow_at_top = DIRECTION::NONE;
			//			else
			//				std::cout << "Error parsing" << std::endl;


			//			// iterate through squares
			//			cv::Point next_position = first_point;

			//			if (CrossWordData::contains(one_def, "answer"))
			//			{

			//				// calculate last point
			//				int nwords = one_def["answer"].size();
			//				if (dir == DIRECTION::RIGHT) last_point = first_point + cv::Point(nwords, 0);
			//				else if (dir == DIRECTION::LEFT) last_point = first_point + cv::Point(-nwords, 0);
			//				else if (dir == DIRECTION::UP) last_point = first_point + cv::Point(0, -nwords);
			//				else if (dir == DIRECTION::DOWN) last_point = first_point + cv::Point(0, nwords);
			//				else
			//					std::cout << "Error parsing" << std::endl;
			//				
			//				for (int nsquare=0;nsquare<nwords;nsquare++)
			//				{
			//					m_crossword_list[idx].map_square_key[next_position.y][next_position.x].resetInDirection(next_position, dir);
			//					
			//					if (dir == DIRECTION::RIGHT) next_position += cv::Point(1, 0);
			//					if (dir == DIRECTION::LEFT) next_position += cv::Point(-1, 0);
			//					if (dir == DIRECTION::UP) next_position += cv::Point(0, -1);
			//					if (dir == DIRECTION::DOWN) next_position += cv::Point(0, 1);
			//				}
			//			}

			//		}

			//		m_crossword_list[idx].map_square_key[row][col].reset();



			//	}
			//	// remove from json		
			//	m_crossword_list[idx].data.removeKey(row,col);

			//}

		}

	}

	std::wstring getSquarePlayingTextLocal(int row, int col, int idx)
	{
		if (idx < 0) idx = (int)m_crossword_list.size() - 1;
		if (idx >= 0 && idx < m_crossword_list.size())
		{
			//if (!m_crossword_list[idx].map_square_key.empty())
			if (!m_crossword_list[idx].data.empty())
			{
				if (m_crossword_list[idx].data.getTile(row, col) && 
					m_crossword_list[idx].data.getTile(row,col)->type == TILETYPE::KEY)
					return m_crossword_list[idx].data.getText(row,col);
				else
				{
					if (row>=0 && row < (int)m_crossword_list[idx].playing_crossword.size() &&
						col >= 0 && col < (int)m_crossword_list[idx].playing_crossword.size())
						return m_crossword_list[idx].playing_crossword[row][col];
				}
			}
		}
		
		return std::wstring();
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
				m_crossword_list[idx_cw].data.clearText(row,col);
			}
			else if ( StatusManager::is(StatusManager::ST_PLAYING) || StatusManager::is(StatusManager::ST_PLAYING_WITH_CLUES))
			{
				m_crossword_list[idx_cw].playing_crossword[row][col].clear();

			}
			
		}
	}

	void appendSquareTextLocal(std::wstring t, int row, int col, int idx_cw) {
		if (idx_cw == -1) idx_cw = (int)m_crossword_list.size() - 1;
		if (row == -1) row = current_tile.y;
		if (col == -1) col = current_tile.x;

		if (idx_cw >= 0)
		{
			if (StatusManager::is(StatusManager::ST_EDITING))
			{
				m_crossword_list[idx_cw].data.appendText(row, col, t);
			}
			else if (StatusManager::is(StatusManager::ST_PLAYING) || StatusManager::is(StatusManager::ST_PLAYING_WITH_CLUES))
			{
				m_crossword_list[idx_cw].playing_crossword[row][col] += t;
			}

		}
	}

	void setSquareTextLocal(std::wstring t, int row, int col, int idx_cw) {
		if (m_crossword_list.empty()) return;
		if (idx_cw == -1) idx_cw = (int)m_crossword_list.size() - 1;
		if (row == -1) row = current_tile.y;
		if (col == -1) col = current_tile.x;

		if (idx_cw >= 0)
		{
			if (StatusManager::is(StatusManager::ST_EDITING))
			{
				m_crossword_list[idx_cw].data.setText(row,col,t);
			}
			else if (StatusManager::is(StatusManager::ST_PLAYING) || StatusManager::is(StatusManager::ST_PLAYING_WITH_CLUES))
			{
				m_crossword_list[idx_cw].playing_crossword[row][col] = t;

			}

		}
	}


	void updateBwdCurrentTileLocal(int idx_cw)
	{
		if (m_crossword_list.empty()) return;
		if (idx_cw == -1) idx_cw = (int)m_crossword_list.size() - 1;

		NormalTile* nt = dynamic_cast<NormalTile*>(m_crossword_list[idx_cw].data.getTile(current_tile.y,current_tile.x));
		if (nt)
		{
			if (current_direction == GAME_DIRECTION::HORIZONTAL)
			{
				int first_x = nt->first_horizontal_tile.x;
				if (current_tile.x > first_x)
				{
					current_tile.x--;
				}
			}
			else if (current_direction == GAME_DIRECTION::VERTICAL)
			{

				int first_y = nt->first_vertical_tile.y;
				if (current_tile.y > first_y)
				{
					current_tile.y--;
				}
			}
		}
	}

	void updateFwdCurrentTileLocal(int idx_cw)
	{
		if (m_crossword_list.empty()) return;
		
		if (idx_cw == -1) idx_cw = (int)m_crossword_list.size() - 1;

		NormalTile* nt = dynamic_cast<NormalTile*>(m_crossword_list[idx_cw].data.getTile(current_tile.y,current_tile.x));
		if (nt)
		{

			if (current_direction == GAME_DIRECTION::HORIZONTAL)
			{
				int last_x = nt->last_horizontal_tile.x;
				if ((current_tile.x + 1) < last_x)
				{
					current_tile.x++;
				}
			}
			else if (current_direction == GAME_DIRECTION::VERTICAL)
			{

				int last_y = nt->last_vertical_tile.y;
				if ((current_tile.y + 1) < last_y)
				{
					current_tile.y++;
				}

			}
		}
	}

	void addKeyLocal(KeyTile::crossword_key_t & info, int col,  int row , int idx_cw ) {
		if (idx_cw == -1) idx_cw = (int)m_crossword_list.size() - 1;
		if (col == -1) col = current_tile.x;
		if (row == -1) row = current_tile.y;

		m_crossword_list[idx_cw].data.addKey(row, col, info);
		//m_crossword_list[idx_cw].map_square_key[row][col].isKey = true;
		//m_crossword_list[idx_cw].map_square_key[row][col].isValid=true;

		//if (m_crossword_list[idx_cw].map_square_key[row][col].text.empty())
		//	m_crossword_list[idx_cw].map_square_key[row][col].text += info.def;
		//else
		//	m_crossword_list[idx_cw].map_square_key[row][col].text += ";;"+info.def;


		//cv::Point first_point, last_point;
		//std::string fp_aux = info.first_point;
		//std::string dir_aux = info.direction;
		//START_POSITION::itype fp = START_POSITION::to_itype(fp_aux);
		//DIRECTION::itype dir = DIRECTION::to_itype(dir_aux);

		//if (fp == START_POSITION::RIGHT) first_point = cv::Point(col + 1, row);
		//else if (fp == START_POSITION::LEFT) first_point = cv::Point(col - 1, row);
		//else if (fp == START_POSITION::TOP) first_point = cv::Point(col, row - 1);
		//else if (fp == START_POSITION::BOTTOM) first_point = cv::Point(col, row + 1);
		//else
		//	std::cout << "Error parsing" << std::endl;

		//if (first_point.y<0 || first_point.x<0 ||
		//	first_point.y > (int)m_crossword_list[idx_cw].map_square_key.size() ||
		//	first_point.x > (int)m_crossword_list[idx_cw].map_square_key[first_point.y].size())
		//{
		//	std::cout << "first point out of margins" << std::endl;
		//	return;
		//}
		//// set arrow indicator
		//if (fp == START_POSITION::RIGHT)m_crossword_list[idx_cw].map_square_key[first_point.y][first_point.x].arrow_at_left = dir;
		//else if (fp == START_POSITION::LEFT) m_crossword_list[idx_cw].map_square_key[first_point.y][first_point.x].arrow_at_right = dir;
		//else if (fp == START_POSITION::TOP) m_crossword_list[idx_cw].map_square_key[first_point.y][first_point.x].arrow_at_bottom = dir;
		//else if (fp == START_POSITION::BOTTOM) m_crossword_list[idx_cw].map_square_key[first_point.y][first_point.x].arrow_at_top = dir;
		//else
		//	std::cout << "Error parsing" << std::endl;

		//// iterate through tiles belonging this definition
		//cv::Point next_position = first_point;

		//if (!info.answers.empty())
		//{

		//	// calculate last point
		//	int nwords = info.answers.size();

		//	//check number of words
		//	if (dir == DIRECTION::UP || dir == DIRECTION::DOWN)
		//	{
		//		int ntotalwordsforthisposition = (int)m_crossword_list[idx_cw].map_square_key.size() - first_point.y;
		//		if (nwords > ntotalwordsforthisposition)
		//		{
		//			nwords = ntotalwordsforthisposition;
		//			info.answers.clear();
		//			info.answers.resize(ntotalwordsforthisposition);

		//		}
		//	}
		//	else
		//	{
		//		int ntotalwordsforthisposition = (int)m_crossword_list[idx_cw].map_square_key[first_point.y].size() - first_point.x;

		//		if (nwords > ntotalwordsforthisposition)
		//		{
		//			nwords = ntotalwordsforthisposition;
		//			info.answers.clear();
		//			info.answers.resize(ntotalwordsforthisposition);

		//		}
		//	}


		//	if (dir == DIRECTION::RIGHT) last_point = first_point + cv::Point(nwords, 0);
		//	else if (dir == DIRECTION::LEFT) last_point = first_point + cv::Point(-nwords, 0);
		//	else if (dir == DIRECTION::UP) last_point = first_point + cv::Point(0, -nwords);
		//	else if (dir == DIRECTION::DOWN) last_point = first_point + cv::Point(0, nwords);
		//	else
		//		std::cout << "Error parsing" << std::endl;
		//	for (int nel =0 ; nel<(int)info.answers.size();nel++)
		//	{

		//		if (next_position.y >= (int)m_crossword_list[idx_cw].map_square_key.size() || next_position.x >= (int)m_crossword_list[idx_cw].map_square_key[next_position.y].size()) break;

		//		// for each answer set text for each square belonging to the answer
		//		//	std::cout << "key: " << el.value() << '\n';
		//		m_crossword_list[idx_cw].map_square_key[next_position.y][next_position.x].isValid = true;
		//		m_crossword_list[idx_cw].map_square_key[next_position.y][next_position.x].isKey = 0;

		//		if (dir == DIRECTION::RIGHT || dir == DIRECTION::LEFT)
		//		{
		//			m_crossword_list[idx_cw].map_square_key[next_position.y][next_position.x].horizontal_key = cv::Point(col, row);
		//			m_crossword_list[idx_cw].map_square_key[next_position.y][next_position.x].first_horizontal_tile = first_point;
		//			m_crossword_list[idx_cw].map_square_key[next_position.y][next_position.x].last_horizontal_tile = last_point;

		//		}
		//		if (dir == DIRECTION::UP || dir == DIRECTION::DOWN)
		//		{
		//			m_crossword_list[idx_cw].map_square_key[next_position.y][next_position.x].vertical_key = cv::Point(col, row);
		//			m_crossword_list[idx_cw].map_square_key[next_position.y][next_position.x].first_vertical_tile = first_point;
		//			m_crossword_list[idx_cw].map_square_key[next_position.y][next_position.x].last_vertical_tile = last_point;
		//		}

		//		if (dir == DIRECTION::RIGHT) next_position += cv::Point(1, 0);
		//		if (dir == DIRECTION::LEFT) next_position += cv::Point(-1, 0);
		//		if (dir == DIRECTION::UP) next_position += cv::Point(0, -1);
		//		if (dir == DIRECTION::DOWN) next_position += cv::Point(0, 1);

		//		if (next_position.y<0 || next_position.x<0 ||
		//			next_position.y >(int)m_crossword_list[idx_cw].map_square_key.size()-1 ||
		//			next_position.x >(int)m_crossword_list[idx_cw].map_square_key[next_position.y].size()-1)
		//		{
		//			LOG_F(ERROR,"Next point out of margins...");
		//			break;
		//		}
		//	}
		//}



		// can not access to QCrosswordSquare
		//if (m_crossword_list[idx_cw].graphical_data[row][col]) 
		//	m_crossword_list[idx_cw].graphical_data[row][col]->isKey = true;
	}

	void updateNormalTilesInfoFromKeyLocal(int col=-1, int row=-1, int idx_cw=-1) {
		if (idx_cw == -1) idx_cw = (int)m_crossword_list.size() - 1;
		if (col == -1) col = current_tile.x;
		if (row == -1) row = current_tile.y;

		m_crossword_list[idx_cw].data.updateNormalTilesFromKey(row, col);
	}

	void clearTileLocal(int col=-1, int row=-1, int idx_cw=-1) {
		if (idx_cw == -1) idx_cw = (int)m_crossword_list.size() - 1;
		if (col == -1) col = current_tile.x;
		if (row == -1) row = current_tile.y;

		m_crossword_list[idx_cw].data.removeKey(row, col);
		//m_crossword_list[idx_cw].map_square_key[row][col].isKey = false;
		//m_crossword_list[idx_cw].map_square_key[row][col].isValid = false;
		// can not access to QCrosswordSquare
		//if (m_crossword_list[idx_cw].graphical_data[row][col])
		//	m_crossword_list[idx_cw].graphical_data[row][col]->isKey = false;
	}
	
	bool isKeyLocal(int col = -1, int row = -1, int idx_cw = -1) { 
		KeyTile::crossword_key_t k;
		if (idx_cw == -1) idx_cw = (int)m_crossword_list.size() - 1;
		if (col == -1) col = current_tile.x;
		if (row == -1) row = current_tile.y;
		return 	m_crossword_list[idx_cw].data.isKey(row,col);
	
	}


private:
	
	std::vector<crossword_data_t> m_crossword_list;  // one image calibration set for each video that means for each distance

	GAME_DIRECTION::itype current_direction;

	cv::Point current_tile;

};
#endif  // MODULES_PARAMETERS_ARGSBASEJSON_H_
