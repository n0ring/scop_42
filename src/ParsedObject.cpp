#include <cstdlib>
#include <ctime>
#include "ParsedObject.hpp"

/*
parse v in vector
parse vt in vector
faces in vectors of struct {IofV, IofVt}

*/

unsigned int getFace(std::string &word)
{
	return static_cast<unsigned int>(std::stoi(word) - 1);
}

void ParsedObject::split(std::string &line, std::vector<std::string> &words, char div)
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

bool isLineValid(std::vector<std::string> &words)
{
	if (words.empty())
		return false;
	std::string &token = words[0];
	return token == "v" || token == "f" || token == "vt" || token == "vn" || token == "usemtl" || token == "mtllib";
}

bool ParsedObject::addVertex(std::vector<std::string> &words, std::vector<Pos> &vertices, ModelState &modelState)
{
	try
	{
		tmpPos.x = std::stof(words[kIdxXData]);
		tmpPos.y = std::stof(words[kIdxYData]);
		tmpPos.z = std::stof(words[kIdxZData]);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
		for (auto &w : words)
			std::cout << w << " ";
		std::cout << std::endl
				  << std::endl;
		return false;
	}

	vertices.push_back(tmpPos);
	modelState.centerOffset.x += tmpPos.x;
	modelState.centerOffset.y += tmpPos.y;
	modelState.centerOffset.z += tmpPos.z;
	return true;
}

bool ParsedObject::addNormals(std::vector<std::string> &words, std::vector<Pos> &normals)
{
	try
	{
		tmpPos.x = std::stof(words[kIdxXData]);
		tmpPos.y = std::stof(words[kIdxYData]);
		tmpPos.z = std::stof(words[kIdxZData]);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
		for (auto &w : words)
			std::cout << w << " ";
		std::cout << std::endl
				  << std::endl;
		return false;
	}
	normals.push_back(tmpPos);
	return true;
}

bool ParsedObject::addTextureCoord(std::vector<std::string> &words, std::vector<textPos> &text_coord)
{
	try
	{
		tmpTexPos.x = std::stof(words[1]);
		tmpTexPos.y = std::stof(words[2]);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
		for (auto &w : words)
			std::cout << w << " ";
		std::cout << std::endl
				  << std::endl;
		return false;
	}
	text_coord.push_back(tmpTexPos);
	return true;
}

bool ParsedObject::addComplexFace(std::vector<std::string> &words, std::vector<face> &faces)
{
	int countFaces = 0;

	if (words.size() > 5)
	{
		std::cout << "incorrect file. Use only triangles please\n";
		return false;
	}
	for (auto &word : words)
	{
		split(word, m_faceWords, '/');
		try
		{
			if (m_faceWords.size() == 2 || m_faceWords.size() == 3)
			{
				if (m_faceWords.size() > 1) // n/n
				{
					m_tmpFaces[countFaces].vtx = getFace(m_faceWords[0]);
					m_tmpFaces[countFaces].txt = getFace(m_faceWords[1]);
				}
				if (m_faceWords.size() > 2) // n/n/n
					m_tmpFaces[countFaces].nrm = getFace(m_faceWords[2]);
				countFaces++;
			}
		}
		catch (const std::exception &e)
		{
			std::cerr << e.what() << '\n';
			std::cout << "Error with face: " << word << std::endl;
			return false;
		}
	}
	if (countFaces == kTriangleCount) // no need to mix faces
	{
		for (int i = 0; i < 3; i++)
			faces.push_back(m_tmpFaces[i]);
	}
	else if (countFaces == kSquareCount) // need to mix in triangle
	{
		if (m_tmpFaces.size() < 4)
			std::cout << "error with faces\n";
		faces.push_back(m_tmpFaces[0]);
		faces.push_back(m_tmpFaces[1]);
		faces.push_back(m_tmpFaces[2]);
		faces.push_back(m_tmpFaces[0]);
		faces.push_back(m_tmpFaces[2]);
		faces.push_back(m_tmpFaces[3]);
	}
	return true;
}

bool ParsedObject::addSimpleFace(std::vector<std::string> &words, std::vector<face> &faces)
{
	if (words.size() - 1 == kTriangleCount) // triangle // -1 for f in line
	{
		for (int i = 1; i < words.size(); i++)
		{
			m_tmpFace.vtx = getFace(words[i]);
			faces.push_back(m_tmpFace);
		}
	}
	else if (words.size() - 1 == kSquareCount) // -1 for f in line
	{
		m_tmpFace.vtx = getFace(words[1]);
		faces.push_back(m_tmpFace);
		m_tmpFace.vtx = getFace(words[2]);
		faces.push_back(m_tmpFace);
		m_tmpFace.vtx = getFace(words[3]);
		faces.push_back(m_tmpFace);
		m_tmpFace.vtx = getFace(words[1]);
		faces.push_back(m_tmpFace);
		m_tmpFace.vtx = getFace(words[3]);
		faces.push_back(m_tmpFace);
		m_tmpFace.vtx = getFace(words[4]);
		faces.push_back(m_tmpFace);
	}
	return true;
}

