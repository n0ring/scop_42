#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>

#include "ModelState.hpp"

struct Material
{
	nrg::vec3 color;

	float Ns;
	nrg::vec3 ka;
	nrg::vec3 kd;
	nrg::vec3 ks;
	float ni;
	float d;
	unsigned int illum;
};

struct Pos
{
	float x;
	float y;
	float z;
	float mtl;
};

struct textPos
{
	float x;
	float y;
	float mtl;
};

struct face
{
	unsigned int vtx;
	unsigned int txt;
	unsigned int nrm;
	float mtl;

};

struct vertex {
    float x, y, z, u, v, vnx, vny, vnz, mtl;

    bool operator==(const vertex &other) const {
        return x == other.x && 
		y == other.y && 
		z == other.z && 
		u == other.u && 
		v == other.v &&
		vnx == other.vnx &&
		vny == other.vny &&
		vnz == other.vnz;
    }
};

struct vertex_hash {
    std::size_t operator()(const vertex& v) const {
        return ((std::hash<float>()(v.x) ^
                (std::hash<float>()(v.y) << 1)) >> 1) ^
                (std::hash<float>()(v.z) << 1) ^
                (std::hash<float>()(v.u) << 1) ^
                (std::hash<float>()(v.v) << 1) ^

                (std::hash<float>()(v.vnx) << 1) ^
                (std::hash<float>()(v.vny) << 1) ^
                (std::hash<float>()(v.vnz) << 1);
    }
};

class ParsedObject
{
public:
	ParsedObject(std::string fileName, ModelState& modelState);
	~ParsedObject() {}

	
	inline const std::vector<vertex>& getPositions() const {return m_positions; }
	inline const std::vector<unsigned int>& getIndices() const {return m_indices; }
	inline const std::vector<Material>& getMaterials() const {return m_materials; }
	inline bool getParseStatus() const {return m_ParseStatus;}
private:
	std::string m_objFileName;
	std::string m_mtlFileName;
	std::vector<vertex> m_positions;
	std::vector<Material> m_materials = {{{1.0f, 0.0f, 0.0f}}};
	std::vector<unsigned int> m_indices;
	float m_countVert = 0;
	// float tmp_x, tmp_y, tmp_z;
	Pos tmpPos;
	textPos tmpTexPos;
	std::vector<face> m_tmpFaces;
	std::vector<std::string> m_faceWords;
	face m_tmpFace = {0, 0,0};
	bool m_ParseStatus = false;
	std::unordered_map<std::string, float> m_materialMap = {{"default", 0}};
	float m_curMtlIdx = 0;


	// const for data extract
	const int kIdxTypeData = 0;
	const int kIdxXData = 1;
	const int kIdxYData = 2;
	const int kIdxZData = 3;
	const int kIdxMaterial = 1;
	const std::string kVertexData = "v";
	const std::string kTextureCoordData = "vt";
	const std::string kFaceData = "f";
	const std::string kNormalData = "vn";
	const std::string kMtlData = "usemtl";
	const int kTriangleCount = 3;
	const int kSquareCount = 4;

	bool addVertex(std::vector<std::string>& words, std::vector<Pos>& vertices, ModelState& modelState);
	bool addNormals(std::vector<std::string>& words, std::vector<Pos>& normals);
	bool addTextureCoord(std::vector<std::string>& words, std::vector<textPos>& text_coord);
	bool addComplexFace(std::vector<std::string>& words, std::vector<face>& faces);
	bool addSimpleFace(std::vector<std::string>& words, std::vector<face>& faces);

	void parseFile(ModelState& modelState);
	void generateIndeces();
	void generateNormals();

	void split(std::string& line, std::vector<std::string>& words, char div);
};