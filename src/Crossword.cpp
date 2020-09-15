#include "Crossword.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "JSON.h"


int findOneOf(std::wstring & json, int pos, std::wstring &out)
{
	bool found = false;
	int cont = pos;
	int loc = -1;

	while (!found)
	{
		wchar_t c = json.at(cont);
		if (c == L'{' || c == L']' || c == L'[' || c == L'}' || c == L':' || c == L',')
		{
			loc = cont;
			found = true;
			out = c;
		}
		if (!found) cont++;

	}
	if (found)
		return cont;
	else return -1;
}

bool closing(std::wstring &str0, std::wstring &str1)
{
	if (str0 == L'{' && str1 == L'}') return true;
	if (str0 == L'[' && str1 == L']') return true;
	return false;
}

std::wstring findNext(std::wstring& json, std::map<int, std::vector <CrossWord::nodo_json > > &list, int &id_nodo, int parent_id, int parent_level, std::wstring &current_token, int pos, int &pos_out)
{

	// to be done recursively
	if (pos > json.length())
	{
		pos_out = -1;
		return std::wstring();
	}
	int this_level = parent_level;


	std::wstring next_token;
	pos_out = findOneOf(json, pos, next_token);

	// Errors
	if ((current_token == L':' && next_token == L'}') ||
		(current_token == L'}' && next_token == L'}') ||
		(current_token == L']' && next_token == L'}'))
	{
		return std::wstring();
	}
	if ((current_token == L',' && next_token == L'}') ||
		(current_token == L':' && next_token == L']') ||
		(current_token == L',' && next_token == L']'))
	{
		std::cout << "Format error." << std::endl;
		return std::wstring();
	}

	if (current_token == L'}' && next_token == L',' || current_token == L']' && next_token == L',')
	{
		int pout1;
		findNext(json, list, id_nodo, id_nodo, this_level, next_token, pos_out + 1, pout1);
	}

	else if (current_token == L':' && next_token == L',')
	{
		list[this_level][id_nodo].rest = json.substr(pos, pos_out - pos);
	}
	else if ((current_token == L':' && next_token == L'{') || (current_token == L':' && next_token == L'[') ||
		(current_token == L'{' && next_token == L'[') || (current_token == L'[' && next_token == L'{') ||
		(current_token == L'[' && next_token == L'[') || (current_token == L'{' && next_token == L'{'))
	{
		this_level = parent_level + 1;
		/*bool found = false;
		std::wstring first_token, out_token;
		first_token = next_token;
		while (!found)
		{
		out_token = findNext(json, list, id_nodo, id_nodo, this_level, next_token, pos, pout1);

		found = closing(first_token, out_token);
		if (!found)
		{


		}
		}

		if (found)
		{
		}*/

	}
	else if (current_token == L'{' && next_token == L':')
	{

		list[this_level].push_back(CrossWord::nodo_json(parent_id, parent_level, json.substr(pos, pos_out - pos)));
		id_nodo = list[this_level].size() - 1;
	}
	else if (current_token == L',' && next_token == L':')
	{
		list[this_level].push_back(CrossWord::nodo_json(parent_id, parent_level, json.substr(pos, pos_out - pos)));
		id_nodo = list[this_level].size() - 1;

	}
	int pout1;

	findNext(json, list, id_nodo, id_nodo, this_level, next_token, pos_out + 1, pout1);

	return std::wstring();

}





CrossWord::CrossWord()
{
}
CrossWord::~CrossWord()
{

}

CrossWord::crossword_description_t &CrossWord::getDescription()
{
	return m_crossword_description;
}


