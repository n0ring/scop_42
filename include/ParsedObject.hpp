#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include "ModelState.hpp"

class ParsedObject
{
public:
	ParsedObject(std::string fileName, ModelState& modelState);
	~ParsedObject() {}

	
	inline const std::vector<float>& getPositions() const {return m_positions; }
	inline const std::vector<unsigned int>& getIndices() const {return m_indices; }
private:
	std::string m_fileName;
	std::vector<float> m_positions;
	std::vector<unsigned int> m_indices;

	void parseFile(ModelState& modelState);
	void split(std::string& line, std::vector<std::string>& words);
	
};