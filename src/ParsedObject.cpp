#include <cstdlib>
#include <ctime>
#include "ParsedObject.hpp"

/*
parse v in vector
parse vt in vector
faces in vectors of struct {IofV, IofVt}

*/

struct Pos
{
	float x;
	float y;
	float z;
};

struct textPos
{
	float x;
	float y;
};

struct face
{
	unsigned int vtx;
	unsigned int txt;
	unsigned int nrm;
};


void ParsedObject::split(std::string& line, std::vector<std::string>& words, char div)
{
	size_t start = 0, end = 0;
	words.clear();
	while (end < line.size())
	{
		end = line.find(div, start);
		if (end != std::string::npos)
			words.push_back(line.substr(start, end - start));
		else 
			words.push_back(line.substr(start));
		start = end;
		while (start < line.size() && line[start] == div)
			start++;
	}
}


void addPos(std::vector<float>& arr, Pos& minPos, Pos& maxPos, float curX, float curY, float curZ)
{
	minPos.x = std::min(curX, minPos.x);
	minPos.y = std::min(curY, minPos.y);
	minPos.z = std::min(curZ, minPos.z);
	maxPos.x = std::max(curX, maxPos.x);
	maxPos.y = std::max(curY, maxPos.y);
	maxPos.z = std::max(curZ, maxPos.z);
	arr.push_back(curX);
	arr.push_back(curY);
	arr.push_back(curZ);
}

bool isLineValid(std::vector<std::string>& words)
{
	if (words.empty())
		return false;
	std::string& token = words[0];
	return token == "v" || token == "f" || token == "vt";
}

void ParsedObject::parseFile(ModelState& modelState, int i)
{
	std::string line;
	std::ifstream file(m_fileName);
	std::vector<std::string> words;
	std::vector<Pos> vertices;
	std::vector<textPos> text_coord;
	std::vector<face> faces;
	std::vector<std::string> face_words;
	std::vector<face> tmp_faces(4);
	float x, y, z;
	Pos minPos = {100000, 10000, 10000};
	Pos maxPos = {-10000, -10000, -10000};
	float countVert = 0;


	if (file.is_open() == false)
		return ;
	while (getline(file, line))
	{
		split(line, words, ' ');
		if (isLineValid(words))
		{
			if (words[0] == "v")
			{
				
				try
				{
					x = std::stof(words[1]);
					y = std::stof(words[2]);
					z = std::stof(words[3]);
				}
				catch(const std::exception& e)
				{
					std::cerr << e.what() << '\n';
					for (auto& w : words)
						std::cout << w << " ";
					std::cout << std::endl << std::endl;
				}
				vertices.push_back({x, y, z});
				countVert++;
				modelState.centerOffset.x += x;
				modelState.centerOffset.y += y;
				modelState.centerOffset.z += z;
			} 
			else if (words[0] == "vt")
			{
				try
				{
					x = std::stof(words[1]);
					y = std::stof(words[2]);
				}
				catch(const std::exception& e)
				{
					std::cerr << e.what() << '\n';
					for (auto& w : words)
						std::cout << w << " ";
					std::cout << std::endl << std::endl;
				}
				text_coord.push_back({x, y});
			}
			else if (words[0] == "f")
			{
				if (line.find('/') != std::string::npos) // v/t/n
				{
					int countFaces = 0;
					for (auto& word : words)
					{
						split(word, face_words, '/');
						if (face_words.size() == 2) // n/n
						{
							tmp_faces[countFaces].vtx = std::stoi(face_words[0]);
							tmp_faces[countFaces++].txt = std::stoi(face_words[1]);
						}
						if (face_words.size() == 3) // n/n/n
							tmp_faces[countFaces++].nrm = std::stoi(face_words[2]);
					}
					if (countFaces == 3) 
					{
						for (auto& f : tmp_faces)
							faces.push_back(f);
					}
					else if (countFaces == 4) 
					{
						faces.push_back(tmp_faces[0]);
						faces.push_back(tmp_faces[1]);
						faces.push_back(tmp_faces[2]);
						faces.push_back(tmp_faces[0]);
						faces.push_back(tmp_faces[2]);
						faces.push_back(tmp_faces[3]);
					}
				}
				else  // default. not / / / 
				{
					face f = {0,0,0};
					if (words.size() == 4) // triangle 
					{
						for (int i = 1; i < words.size(); i++)
						{
							f.vtx =  std::stoi(words[i]);
							// TODO TXT 
							faces.push_back(f);
						}
					}
					else if (words.size() == 5)
					{
						f.vtx =  std::stoi(words[1]);
						faces.push_back(f);
						f.vtx =  std::stoi(words[2]);
						faces.push_back(f);
						f.vtx =  std::stoi(words[3]);
						faces.push_back(f);
						f.vtx =  std::stoi(words[1]);
						faces.push_back(f);
						f.vtx =  std::stoi(words[3]);
						faces.push_back(f);
						f.vtx =  std::stoi(words[4]);
						faces.push_back(f);
					}
				}
			}
		}
	}
	for (auto f : faces)
	{
		m_positions.push_back(vertices[f.vtx - 1].x);
		m_positions.push_back(vertices[f.vtx - 1].y);
		m_positions.push_back(vertices[f.vtx - 1].z);
		if (f.txt != 0) // textures
		{
			m_positions.push_back(text_coord[f.txt - 1].x);
			m_positions.push_back(text_coord[f.txt - 1].y);
		}
		else 
		{
			m_positions.push_back(0); // TODO texture calculate
			m_positions.push_back(0);
		}
	}
	modelState.centerOffset /= countVert;
}



