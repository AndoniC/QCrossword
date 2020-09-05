#include "Crossword.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>


CrossWord::CrossWord()
{
}
CrossWord::~CrossWord()
{

}

CrossWord::crossword_description_t CrossWord::getDescription()
{
	return m_crossword_description;
}


KeyTile * CrossWord::getKeySquare(int icol, int irow)
{
	if (irow >= 0 && irow < (int)m_crossword_content.size())
	{
		if (m_crossword_content[irow][icol] != NULL)
		{
			KeyTile *kt = dynamic_cast<KeyTile*>(m_crossword_content[irow][icol]);
			//if (kt->isValid)
			return kt;
		}
	}
	return NULL;
}

void CrossWord::createCrosswordData(crossword_description_t &desc)
{
	m_crossword_content.clear();
	m_crossword_description = desc;

	m_crossword_content.resize(desc.rows);
	for (int i = 0; i < desc.rows; i++)
	{
		m_crossword_content[i].resize(desc.cols);
		for (int j = 0; j < desc.cols; j++)
		{
			m_crossword_content[i][j] = NULL;
		}
	}
}






void CrossWord::loadCrosswordData(std::string _json_file_name, int format)
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
		nlohmann::json data = nlohmann::json::parse(json_txt);
		// check direction and position
		
		

		if (contains(data["crossword"]["description"], "cols") && contains(data["crossword"]["description"], "rows"))
		{

			crossword_description_t cdesc;
			cdesc.cols = data["crossword"]["description"]["cols"].get<int>();
			cdesc.rows = data["crossword"]["description"]["rows"].get<int>();
			cdesc.title = ext::string::string_to_wstring(data["crossword"]["description"]["title"].get<std::string>());
			cdesc.topic = ext::string::string_to_wstring(data["crossword"]["description"]["topic"].get<std::string>());
			cdesc.unit = UNITS::to_itype(data["crossword"]["description"]["units"].get<std::string>());
			clear();
			createCrosswordData(cdesc);

			if (contains(data["crossword"], "content"))
			{
				for (int i = 0; i < cdesc.rows; i++)
				{
					for (int j = 0; j < cdesc.cols; j++)
					{
						if (!contains(data["crossword"]["content"], std::to_string(i).c_str())) continue;
						if (!contains(data["crossword"]["content"][std::to_string(i).c_str()], std::to_string(j).c_str())) continue;

						nlohmann::json &json_node = data["crossword"]["content"][std::to_string(i).c_str()][std::to_string(j).c_str()];
						if (json_node.empty()) continue;
						int count = 0;
						for (auto& definition : json_node.items())
						{
							KeyTile::crossword_key_t kt;
							count++;
							kt.first_point = definition.value()["first_point"].get<std::string>();
							if (kt.first_point.empty())
							{
								LOG_F(ERROR, "First_point not detected");
							}
							else
							{
								START_POSITION::itype fp_aux = START_POSITION::to_itype(kt.first_point);
								if (fp_aux == START_POSITION::__COUNT)
									std::cout << "Error parsing " << i << " , " << j << " position, def " << count << std::endl;
								kt.first_point = START_POSITION::to_string(fp_aux).c_str();
							}

							kt.direction = definition.value()["direction"].get<std::string>();
							if (kt.direction.empty())
							{
								LOG_F(ERROR, "First_point not detected");
							}
							else
							{
								DIRECTION::itype dir_aux = DIRECTION::to_itype(kt.direction);
								if (dir_aux == DIRECTION::__COUNT)
									std::cout << "Error parsing " << i << " , " << j << " position, def " << count << std::endl;
								kt.direction = DIRECTION::to_string(dir_aux).c_str();
							}

							kt.def = ext::string::string_to_wstring(definition.value()["def"].get<std::string>());
							if (contains(definition.value(), "short_def"))
								kt.short_def = ext::string::string_to_wstring(definition.value()["short_def"].get<std::string>());
							if (contains(definition.value(), "topic"))
								kt.topic = ext::string::string_to_wstring(definition.value()["topic"].get<std::string>());

							if (contains(definition.value(), "answer"))
							{
								int nwords = definition.value()["answer"].size();
		
								for (auto& el : definition.value()["answer"].items())
								{
									std::string aux_text = el.value().get<std::string>();
									kt.answers.push_back(ext::string::string_to_wstring(aux_text));
								}
							}
							
							if (contains(definition.value(), "clues"))
							{
								int nwords = definition.value()["clues"].size();

								for (auto& el : definition.value()["clues"].items())
								{
									std::string aux_text = el.value().get<std::string>();
									kt.clues.push_back(ext::string::string_to_wstring(aux_text));
								}
							}
							
							this->addKey(i, j, kt);

							
						}
					}
				}

				// Once we have set key tiles let's fill non-key tiles content
				fillNormalTilesFromKeyTiles();
			}
		}


	}
	else if (format == 1)
	{

	}

}

void CrossWord::saveCrosswordData(std::string _json_file_name, int format)
{
	/*if (format == 0)
	{
		std::ofstream o(_json_file_name);
		o << std::setw(4) << m_data << std::endl;
		o.close();
	}
	else
	{

	}*/

}




int CrossWord::addKey(int row, int col, KeyTile::crossword_key_t &desc)
{
	if (row < 0 || row >= (int)m_crossword_content.size()) return -1;
	if (col < 0 || col >= (int)m_crossword_content[row].size()) return -1;

	if (m_crossword_content[row][col] == NULL)
		m_crossword_content[row][col] = new KeyTile(desc);
	else
	{
		KeyTile * kt = dynamic_cast<KeyTile*> (m_crossword_content[row][col]);
		if (kt)
			kt->push_back(desc);
	}

	return 1;
}


int CrossWord::removeKey(int row, int col)
{
	if (row < 0 || row >= (int)m_crossword_content.size()) return -1;
	if (col < 0 || col >= (int)m_crossword_content[row].size()) return -1;

	delete m_crossword_content[row][col];
	m_crossword_content[row][col] = NULL;


	return 1;
}


void CrossWord::fillNormalTilesFromKeyTiles()
{



}
void CrossWord::fillKeyTilesFromNormalTiles()
{

}