void ParsedObject::parseObjFile(ModelState &modelState)
{
	std::string line;
	std::ifstream file(m_objFileName);
	std::vector<std::string> words;
	std::vector<Pos> vertices;
	std::vector<textPos> text_coord;
	std::vector<face> faces;
	std::vector<Pos> normals;
	float countVert = 0;
	float mtlCount = 0;

	int debug_count = 0;
	if (file.is_open() == false)
	{
		std::cout << "File error: " << m_objFileName << std::endl;
		return;
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
			else if (words[kIdxTypeData] == kMtlData)
			{
				if (m_materialMap.count(words[kIdxMaterial]))
					m_curMtlIdx = m_materialMap[words[kIdxMaterial]];
				else
				{
					mtlCount++;
					m_materialMap[words[kIdxMaterial]] = mtlCount;
				}
				m_curMtlIdx = m_materialMap[words[kIdxMaterial]];
				m_tmpFace.mtl = m_curMtlIdx;
				tmpTexPos.mtl = m_curMtlIdx;
			}
			else if (words[kIdxTypeData] == kMtlFileData)
			{
				if (words.size() == 2)
				{
					m_mtlFileName = words[kIdxMtlFileName];
					m_ParseStatus = true;
				}
				else
				{
					std::cout << "mtllib has more then one argument\n";
					m_ParseStatus = false;
				}
			}
			if (m_ParseStatus == false)
				return;
		}
	}
	// std::cout << "Faces count: " << faces.size() << std::endl;
	// std::cout << "Vertices count: " << vertices.size() << std::endl;
	// std::cout << "Text_coord count: " << text_coord.size() << std::endl;
	// std::cout << "Normals count: " << normals.size() << std::endl;
	vertex tmpVtx;
	for (auto f : faces)
	{
		tmpVtx.x = vertices[f.vtx].x;
		tmpVtx.y = vertices[f.vtx].y;
		tmpVtx.z = vertices[f.vtx].z;

		if (!text_coord.empty()) // textures
		{
			tmpVtx.u = text_coord[f.txt].x;
			tmpVtx.v = text_coord[f.txt].y;
		}
		else
		{
			float theta = atan2(vertices[f.vtx].z, vertices[f.vtx].x);
			float phi =
				acos(vertices[f.vtx].y / sqrt(vertices[f.vtx].x * vertices[f.vtx].x + vertices[f.vtx].y * vertices[f.vtx].y + vertices[f.vtx].z * vertices[f.vtx].z));
			tmpVtx.u = (theta + M_PI) / (2.0f * M_PI);
			tmpVtx.v = phi / M_PI;
		}
		tmpVtx.vnx = normals.empty() ? 0 : normals[f.nrm].x;
		tmpVtx.vny = normals.empty() ? 0 : normals[f.nrm].y;
		tmpVtx.vnz = normals.empty() ? 0 : normals[f.nrm].z;
		tmpVtx.mtl = f.mtl ? f.mtl : vertices[f.vtx].mtl; // if face hasn't not default (0) then use from vtx
		m_positions.push_back(tmpVtx);
	}
	if (countVert)
		modelState.centerOffset = modelState.centerOffset / countVert;
	if (mtlCount)
		m_materials.resize(mtlCount + 1); // one [0] for default
}
void ParsedObject::generateIndeces()
{
	std::unordered_map<vertex, int, vertex_hash> mp; // [vtx, idx]
	for (int i = 0; i < m_positions.size(); i++)
	{
		if (mp.count(m_positions[i]))
		{
			m_indices.push_back(mp[m_positions[i]]);
		}
		else
		{
			mp[m_positions[i]] = i;
			m_indices.push_back(i);
		}
	}
}

void ParsedObject::generateNormals()
{
	for (int i = 2; i < m_positions.size(); i += 3)
	{
		// sides						x						y				z
		nrg::vec3 v0 = nrg::vec3(m_positions[i - 2].x, m_positions[i - 2].y, m_positions[i - 2].z);
		nrg::vec3 v1 = nrg::vec3(m_positions[i - 1].x, m_positions[i - 1].y, m_positions[i - 1].z);
		nrg::vec3 v2 = nrg::vec3(m_positions[i].x, m_positions[i].y, m_positions[i].z);

		nrg::vec3 edge1 = v1 - v0;
		nrg::vec3 edge2 = v2 - v0;
		nrg::vec3 normal = nrg::normalize(nrg::cross(edge1, edge2));
		m_positions[i - 2].vnx = m_positions[i - 1].vnx = m_positions[i].vnx = normal.x;
		m_positions[i - 2].vny = m_positions[i - 1].vny = m_positions[i].vny = normal.y;
		m_positions[i - 2].vnz = m_positions[i - 1].vnz = m_positions[i].vnz = normal.z;
	}
}

bool ParsedObject::addDataVec3(std::vector<std::string> &words, nrg::vec3 &v)
{
	if (words.size() == 4)
	{
		v.x = std::stof(words[1]);
		v.y = std::stof(words[2]);
		v.z = std::stof(words[3]);
	}
	else
	{
		std::cout << "Error with add data to " << words[0] << std::endl;
		return false;
	}
	return true;
}

