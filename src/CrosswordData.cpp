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
		m_data["crossword"]["content"][std::to_string(row)][std::to_string(col)] = nlohmann::json::object();
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
							std::string fp = one_def["first_point"].get<std::string>();
							std::string dir = one_def["direction"].get<std::string>();

							if (!fp.compare("right")) first_point = cv::Point(j + 1, i);
							if (!fp.compare("left")) first_point = cv::Point(j - 1, i);
							if (!fp.compare("up")) first_point = cv::Point(j, i - 1);
							if (!fp.compare("down")) first_point = cv::Point(j, i + 1);

							// calculate last point
							int nwords = one_def["answer"].size();
							if (!dir.compare("right")) last_point = first_point + cv::Point(nwords, 0);
							if (!dir.compare("left")) last_point = first_point + cv::Point(-nwords, 0);
							if (!dir.compare("up")) last_point = first_point + cv::Point(0, -nwords);
							if (!dir.compare("down")) last_point = first_point + cv::Point(0, nwords);

							cv::Point next_position = first_point;
							one_def["answer"].clear();

							for (int i =0;i<nwords;i++)
							{
								one_def["answer"].push_back(content[next_position.y][next_position.x].text);

								if (!dir.compare("right")) next_position += cv::Point(1, 0);
								if (!dir.compare("left")) next_position += cv::Point(-1, 0);
								if (!dir.compare("up")) next_position += cv::Point(0, -1);
								if (!dir.compare("down")) next_position += cv::Point(0, 1);
							}

						}

					}
				}
			}


		}
	}

}
