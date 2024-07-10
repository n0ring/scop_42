#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include "ModelState.hpp"
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

class ParsedObject
{
public:
	ParsedObject(std::string fileName, ModelState& modelState);
	~ParsedObject() {}

	
	inline const std::vector<float>& getPositions() const {return m_positions; }
	inline const std::vector<unsigned int>& getIndices() const {return m_indices; }
	inline bool getParseStatus() const {return m_ParseStatus;}

private:
	std::string m_fileName;
	std::vector<float> m_positions;
	std::vector<unsigned int> m_indices;
	float m_countVert = 0;
	float tmp_x, tmp_y, tmp_z;
	std::vector<face> m_tmpFaces;
	std::vector<std::string> m_faceWords;
	face m_tmpFace = {0, 0,0};
	bool m_ParseStatus = false;


	// const for data extract
	const int kIdxTypeData = 0;
	const int kIdxXData = 1;
	const int kIdxYData = 2;
	const int kIdxZData = 3;
	const std::string kVertexData = "v";
	const std::string kTextureCoordData = "vt";
	const std::string kFaceData = "f";
	const int kTriangleCount = 3;
	const int kSquareCount = 4;

	bool addVertex(std::vector<std::string>& words, std::vector<Pos>& vertices, ModelState& modelState);
	bool addTextureCoord(std::vector<std::string>& words, std::vector<textPos>& text_coord);
	bool addComplexFace(std::vector<std::string>& words, std::vector<face>& faces);
	bool addSimpleFace(std::vector<std::string>& words, std::vector<face>& faces);

	void parseFile(ModelState& modelState, int i);

	void split(std::string& line, std::vector<std::string>& words, char div);
};