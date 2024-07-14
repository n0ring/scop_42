#include <cstdlib>
#include <ctime>
#include "ParsedObject.hpp"

/*
parse v in vector
parse vt in vector
faces in vectors of struct {IofV, IofVt}

*/

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
	return token == "v" || token == "f" || token == "vt" || token == "vn";
}

bool ParsedObject::addVertex(std::vector<std::string>& words, std::vector<Pos>& vertices, ModelState& modelState)
{
	try
	{
		tmp_x = std::stof(words[kIdxXData]);
		tmp_y = std::stof(words[kIdxYData]);
		tmp_z = std::stof(words[kIdxZData]);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		for (auto& w : words)
			std::cout << w << " ";
		std::cout << std::endl << std::endl;
		return false;
	}
	vertices.push_back({tmp_x, tmp_y, tmp_z});
	modelState.centerOffset.x += tmp_x;
	modelState.centerOffset.y += tmp_y;
	modelState.centerOffset.z += tmp_z;
	return true;
}

bool ParsedObject::addNormals(std::vector<std::string>& words, std::vector<Pos>& normals)
{
	try
	{
		tmp_x = std::stof(words[kIdxXData]);
		tmp_y = std::stof(words[kIdxYData]);
		tmp_z = std::stof(words[kIdxZData]);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		for (auto& w : words)
			std::cout << w << " ";
		std::cout << std::endl << std::endl;
		return false;
	}
	normals.push_back({tmp_x, tmp_y, tmp_z});
	return true;
}

bool ParsedObject::addTextureCoord(std::vector<std::string>& words, std::vector<textPos>& text_coord)
{
	try
	{
		tmp_x = std::stof(words[1]);
		tmp_y = std::stof(words[2]);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		for (auto& w : words)
			std::cout << w << " ";
		std::cout << std::endl << std::endl;
		return false;
	}
	text_coord.push_back({tmp_x, tmp_y});
	return true;
}

bool ParsedObject::addComplexFace(std::vector<std::string>& words, std::vector<face>& faces)
{
	int countFaces = 0;

	
	for (auto& word : words)
	{
		split(word, m_faceWords, '/');
		try
		{
			if (m_faceWords.size() == 2 || m_faceWords.size() == 3)
			{
				if (m_faceWords.size() > 1) // n/n
				{
					m_tmpFaces[countFaces].vtx = std::stoi(m_faceWords[0]);
					m_tmpFaces[countFaces].txt = std::stoi(m_faceWords[1]);
				}
				if (m_faceWords.size() > 2) // n/n/n
					m_tmpFaces[countFaces].nrm = std::stoi(m_faceWords[2]);
				countFaces++;
			}
				
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
			std::cout << "Error with face: " << word << std::endl;
			return false;
		}
	}
	if (countFaces == kTriangleCount) // no need to mix faces 
	{
		for (auto& f : m_tmpFaces)
			faces.push_back(f);
	}
	else if (countFaces == kSquareCount) // need to mix in triangle 
	{
		faces.push_back(m_tmpFaces[0]);
		faces.push_back(m_tmpFaces[1]);
		faces.push_back(m_tmpFaces[2]);
		faces.push_back(m_tmpFaces[0]);
		faces.push_back(m_tmpFaces[2]);
		faces.push_back(m_tmpFaces[3]);
	}
	return true;
}

bool ParsedObject::addSimpleFace(std::vector<std::string>& words, std::vector<face>& faces)
{
	if (words.size() - 1 == kTriangleCount) // triangle // -1 for f in line
	{
		for (int i = 1; i < words.size(); i++)
		{
			m_tmpFace.vtx =  std::stoi(words[i]);
			// TODO TXT 
			faces.push_back(m_tmpFace);
		}
	}
	else if (words.size() - 1 == kSquareCount) // -1 for f in line
	{
		m_tmpFace.vtx =  std::stoi(words[1]);
		faces.push_back(m_tmpFace);
		m_tmpFace.vtx =  std::stoi(words[2]);
		faces.push_back(m_tmpFace);
		m_tmpFace.vtx =  std::stoi(words[3]);
		faces.push_back(m_tmpFace);
		m_tmpFace.vtx =  std::stoi(words[1]);
		faces.push_back(m_tmpFace);
		m_tmpFace.vtx =  std::stoi(words[3]);
		faces.push_back(m_tmpFace);
		m_tmpFace.vtx =  std::stoi(words[4]);
		faces.push_back(m_tmpFace);
	}
	return true;
}


