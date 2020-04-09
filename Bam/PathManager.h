#pragma once
#include <unordered_map>

typedef enum
{
	CONFIG,
	GRAPHICS,
	SHADERS
} RESOURCE_PATH;

typedef enum
{
	OPTIONS
} RESOURCE_FILE;

class ifstream;

class PathManager
{
private:
	std::unordered_map<int, std::string> paths;
	std::unordered_map<int, std::string> files;

public:
	GLuint LoadTextureP(std::string name);
	GLuint LoadShadersP(std::string vertex, std::string fragment);
	void LoadModelP(std::string& path, GLuint& vertexbuffer, GLuint& uvbuffer, GLuint& normalbuffer, GLuint& indexbuffer, int& indexsize);
	bool openFile(std::ifstream& file, RESOURCE_FILE t);
	bool openFile(std::ofstream& file, RESOURCE_FILE t);

	PathManager();
	~PathManager();
};

