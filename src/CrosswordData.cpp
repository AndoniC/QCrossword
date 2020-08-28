#include "CrosswordData.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>


CrossWordData::CrossWordData()
{

}
CrossWordData::~CrossWordData()
{

}

CrossWordData::crossword_desc_t CrossWordData::getDescription()
{
	crossword_desc_t res;
	
	if (contains(m_data, "crossword","description"))
	{
		nlohmann::json &j = m_data["crossword"]["description"];
		if (contains(j, "cols")) res.cols = j["cols"].get<int>();
		if (contains(j, "rows")) res.rows = j["rows"].get<int>();
		if (contains(j, "title")) res.title = j["title"].get<std::string>();
		if (contains(j, "topic")) res.topic = j["topic"].get<std::string>();
		if (contains(j, "unit")) res.unit = j["unit"].get<int>();
	}
	return res;
}


nlohmann::json CrossWordData::getKeySquare(int icol, int irow)
{
	if (contains(m_data, "crossword", "content"))
	{
		if (contains(m_data["crossword"]["content"], std::to_string(irow), std::to_string(icol)))
		{
			return m_data["crossword"]["content"][std::to_string(irow)][std::to_string(icol)];
		}
	}
	return nlohmann::json();
}

void CrossWordData::loadCrosswordData(std::string _json_file_name, int format)
{
	//get text from json
	std::string json_txt, line_aux;
	printf("reading json file...\n");
	std::ifstream myfile(_json_file_name.c_str());
	if (myfile.is_open()) {
		while (getline(myfile, line_aux)) {
			json_txt += line_aux;
		}
		myfile.close();
	}

	if (format == 0)
	{
		// parse parameters from file
		m_data = nlohmann::json::parse(json_txt);
		// check direction and position
		
	

		if (contains(m_data["crossword"]["description"], "cols") && contains(m_data["crossword"]["description"], "rows"))
		{

			int cols = m_data["crossword"]["description"]["cols"].get<int>();
			int rows = m_data["crossword"]["description"]["rows"].get<int>();
			if (contains(m_data["crossword"], "content"))
			{
				for (int i = 0; i < rows; i++)
				{
					for (int j = 0; j < cols; j++)
					{
						if (!contains(m_data["crossword"]["content"], std::to_string(i).c_str())) continue;
						if (!contains(m_data["crossword"]["content"][std::to_string(i).c_str()], std::to_string(j).c_str())) continue;

						nlohmann::json &json_node = m_data["crossword"]["content"][std::to_string(i).c_str()][std::to_string(j).c_str()];
						if (json_node.empty()) continue;
						int count = 0;
						for (auto& definition : json_node.items())
						{
							count++;
							std::string fp = definition.value()["first_point"].get<std::string>();
							if (fp.empty())
							{
								//LOG_F(ERROR, "First_point not detected");
							}
							else
							{
								START_POSITION::itype fp_aux = START_POSITION::to_itype(fp);
								if (fp_aux == START_POSITION::__COUNT)
									std::cout << "Error parsing " << i << " , " << j << " position, def " << count << std::endl;
								definition.value()["first_point"] = START_POSITION::to_string(fp_aux).c_str();
							}

							std::string dir = definition.value()["direction"].get<std::string>();
							if (dir.empty())
							{
								//LOG_F(ERROR, "First_point not detected");
							}
							else
							{
								DIRECTION::itype dir_aux = DIRECTION::to_itype(dir);
								if (dir_aux == DIRECTION::__COUNT)
									std::cout << "Error parsing " << i << " , " << j << " position, def " << count << std::endl;
								definition.value()["direction"] = DIRECTION::to_string(dir_aux).c_str();
							}
						}
					}
				}
			}
		}


	}
	else if (format == 1)
	{

	}

}

void CrossWordData::saveCrosswordData(std::string _json_file_name, int format)
{
	if (format == 0)
	{
		std::ofstream o(_json_file_name);
		o << std::setw(4) << m_data << std::endl;
		o.close();
	}
	else
	{

	}

}

