#include "ParsedObject.hpp"

void ParsedObject::split(std::string& line, std::vector<std::string>& words)
{
	std::string word;
	std::stringstream ss(line);
	words.clear();
	while (ss >> word)
	{
		words.push_back(word);
	}
}

void ParsedObject::parseFile()
{
	std::string line;
	std::ifstream file(m_fileName);
	std::vector<std::string> words;

	if (file.is_open() == false)
		return ;
	
	while (getline(file, line))
	{
		if (line[0] == 'v' || line[0] == 'f')
		{
			split(line, words);
			if (words[0] == "v")
			{
				m_positions.push_back(std::stof(words[1]));
				m_positions.push_back(std::stof(words[2]));
				m_positions.push_back(std::stof(words[3]));
			}
			if (words[0] == "f")
			{
				m_indices.push_back(std::stoi(words[1]) - 1);
				m_indices.push_back(std::stoi(words[2]) - 1);
				m_indices.push_back(std::stoi(words[3]) - 1);
			}
		}
	}
}

ParsedObject::ParsedObject(std::string fileName) : m_fileName(fileName)
{
	parseFile();
}