KeyTile * CrossWord::getKeySquare(int irow, int icol)
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
	{
		LOG_SCOPE_F(INFO, "Loading Crossword: ...");
		LOG_F(INFO, "Size: %d x %d", desc.rows, desc.cols);
		LOG_F(INFO, "Topic : %s ", desc.topic.c_str());
		LOG_F(INFO, "Units : %s ", UNITS::to_string(desc.unit).c_str());
	}

	m_crossword_content.clear();
	m_crossword_description = desc;

	m_crossword_content.resize(desc.rows);
	for (int i = 0; i < desc.rows; i++)
	{
		m_crossword_content[i].resize(desc.cols);
		for (int j = 0; j < desc.cols; j++)
		{
			m_crossword_content[i][j] = NULL; // new NormalTile();
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
		// load from nlohmann json (string)
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
			cdesc.unit = UNITS::to_itype(data["crossword"]["description"]["unit"].get<std::string>());
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
								kt.topic = definition.value()["topic"].get<bool>();

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
		std::map<int, std::vector <CrossWord::nodo_json > > indentations;


		// load from json (wstring)
		std::wstring file_content = ext::string::readUTF8File(_json_file_name.c_str());
		int pos = file_content.find(L'{');
		std::wstring first = L"{";
		int pout;
		int level = 0;   // this is the first index of the indentations matrix
		int id = 0; // this is the second index of the indentations matrix
		findNext(file_content, indentations,id,id, level, first, pos+1,pout);
		





	}
	else if (format == 2)
	{
		std::wstring file_content = ext::string::readUTF8File(_json_file_name.c_str());
		JSONValue *value = JSON::Parse(file_content.c_str());
		if (!value || value->IsObject() == false)
		{
			std::cout << "The root element is not an object, did you change the example?\r\n" << std::endl;
		}
		else
		{
			JSONObject root;
			root = value->AsObject();

			// Retrieving a string
			if (root.find(L"crossword") != root.end() && root[L"crossword"]->IsObject())
			{
				crossword_description_t cdesc;
				JSONObject crossword;
				crossword = root[L"crossword"]->AsObject();
				if (crossword.find(L"description") != crossword.end() && crossword[L"description"]->IsObject())
				{
					JSONObject description;
					description = crossword[L"description"]->AsObject();
					
					cdesc.cols = description[L"cols"]->AsNumber();
					cdesc.rows = description[L"rows"]->AsNumber();
					cdesc.title = description[L"title"]->AsString();
					cdesc.topic = description[L"topic"]->AsString();
					cdesc.unit = UNITS::to_itype(description[L"unit"]->AsString());
					clear();
					createCrosswordData(cdesc);

					
				}

				if (crossword.find(L"content") != crossword.end() && crossword[L"content"]->IsObject())
				{
					JSONObject content;
					content = crossword[L"content"]->AsObject();
					for (int i = 0; i < cdesc.rows; i++)
					{
						for (int j = 0; j < cdesc.cols; j++)
						{
							if (content.find(std::to_wstring(i)) != content.end() && content[std::to_wstring(i)]->IsObject())
							{
								JSONObject row;
								row = content[std::to_wstring(i)]->AsObject();

								if (row.find(std::to_wstring(j)) != row.end() && row[std::to_wstring(j)]->IsArray())
								{
									int count = 0;
									JSONArray  col;
									col = row[std::to_wstring(j)]->AsArray();
									for (unsigned int def_idx = 0; def_idx < col.size(); def_idx++)
									{
										JSONObject  def_item;
										def_item = col[def_idx]->AsObject();

										KeyTile::crossword_key_t kt;
										count++;
										kt.first_point = ext::string::wstring_to_string(def_item[L"first_point"]->AsString());
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

										kt.direction = ext::string::wstring_to_string(def_item[L"direction"]->AsString());
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

										kt.def = def_item[L"def"]->AsString();
										if (def_item.find(L"short_def") != def_item.end() && def_item[L"short_def"]->IsString())
											kt.short_def = def_item[L"short_def"]->AsString();
										if (def_item.find(L"topic") != def_item.end() && def_item[L"topic"]->IsBool())
											kt.topic = def_item[L"topic"]->AsBool();

										if (def_item.find(L"answer") != def_item.end() && def_item[L"answer"]->IsArray())
										{
											JSONArray  answerList;
											answerList = def_item[L"answer"]->AsArray();
											int nwords = answerList.size();

											for (unsigned int answeridx = 0; answeridx < answerList.size(); answeridx++)
											{
												std::wstring answer_item = answerList[answeridx]->AsString();
												kt.answers.push_back(answer_item);
											}
										}


										if (def_item.find(L"clues") != def_item.end() && def_item[L"clues"]->IsArray())
										{
											JSONArray  cluesList;
											cluesList = def_item[L"clues"]->AsArray();
											int nwords = cluesList.size();

											for (unsigned int clueidx = 0; clueidx < cluesList.size(); clueidx++)
											{
												std::wstring clue_item = cluesList[clueidx]->AsString();
												kt.clues.push_back(clue_item);
											}
										}

										this->addKey(i, j, kt);


									}
								}
							}
						}
					}
					// Once we have set key tiles let's fill non-key tiles content
					fillNormalTilesFromKeyTiles();
				}
			}
				
		}
	}
}