void ParsedObject::parseFile(ModelState& modelState)
{
	std::string line;
	std::ifstream file(m_fileName);
	std::vector<std::string> words;
	std::vector<Pos> vertices;
	std::vector<textPos> text_coord;
	std::vector<face> faces;
	std::vector<Pos> normals;
	// float x, y, z;
	Pos minPos = {100000, 10000, 10000};
	Pos maxPos = {-10000, -10000, -10000};
	float countVert = 0;

	int debug_count = 0;
	if (file.is_open() == false)
	{
		std::cout << "File error: " << m_fileName << std::endl;
		return ;
	}
	while (getline(file, line))
	{
		split(line, words, ' ');
		if (isLineValid(words))
		{
			if (words[kIdxTypeData] == kVertexData) // VERTEX
			{
				m_ParseStatus = addVertex(words, vertices, modelState);
				countVert++;
			}
			else if (words[kIdxTypeData] == kTextureCoordData) // TEXTURES
			{
				m_ParseStatus = addTextureCoord(words, text_coord);
			}
			else if (words[kIdxTypeData] == kFaceData) // FACES
			{
				if (line.find('/') != std::string::npos) // v/t/n
				{
					m_ParseStatus = addComplexFace(words, faces);
				}
				else
				{
					m_ParseStatus = addSimpleFace(words, faces);
				}
			}
			else if (words[kIdxTypeData] == kNormalData) // NORMALS
			{
				if (!modelState.hasNormals)
					modelState.hasNormals = true;
				m_ParseStatus = addNormals(words, normals);
			}

			if (m_ParseStatus == false)
				return ;
		}
	}
	std::cout << "Faces count: " << faces.size() << std::endl;
	std::cout << "Vertices count: " << vertices.size() << std::endl;
	std::cout << "Text_coord count: " << text_coord.size() << std::endl;
	std::cout << "Normals count: " << normals.size() << std::endl;
	for (auto f : faces)
	{
		if (f.vtx)
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
				float theta = atan2(vertices[f.vtx - 1].z, vertices[f.vtx - 1].x);
				float phi =
				acos(vertices[f.vtx - 1].y / sqrt(vertices[f.vtx - 1].x * vertices[f.vtx - 1].x 
					+ vertices[f.vtx - 1].y * vertices[f.vtx - 1].y 
					+ vertices[f.vtx - 1].z * vertices[f.vtx - 1].z));
				m_positions.push_back( (theta + M_PI) / (2.0f * M_PI)); // TODO texture calculate
				m_positions.push_back(phi / M_PI);
			}
			if (f.nrm)
			{

				m_positions.push_back(normals[f.nrm - 1].x);
				m_positions.push_back(normals[f.nrm - 1].y);
				m_positions.push_back(normals[f.nrm - 1].z);
			}
			else 
			{
				m_positions.push_back(0);
				m_positions.push_back(0);
				m_positions.push_back(0);
			}
		}
	}
	std::cout << "offset: " << modelState.centerOffset.x << " " << modelState.centerOffset.y << " "<< modelState.centerOffset.z << std::endl; 
	std::cout << "countVert: " << countVert << std::endl;
	if (countVert)
		modelState.centerOffset /= countVert;
	std::cout << "offset after div: " << modelState.centerOffset.x << " " << modelState.centerOffset.y << " "<< modelState.centerOffset.z  << std::endl; 
}
void ParsedObject::generateIndeces()
{
	vertex vtx;
	std::unordered_map<vertex, int, vertex_hash> mp; // [vtx, idx]
	int idx = 0;
	// every 5 vtx
	for (int i = 7; i < m_positions.size(); i += 8)
	{
		vtx.x = m_positions[i - 7];
		vtx.y = m_positions[i - 6];
		vtx.z = m_positions[i - 5];
		vtx.u = m_positions[i - 4];
		vtx.v = m_positions[i - 3];
		vtx.vnx = m_positions[i - 2];
		vtx.vny = m_positions[i - 1];
		vtx.vnz = m_positions[i];
		
		if (mp.count(vtx))
		{
			m_indices.push_back(mp[vtx]);
		}
		else 
		{
			mp[vtx] = idx;
			m_indices.push_back(idx);
		}
		idx++;
	}
}

// 0 1 2 3 4 5 6 7
// 8 9 0 1 2 3 4 5
// 6 7 8 9 0 1 2 3 

void ParsedObject::generateNormals()
{
	for (int i = 23; i < m_positions.size(); i += 24)
	{
		// sides						x						y				z
		glm::vec3 v0 = glm::vec3(m_positions[i - 23], m_positions[i - 22], m_positions[i - 21]); 
		glm::vec3 v1 = glm::vec3(m_positions[i - 15], m_positions[i - 14], m_positions[i - 13]); 
		glm::vec3 v2 = glm::vec3(m_positions[i - 7], m_positions[i - 6], m_positions[i - 5]); 

		glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;
		glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));
		m_positions[i - 18] = normal.x;
		m_positions[i - 17] = normal.y;
		m_positions[i - 16] = normal.z;

		m_positions[i - 10] = normal.x;
		m_positions[i - 9] = normal.y;
		m_positions[i - 8] = normal.z;

		m_positions[i - 2] = normal.x;
		m_positions[i - 1] = normal.y;
		m_positions[i - 0] = normal.z;
	}
}

ParsedObject::ParsedObject(std::string fileName, ModelState& modelState) : m_fileName(fileName), m_tmpFaces(4)
{
	parseFile(modelState);
	if (m_ParseStatus)
	{
		if (modelState.hasNormals == false)
		{
			generateNormals();
			modelState.hasNormals = true;
		}
		generateIndeces();
		std::cout << "Log: Parse done for file: " << fileName << ". Verticies: " << m_positions.size() << " indeces: " << m_indices.size() << std::endl;
	}
}
