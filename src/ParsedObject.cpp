#include <cstdlib>
#include <ctime>
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

struct Pos
{
	float x;
	float y;
};

void addPos(std::vector<float>& arr, Pos& minPos, Pos& maxPos, float curX, float curY)
{
	minPos.x = std::min(curX, minPos.x);
	minPos.y = std::min(curY, minPos.y);
	maxPos.x = std::max(curX, maxPos.x);
	maxPos.y = std::max(curY, maxPos.y);
	arr.push_back(curX);
	arr.push_back(curY);
}



void ParsedObject::parseFile(ModelState& modelState)
{
	std::string line;
	std::ifstream file(m_fileName);
	std::vector<std::string> words;
	Pos minPos = {100000, 10000};
	Pos maxPos = {-10000, -10000};
	float x, y, z;
	float countVert = 0;


	if (file.is_open() == false)
		return ;
	while (getline(file, line))
	{
		// TODO add try catch
		if (line[0] == 'v' || line[0] == 'f')
		{
			split(line, words);
			if (words[0] == "v")
			{
				countVert++;
				x = std::stof(words[1]);
				y = std::stof(words[2]);
				z = std::stof(words[3]);
				modelState.centerOffset.x += x;
				modelState.centerOffset.y += y;
				modelState.centerOffset.z += z;
				addPos(m_positions, minPos, maxPos, x, y);
				m_positions.push_back(z);
				for (int i = 0; i < 2; i++)
					m_positions.push_back(0);
			}
			if (words[0] == "f")
			{
				if (words.size() == 4)
				{
					m_indices.push_back(std::stoi(words[1]) - 1);
					m_indices.push_back(std::stoi(words[2]) - 1);
					m_indices.push_back(std::stoi(words[3]) - 1);
				} else if (words.size() == 5)
				{
					m_indices.push_back(std::stoi(words[1]) - 1);
					m_indices.push_back(std::stoi(words[2]) - 1);
					m_indices.push_back(std::stoi(words[3]) - 1);

					m_indices.push_back(std::stoi(words[1]) - 1);
					m_indices.push_back(std::stoi(words[3]) - 1);
					m_indices.push_back(std::stoi(words[4]) - 1);
				}
			}
		}
	}
	float sizeX = maxPos.x - minPos.x;
	float sizeY = maxPos.y - minPos.y;
	for (int i = 3; i < m_positions.size(); i += 5)
	{
		float x = m_positions[i - 3];
		float y = m_positions[i - 2];
		float u = (x - minPos.x) / sizeX;
        float v = (y - minPos.y) / sizeY;
		m_positions[i] = v;
		m_positions[i + 1] = u;
	}
	modelState.centerOffset /= countVert;
	std::cout << modelState.centerOffset.x << " " << modelState.centerOffset.y << " " << modelState.centerOffset.z << std::endl;
}

ParsedObject::ParsedObject(std::string fileName, ModelState& modelState) : m_fileName(fileName)
{
	parseFile(modelState);
}