void CrossWord::saveCrosswordData(std::string _json_file_name, int format)
{
	fillKeyTilesFromNormalTiles();
	std::ofstream o;

	if (format == 0)
	{ // using json
		o.open(_json_file_name.c_str(), std::ios::out);
		//o << std::setw(4) << m_data << std::endl;
	}
	else
	{  //wstring

		o.open(_json_file_name.c_str(), std::ios::out | std::ios::binary);
		// save description
		o << "{  " << std::endl;
		o << "     \"crossword\":" << std::endl;
		o << "      {" << std::endl;
		o << "          \"description\":" << std::endl;
		o << "            {" << std::endl;
		o << "					 \"title\":" << "\"" << ext::string::wstring_to_utf8(m_crossword_description.title) << "\"," << std::endl;
		o << "					 \"cols\":" << "" << std::to_string(m_crossword_description.cols) << "," << std::endl;
		o << "					 \"rows\":" << "" << std::to_string(m_crossword_description.rows) << "," << std::endl;
		o << "					 \"topic\":" << "\"" << ext::string::wstring_to_utf8(m_crossword_description.topic) << "\"," << std::endl;
		o << "					 \"unit\":" << "\"" << UNITS::to_string(m_crossword_description.unit) << "\"" << std::endl;
		o << "            }," << std::endl;
		o << "          \"content\":" << std::endl;
		o << "            {" << std::endl;
		bool first_row_already_written = false;
		for (int i = 0; i < (int)m_crossword_content.size(); i++)
		{
			if (first_row_already_written) o << "," << std::endl;
			first_row_already_written = true;
			o << "				 \"" << std::to_string(i) << "\":" << std::endl;
			o << "				{" << std::endl;

			bool first_col_already_written = false;
			
			for (int j = 0; j < (int)m_crossword_content[i].size(); j++)
			{
				if(m_crossword_content[i][j] == NULL) continue;
				KeyTile* kt = dynamic_cast<KeyTile*>(m_crossword_content[i][j]);
				if (kt)
				{
					if (first_col_already_written) o << "," << std::endl;
					o << "					\""<< std::to_string(j) << "\":" << std::endl;
					o << "					 [" << std::endl;

					
					first_col_already_written = true;

					int num_definitions = (int)kt->defs.size();
					for (int ndefs = 0; ndefs <num_definitions; ndefs++)
					{
						if (ndefs > 0) o << "," << std::endl;
					o << "						{" << std::endl;
						
					o << "							\"first_point\":" << "\"" << kt->defs[ndefs].first_point << "\"," << std::endl;
					o << "							\"direction\":" << "\"" << kt->defs[ndefs].direction << "\"," << std::endl;
					o << "							\"def\":" << "\"" << ext::string::wstring_to_utf8(kt->defs[ndefs].def) << "\"," << std::endl;
					o << "							\"short_def\":" << "\"" << ext::string::wstring_to_utf8(kt->defs[ndefs].short_def) << "\"," << std::endl;
					o << "							\"topic\":" << "\"" << ext::string::fromBool(kt->defs[ndefs].topic) << "\"," << std::endl;
					o << "							\"clue\":" << std::endl;
					o << "							[" << std::endl;
					int tam = (int)kt->defs[ndefs].clues.size();
					for (int k = 0; k < tam; k++)
						{
							if (k > 0) o << ", " << std::endl;
							o << "								\"" <<  ext::string::wstring_to_utf8(kt->defs[ndefs].clues[k]) << "\"";

						}
					if (tam > 0) o << std::endl;
					o << "							]," << std::endl;
					o << "							\"answer\":" << std::endl;
					o << "							[" << std::endl;
					tam = (int)kt->defs[ndefs].answers.size();
					for (int k = 0; k < tam; k++)
					{
						if (k > 0) o << ", " << std::endl;
						o << "								\"" << ext::string::wstring_to_utf8(kt->defs[ndefs].answers[k]) << "\"";

					}
					if (tam > 0) o << std::endl;
					o << "							]" << std::endl;
					o << "						}" ;
					}

					if (num_definitions > 0) o << std::endl;

					o << "					]";
					
				}
			}
			o << std::endl << "				}";

		}
		o << "            }" << std::endl;
		o << "      }" << std::endl;
		o << "}" << std::endl;

		// save content

	}
	o.close();

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


	//remove anchor info in normal tiles in current direction


	// delete key tile
	delete m_crossword_content[row][col];
	m_crossword_content[row][col] = NULL;


	return 1;
}