void CrossWordData::createCrosswordData(crossword_desc_t &desc)
{
	if (!m_data.empty())
	{
		m_data.clear();
	}
	m_data["crossword"]["description"]["title"] = desc.title;
	m_data["crossword"]["description"]["cols"] = desc.cols;
	m_data["crossword"]["description"]["rows"] = desc.rows;
	m_data["crossword"]["description"]["topic"] = desc.topic;
	m_data["crossword"]["description"]["unit"] = desc.unit;

	std::cout << m_data["crossword"]["description"] << std::endl;
}
void CrossWordData::addKey(int row, int col, crossword_key_t &desc)
{
	if (!contains(m_data["crossword"]["content"], std::to_string(row), std::to_string(col)))
		m_data["crossword"]["content"][std::to_string(row)][std::to_string(col)] = nlohmann::json::array();
	

	nlohmann::json node;
	node["first_point"] = desc.first_point;
	node["direction"] = desc.direction;
	node["def"] = desc.def;
	node["topic"] = desc.topic;
	node["short_def"] = desc.short_def;
	
	node["clue"] = nlohmann::json::array();
	for (int i = 0; i < desc.clues.size(); i++)
		node["clue"].push_back(desc.clues[i]);

	node["answer"] = nlohmann::json::array();
	for (int i = 0; i < desc.answers.size(); i++)
		node["answer"].push_back(desc.answers[i]);


	m_data["crossword"]["content"][std::to_string(row)][std::to_string(col)].push_back(node);
}
void CrossWordData::removeKey(int row, int col)
{

	if (contains(m_data["crossword"]["content"], std::to_string(row), std::to_string(col)))
	{
		m_data["crossword"]["content"][std::to_string(row)][std::to_string(col)].clear();// = nlohmann::json::object();
	}

}

void CrossWordData::fillWithContent(std::vector<std::vector<anchor_points_t> > &content)
{
	int rows = (int)content.size();
	int cols = (int)content[0].size();

	if (contains(m_data["crossword"]["description"], "cols") && contains(m_data["crossword"]["description"], "rows"))
	{
		if (m_data["crossword"]["description"]["cols"].get<int>() == cols && m_data["crossword"]["description"]["rows"].get<int>() == rows)
		{
			if (contains(m_data["crossword"], "content"))
			{
				for (int i = 0; i < rows; i++)
				{
					for (int j = 0; j < cols; j++)
					{
						if (!contains(m_data["crossword"]["content"], std::to_string(i).c_str())) continue;
						if (!contains(m_data["crossword"]["content"][std::to_string(i).c_str()], std::to_string(j).c_str())) continue;

						nlohmann::json &json_node = m_data["crossword"]["content"][std::to_string(i).c_str()][std::to_string(j).c_str()];
						if (json_node.empty()) continue;

						for (auto& definition : json_node.items())
						{
							nlohmann::json &one_def = definition.value();
							// key tile: remove answers and rewrite them
							//one_def["answer"].clear();

							cv::Point first_point,last_point;
							std::string fp_aux = one_def["first_point"].get<std::string>();
							std::string dir_aux = one_def["direction"].get<std::string>();
							START_POSITION::itype fp = START_POSITION::to_itype(fp_aux);
							DIRECTION::itype dir = DIRECTION::to_itype(dir_aux);


							if (fp == START_POSITION::RIGHT) first_point = cv::Point(j + 1, i);
							else if (fp == START_POSITION::LEFT) first_point = cv::Point(j - 1, i);
							else if (fp == START_POSITION::TOP) first_point = cv::Point(j, i - 1);
							else if (fp == START_POSITION::BOTTOM) first_point = cv::Point(j, i + 1);
							else std::cout << "Error parsing" << std::endl;

							// calculate last point
							int nwords = one_def["answer"].size();
							
							if (dir == DIRECTION::RIGHT) last_point = first_point + cv::Point(nwords, 0);
							else if (dir == DIRECTION::LEFT) last_point = first_point + cv::Point(-nwords, 0);
							else if (dir == DIRECTION::UP) last_point = first_point + cv::Point(0, -nwords);
							else if (dir == DIRECTION::DOWN) last_point = first_point + cv::Point(0, nwords);
							else
								std::cout << "Error parsing" << std::endl;
													

							cv::Point next_position = first_point;
							one_def["answer"].clear();

							for (int i =0;i<nwords;i++)
							{
								one_def["answer"].push_back(content[next_position.y][next_position.x].text);

								if (dir == DIRECTION::RIGHT) next_position += cv::Point(1, 0);
								if (dir == DIRECTION::LEFT) next_position += cv::Point(-1, 0);
								if (dir == DIRECTION::UP) next_position += cv::Point(0, -1);
								if (dir == DIRECTION::DOWN) next_position += cv::Point(0, 1);
							}

						}

					}
				}
			}


		}
	}

}
