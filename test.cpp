#include <vector>
#include <fstream>
#include <iostream>

void split(std::string& line, std::vector<std::string>& words, char div)
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

bool isLineValid(std::vector<std::string>& words)
{
	if (words.empty())
		return false;
	std::string& token = words[0];
	return token == "v" || token == "f" || token == "vt";
}


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


void parseFile()
{
	std::string line;
	std::ifstream file("new_cube.obj");
	std::vector<std::string> words;
	std::vector<Pos> vertices;
	std::vector<textPos> text_coord;
	std::vector<face> faces;
	std::vector<std::string> face_words;
	std::vector<face> tmp_faces(4);
	float x, y, z;

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
			} else if (words[0] == "vt")
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
							tmp_faces[countFaces].txt = std::stoi(face_words[1]);
						}
						if (face_words.size() == 3) // n/n/n
							tmp_faces[countFaces].nrm = std::stoi(face_words[2]);
						countFaces++;
					}
					if (countFaces == 4)
					{
						std::cout << "here\n";
						for (auto& f : tmp_faces)
							faces.push_back(f);
					}
					else if (countFaces == 5)
					{
						std::cout << "here1\n";
						faces.push_back(tmp_faces[0]);
						faces.push_back(tmp_faces[1]);
						faces.push_back(tmp_faces[2]);
						faces.push_back(tmp_faces[0]);
						faces.push_back(tmp_faces[2]);
						faces.push_back(tmp_faces[3]);
				}
				else  // default not / / / 
				{

				}

				}
			}
		}
	}
	for (auto f : faces)
		std::cout << f.vtx << " " << f.txt << " " << f.nrm << std::endl;
}

int main()
{
	parseFile();
	return 0;
}