bool ParsedObject::addDataF(std::vector<std::string> &words, float& f)
{
	if (words.size() == 2)
	{
		f = std::stof(words[1]);
	}
	else
	{
		std::cout << "Error with add data to " << words[0] << std::endl;
		return false;
	}
	return true;
}

std::string getPathToFile(std::string& filePath)
{
	int i = filePath.size() - 1;
	while (i >= 0 && filePath[i] != '/')
		i--;
	return filePath.substr(0, i + 1);
}

void ParsedObject::parseMtlFile()
{
	std::string line;
	std::vector<std::string> words;
	int curMtlIdx = 0; // default
	if (m_mtlFileName.empty())
	{
		std::cout << "no mtl file name in obj\n";
		return;
	}
	m_mtlFileName = getPathToFile(m_objFileName) + m_mtlFileName;
	std::ifstream file(m_mtlFileName);
	if (file.is_open() == false)
	{
		std::cout << "error with open file: " << m_mtlFileName << std::endl;
		return;
	}
	m_ParseStatus = true;

	while (getline(file, line))
	{
		split(line, words, ' ');
		if (!words.empty())
		{
			if ((words[kIdxTypeData] == kMtlNewMtlData))
			{
				if (words.size() == 2 && m_materialMap.count(words[kIdxMtlFileName]))
					curMtlIdx = m_materialMap[words[kIdxMtlFileName]];
				else
				{
					std::cout << "Material not exist: " << words[kIdxMtlFileName] << std::endl;
					m_ParseStatus = false;
				}
			}
			else if ((words[kIdxTypeData] == kMtlNsData) && curMtlIdx)
			{
				m_materials[curMtlIdx].Ns = std::stof(words[1]);
			}
			else if ((words[kIdxTypeData] == kMtlKaData) && curMtlIdx)
			{
				m_ParseStatus = addDataVec3(words, m_materials[curMtlIdx].ka);
			}
			else if ((words[kIdxTypeData] == kMtlKdData) && curMtlIdx)
			{
				m_ParseStatus = addDataVec3(words, m_materials[curMtlIdx].kd);
			}
			else if ((words[kIdxTypeData] == kMtlKsData) && curMtlIdx)
			{
				m_ParseStatus = addDataVec3(words, m_materials[curMtlIdx].ks);
			}
			else if ((words[kIdxTypeData] == kMtlKeData) && curMtlIdx)
			{
				m_ParseStatus = addDataVec3(words, m_materials[curMtlIdx].ke);
			}
			else if ((words[kIdxTypeData] == kMtlNiData) && curMtlIdx)
			{
				m_ParseStatus = addDataF(words, m_materials[curMtlIdx].ni);
			}
			else if ((words[kIdxTypeData] == kMtlDData) && curMtlIdx)
			{
				m_ParseStatus = addDataF(words, m_materials[curMtlIdx].d);
			}
			else if ((words[kIdxTypeData] == kMtlIlData) && curMtlIdx)
			{
				m_ParseStatus = addDataF(words, m_materials[curMtlIdx].illum);
			}
			if (m_ParseStatus == false)
				return;
		}
	}

	// for (auto &material : m_materials)
	// {
	// 	std::cout << "Ns (Specular Exponent): " << material.Ns << std::endl;
	// 	std::cout << "Ka (Ambient Color): " << material.ka.x << " " << material.ka.y << " " << material.ka.z << std::endl;
	// 	std::cout << "Kd (Diffuse Color): " << material.kd.x << " " << material.kd.y << " " << material.kd.z << std::endl;
	// 	std::cout << "Ks (Specular Color): " << material.ks.x << " " << material.ks.y << " " << material.ks.z << std::endl;
	// 	std::cout << "Ke (Emissive Color): " << material.ke.x << " " << material.ke.y << " " << material.ke.z << std::endl;
	// 	std::cout << "Ni (Index of Refraction): " << material.ni << std::endl;
	// 	std::cout << "d (Transparency): " << material.d << std::endl;
	// 	std::cout << "illum (Illumination Model): " << material.illum << std::endl;
	// 	std::cout << std::endl;
	// }
}

ParsedObject::ParsedObject(std::string objFileName, ModelState &modelState) : m_objFileName(objFileName), m_tmpFaces(4)
{
	parseObjFile(modelState);
	if (m_ParseStatus)
	{
		if (modelState.hasNormals == false)
		{
			generateNormals();
			modelState.hasNormals = true;
		}
		generateIndeces();
		std::cout << "Log: Parse done for file: " << m_objFileName << ". Verticies: " << m_positions.size() << " indeces: " << m_indices.size() << std::endl;
	}
	parseMtlFile();
}

/*
i need to know for each fragpos which material use
1. add all data to every vertex as one more positions (which is stupid)
2. use some kind of indeces as arr/map for every material
2.1 use struct material {}
2.2 on parse shader change var to set arrays of materials

map [materialname, idx]
to m_positions (vertex) add idx float for material


TODO
make cube mtl
parse it
*/