void ParsedObject::parseFile(ModelState& modelState)
{
	std::string line;
	std::ifstream file(m_fileName);
	std::vector<std::string> words;

	Pos minPos = {100000, 10000, 10000};
	Pos maxPos = {-10000, -10000, -10000};
	float x, y, z;
	float countVert = 0;

	if (file.is_open() == false)
		return ;
	while (getline(file, line))
	{
		// TODO add try catch
		split(line, words, ' ');
		if (isLineValid(words))
		{
			if (words[0] == "v")
			{
				countVert++;
				try
				{
					x = std::stof(words[1]);
					y = std::stof(words[2]);
					z = std::stof(words[3]);
				}
				catch(const std::exception& e)
				{
					std::cerr << e.what() << '\n';
					for (auto& w : words)
						std::cout << w << " ";
					std::cout << std::endl;
				}
				
				modelState.centerOffset.x += x;
				modelState.centerOffset.y += y;
				modelState.centerOffset.z += z;
				addPos(m_positions, minPos, maxPos, x, y, z);
				for (int i = 0; i < 2; i++)
					m_positions.push_back(0);
			}
			if (words[0] == "f")
			{
				if (line.find('/') != std::string::npos)
					addFaceSet(words);
				else
					addFaceDefault(words);
			}
		}
	}
	float sizeX = maxPos.x - minPos.x;
	float sizeY = maxPos.y - minPos.y;
	float sizeZ = maxPos.z - minPos.z;
	float u, v;
	for (int i = 3; i < m_positions.size(); i += 5)
	{
		x = m_positions[i - 3];
		y = m_positions[i - 2];

		float u = (x - minPos.x) / sizeX;
        float v = (y - minPos.y) / sizeY;
		m_positions[i] = v;
		m_positions[i + 1] = u;
	}
	modelState.centerOffset /= countVert;
	// std::cout << modelState.centerOffset.x << " " << modelState.centerOffset.y << " " << modelState.centerOffset.z << std::endl;
}

// v/vt/vn
void ParsedObject::addFaceDefault( std::vector<std::string>& words)
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
void ParsedObject::addFaceSet( std::vector<std::string>& words)
{
	if (words.size() == 4)
	{

	} else if (words.size() == 5)
	{

	}
}


ParsedObject::ParsedObject(std::string fileName, ModelState& modelState) : m_fileName(fileName)
{
	parseFile(modelState, 1);
}