void CrossWord::fillNormalTilesFromKeyTiles()
{
	int rows = (int)m_crossword_content.size();
	for (int i = 0; i < rows; i++)
	{
		int cols = (int)m_crossword_content[i].size();
		for (int j = 0; j < cols; j++)
		{
			if (!m_crossword_content[i][j]) continue;
				
			KeyTile* kt = dynamic_cast<KeyTile*> (m_crossword_content[i][j]);
			if (kt)
			{
				int def_list_size = (int)kt->defs.size();
				for (int ndefs = 0; ndefs < def_list_size; ndefs++)
				{
					KeyTile::crossword_key_t &one_def = kt->defs[ndefs];

					// key square
					cv::Point first_point, last_point;
					std::string fp_aux = one_def.first_point;
					std::string dir_aux = one_def.direction;
					START_POSITION::itype fp = START_POSITION::to_itype(fp_aux);
					DIRECTION::itype dir = DIRECTION::to_itype(dir_aux);

					if (fp == START_POSITION::RIGHT) first_point = cv::Point(j + 1, i);
					else if (fp == START_POSITION::LEFT) first_point = cv::Point(j - 1, i);
					else if (fp == START_POSITION::TOP) first_point = cv::Point(j, i - 1);
					else if (fp == START_POSITION::BOTTOM) first_point = cv::Point(j, i + 1);
					else
						std::cout << "Error parsing" << std::endl;

					if (!m_crossword_content[first_point.y][first_point.x])
						m_crossword_content[first_point.y][first_point.x] = new NormalTile();
					NormalTile* nt = dynamic_cast<NormalTile*> (m_crossword_content[first_point.y][first_point.x]);
					if (nt)
					{
						// set arrow indicator
						if (fp == START_POSITION::RIGHT) nt->arrow_at_left = dir;
						else if (fp == START_POSITION::LEFT) nt->arrow_at_right = dir;
						else if (fp == START_POSITION::TOP) nt->arrow_at_bottom = dir;
						else if (fp == START_POSITION::BOTTOM) nt->arrow_at_top = dir;
						else
							std::cout << "Error parsing" << std::endl;
					}
					// iterate through squares
					cv::Point next_position = first_point;

					// calculate last point
					int nwords = one_def.answers.size();
					if (dir == DIRECTION::RIGHT) last_point = first_point + cv::Point(nwords, 0);
					else if (dir == DIRECTION::LEFT) last_point = first_point + cv::Point(-nwords, 0);
					else if (dir == DIRECTION::UP) last_point = first_point + cv::Point(0, -nwords);
					else if (dir == DIRECTION::DOWN) last_point = first_point + cv::Point(0, nwords);
					else
						std::cout << "Error parsing" << std::endl;
					for (int idanswer=0;idanswer<nwords;idanswer++)
					{
						if (next_position.y<0 || next_position.y>m_crossword_content.size() - 1 ||
							next_position.x<0 || next_position.x>m_crossword_content[next_position.y].size() - 1) continue;

						if (!m_crossword_content[next_position.y][next_position.x])
							m_crossword_content[next_position.y][next_position.x] = new NormalTile();
						NormalTile* nextPtile = dynamic_cast<NormalTile*> (m_crossword_content[next_position.y][next_position.x]);
						if (nextPtile)
						{
							// for each answer set text for each square belonging to the answer
							//	std::cout << "key: " << el.value() << '\n';
							std::wstring aux_text = one_def.answers[idanswer];
							if (!nextPtile->text.empty() && aux_text.compare(nextPtile->text))
							{
								cv::Point conflicting_key;
								if (dir == DIRECTION::RIGHT || dir == DIRECTION::LEFT)
									conflicting_key = nextPtile->vertical_key;
								else conflicting_key = nextPtile->horizontal_key;

								LOG_F(ERROR, " Error between tiles (%d, %d) and (%d, %d)", i, j, conflicting_key.y, conflicting_key.x);
								LOG_F(ERROR, "Textos diferentes %S vs %S", aux_text.c_str(), nextPtile->text.c_str());
							}
							nextPtile->text = aux_text;

							nextPtile->isValid = true;
							nextPtile->type = TILETYPE::NORMAL;

							if (dir == DIRECTION::RIGHT || dir == DIRECTION::LEFT)
							{
								nextPtile->horizontal_key = cv::Point(j, i);
								nextPtile->first_horizontal_tile = first_point;
								nextPtile->last_horizontal_tile = last_point;

							}
							if (dir == DIRECTION::UP || dir == DIRECTION::DOWN)
							{
								nextPtile->vertical_key = cv::Point(j, i);
								nextPtile->first_vertical_tile = first_point;
								nextPtile->last_vertical_tile = last_point;
							}
						}


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
void CrossWord::fillKeyTilesFromNormalTiles()
{
	int rows = (int)m_crossword_content.size();
	for (int i = 0; i < rows; i++)
	{
		int cols = (int)m_crossword_content[i].size();
		for (int j = 0; j < cols; j++)
		{
			if (!m_crossword_content[i][j])
				continue;
			KeyTile* kt = dynamic_cast<KeyTile*> (m_crossword_content[i][j]);
			if (kt)
			{
				int def_list_size = (int)kt->defs.size();
				for (int ndefs = 0; ndefs < def_list_size; ndefs++)
				{
					KeyTile::crossword_key_t &one_def = kt->defs[ndefs];

					// key square
					cv::Point first_point, last_point;
					std::string fp_aux = one_def.first_point;
					std::string dir_aux = one_def.direction;
					START_POSITION::itype fp = START_POSITION::to_itype(fp_aux);
					DIRECTION::itype dir = DIRECTION::to_itype(dir_aux);

					if (fp == START_POSITION::RIGHT) first_point = cv::Point(j + 1, i);
					else if (fp == START_POSITION::LEFT) first_point = cv::Point(j - 1, i);
					else if (fp == START_POSITION::TOP) first_point = cv::Point(j, i - 1);
					else if (fp == START_POSITION::BOTTOM) first_point = cv::Point(j, i + 1);
					else
						std::cout << "Error parsing" << std::endl;

					
					// iterate through squares
					cv::Point next_position = first_point;

					// calculate last point
					int nwords = one_def.answers.size();
					if (dir == DIRECTION::RIGHT) last_point = first_point + cv::Point(nwords, 0);
					else if (dir == DIRECTION::LEFT) last_point = first_point + cv::Point(-nwords, 0);
					else if (dir == DIRECTION::UP) last_point = first_point + cv::Point(0, -nwords);
					else if (dir == DIRECTION::DOWN) last_point = first_point + cv::Point(0, nwords);
					else
						std::cout << "Error parsing" << std::endl;

					for (int idanswer = 0; idanswer<nwords; idanswer++)
					{
						if (next_position.y<0 || next_position.y>m_crossword_content.size() - 1 ||
							next_position.x<0 || next_position.x>m_crossword_content[next_position.y].size() - 1) continue;

						NormalTile* nextPtile = dynamic_cast<NormalTile*> (m_crossword_content[next_position.y][next_position.x]);
						if (nextPtile)
						{
							one_def.answers[idanswer]= nextPtile->text;
												
						}


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


std::wstring CrossWord::getText(int irow, int icol)
{
	if (irow < 0 || irow >= (int)m_crossword_content.size()) return std::wstring();
	if (icol < 0 || icol >= (int)m_crossword_content[irow].size()) return std::wstring();
	if (!m_crossword_content[irow][icol]) return std::wstring();

	KeyTile* kt = dynamic_cast<KeyTile*>(m_crossword_content[irow][icol]);
	if (kt)
		return kt->getText();
	else
	{
		return m_crossword_content[irow][icol]->getText();
	}
	
}

Tile *CrossWord::getTile(int irow, int icol)
{
	if (irow < 0 || irow >= (int)m_crossword_content.size()) return NULL;
	if (icol < 0 || icol >= (int)m_crossword_content[irow].size()) return NULL;

	return m_crossword_content[irow][icol];
}

void CrossWord::clearText(int irow, int icol)
{
	if (irow < 0 || irow >= (int)m_crossword_content.size()) return;
	if (icol < 0 || icol >= (int)m_crossword_content[irow].size()) return;

	m_crossword_content[irow][icol]->clearText();
}

void CrossWord::appendText(int irow, int icol,std::wstring t)
{
	if (irow < 0 || irow >= (int)m_crossword_content.size()) return;
	if (icol < 0 || icol >= (int)m_crossword_content[irow].size()) return;

	NormalTile* nt = dynamic_cast<NormalTile*>(m_crossword_content[irow][icol]);
	if (nt) nt->text += t;
}

void CrossWord::setText(int irow, int icol, std::wstring t)
{
	if (irow < 0 || irow >= (int)m_crossword_content.size()) return;
	if (icol < 0 || icol >= (int)m_crossword_content[irow].size()) return;

	NormalTile* nt = dynamic_cast<NormalTile*>(m_crossword_content[irow][icol]);
	if (nt) nt->text = t;
}

bool CrossWord::isKey(int irow, int icol)
{
	if (irow < 0 || irow >= (int)m_crossword_content.size()) return false;
	if (icol < 0 || icol >= (int)m_crossword_content[irow].size()) return false;

	Tile* t = m_crossword_content[irow][icol];
	if (t)
		return t->isKey();

	return false;
}

void CrossWord::updateNormalTilesFromKey(int irow, int icol)
{
	if (irow < 0 || irow >= (int)m_crossword_content.size()) return;
	if (icol < 0 || icol >= (int)m_crossword_content[irow].size()) return;

	KeyTile* kt = dynamic_cast<KeyTile*>(m_crossword_content[irow][icol]);
	if (kt)
	{
		for (int i = 0; i < kt->defs.size(); i++)
		{
			KeyTile::crossword_key_t & one_def = kt->defs[i];
			cv::Point first_point, last_point;
			std::string fp_aux = one_def.first_point;
			std::string dir_aux = one_def.direction;
			START_POSITION::itype fp = START_POSITION::to_itype(fp_aux);
			DIRECTION::itype dir = DIRECTION::to_itype(dir_aux);

			if (fp == START_POSITION::RIGHT) first_point = cv::Point(icol + 1, irow);
			else if (fp == START_POSITION::LEFT) first_point = cv::Point(icol - 1, irow);
			else if (fp == START_POSITION::TOP) first_point = cv::Point(icol, irow - 1);
			else if (fp == START_POSITION::BOTTOM) first_point = cv::Point(icol, irow + 1);
			else
				std::cout << "Error parsing" << std::endl;

			if (first_point.y<0 || first_point.x<0 ||
				first_point.y >(int)m_crossword_content.size() ||
				first_point.x >(int)m_crossword_content[first_point.y].size())
			{
				std::cout << "first point out of margins" << std::endl;
				return;
			}

			if (!m_crossword_content[first_point.y][first_point.x])
			{
				m_crossword_content[first_point.y][first_point.x] = new NormalTile();
			}

			NormalTile* nt = dynamic_cast<NormalTile*>(m_crossword_content[first_point.y][first_point.x]);

			if (nt)
			{
				// set arrow indicator
				if (fp == START_POSITION::RIGHT) nt->arrow_at_left = dir;
				else if (fp == START_POSITION::LEFT) nt->arrow_at_right = dir;
				else if (fp == START_POSITION::TOP) nt->arrow_at_bottom = dir;
				else if (fp == START_POSITION::BOTTOM) nt->arrow_at_top = dir;
				else
					std::cout << "Error parsing" << std::endl;

			}
			// iterate through tiles belonging this definition
			cv::Point next_position = first_point;

			if (!one_def.answers.empty())
			{

				// calculate last point
				int nwords = one_def.answers.size();

				//check number of words
				if (dir == DIRECTION::UP || dir == DIRECTION::DOWN)
				{
					int ntotalwordsforthisposition = (int)m_crossword_content.size() - first_point.y;
					if (nwords > ntotalwordsforthisposition)
					{
						nwords = ntotalwordsforthisposition;
						one_def.answers.clear();
						one_def.answers.resize(ntotalwordsforthisposition);

					}
				}
				else
				{
					int ntotalwordsforthisposition = (int)m_crossword_content[first_point.y].size() - first_point.x;

					if (nwords > ntotalwordsforthisposition)
					{
						nwords = ntotalwordsforthisposition;
						one_def.answers.clear();
						one_def.answers.resize(ntotalwordsforthisposition);

					}
				}


				if (dir == DIRECTION::RIGHT) last_point = first_point + cv::Point(nwords, 0);
				else if (dir == DIRECTION::LEFT) last_point = first_point + cv::Point(-nwords, 0);
				else if (dir == DIRECTION::UP) last_point = first_point + cv::Point(0, -nwords);
				else if (dir == DIRECTION::DOWN) last_point = first_point + cv::Point(0, nwords);
				else
					std::cout << "Error parsing" << std::endl;
				for (int nel = 0; nel < (int)one_def.answers.size(); nel++)
				{
					if (!m_crossword_content[next_position.y][next_position.x])
						m_crossword_content[next_position.y][next_position.x] = new NormalTile();

					NormalTile* nt = dynamic_cast<NormalTile*>(m_crossword_content[next_position.y][next_position.x]);
					if (nt)
					{
						if (next_position.y >= (int)m_crossword_content.size() || next_position.x >= (int)m_crossword_content[next_position.y].size()) break;

						// for each answer set text for each square belonging to the answer
						//	std::cout << "key: " << el.value() << '\n';
						nt->isValid = true;
						if (nt->type != TILETYPE::KEY)
							nt->type = TILETYPE::NORMAL;
						else
							std::cout << "trying to delete key tile" << std::endl;

						if (dir == DIRECTION::RIGHT || dir == DIRECTION::LEFT)
						{
							nt->horizontal_key = cv::Point(icol, irow);
							nt->first_horizontal_tile = first_point;
							nt->last_horizontal_tile = last_point;

						}
						if (dir == DIRECTION::UP || dir == DIRECTION::DOWN)
						{
							nt->vertical_key = cv::Point(icol, irow);
							nt->first_vertical_tile = first_point;
							nt->last_vertical_tile = last_point;
						}

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