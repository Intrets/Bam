#include "common.h"
#include "PathManager.h"
#include "ShaderLoader.h"
#include "TextureLoader.h"
#include "ModelLoader.h"
#include <fstream>

GLuint PathManager::LoadTextureP(std::string name) {
	return loadTexture(paths[RESOURCE_PATH::GRAPHICS] + name);
}

GLuint PathManager::LoadShadersP(std::string vertex, std::string fragment) {
	std::string fullPathVertex = paths[RESOURCE_PATH::SHADERS] + vertex;
	std::string fullPathFragment = paths[RESOURCE_PATH::SHADERS] + fragment;
	return LoadShaders(fullPathVertex.c_str(), fullPathFragment.c_str());
}

void PathManager::LoadModelP(std::string& name, GLuint& vertexbuffer, GLuint& uvbuffer, GLuint& normalbuffer, GLuint& indexbuffer, int32_t& indexsize) {
	std::string path = paths[RESOURCE_PATH::GRAPHICS] + name;
	loadModel(path, vertexbuffer, uvbuffer, normalbuffer, indexbuffer, indexsize);
}

bool PathManager::openFile(std::ifstream& file, RESOURCE_FILE t) {
	file.open(files[t]);
	return file.is_open();
}

bool PathManager::openFile(std::ofstream& file, RESOURCE_FILE t) {
	file.open(files[t]);
	return file.is_open();
}

bool PathManager::openSave(std::ifstream& file, std::string name) {
	file.open(paths[RESOURCE_PATH::SAVE] + name, std::ifstream::binary);
	return file.is_open();
}

bool PathManager::openSave(std::ofstream& file, std::string name) {
	file.open(paths[RESOURCE_PATH::SAVE] + name, std::ofstream::binary);
	return file.is_open();
}

PathManager::PathManager() {
	std::string r = "C:/Users/Intrets/source/Repos/Bam/Bam/";
	paths[RESOURCE_PATH::CONFIG] = r + "config/";
	paths[RESOURCE_PATH::GRAPHICS] = r + "Resources/";
	paths[RESOURCE_PATH::SHADERS] = r;
	paths[RESOURCE_PATH::SAVE] = r + "saves/";
	files[RESOURCE_FILE::OPTIONS] = r + "config/options.txt";
}

PathManager::~PathManager() {
}

