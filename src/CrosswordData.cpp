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