#pragma once

#include "BufferWrappers.h"

#include <vector>
#include <random>
#include <functional>

class PerlinNoise
{
public:
	glm::ivec2 pos;
	glm::ivec2 dpos;

	std::vector<std::vector<glm::vec2>> gradient;
	std::vector<std::vector<bool>> block;
	std::vector<std::vector<float>> values;

	//bwo::FrameBuffer frameBuffer;
	bwo::Texture texture;

	void fillTexture(bool fillTexture);

	float dotGridGradient(int32_t ix, int32_t iy, float x, float y);

	PerlinNoise() = delete;
	PerlinNoise(glm::ivec2 pos, glm::ivec2 dpos);
	~PerlinNoise() = default;
};

class NoiseLayer
{
public:
	int32_t seed;
	glm::vec2 pos;
	glm::vec2 dpos;
	glm::vec2 scale;
	glm::ivec2 di;
	glm::ivec2 ipos;

	std::vector<std::vector<glm::vec2>> gradient;

	float value(float x, float y);

	float dotGridGradient(int32_t ix, int32_t iy, float x, float y);
	glm::vec2 hashGradient(int32_t x, int32_t y);

	NoiseLayer(int32_t _seed, glm::vec2 _pos, glm::vec2 _dpos, glm::vec2 _scale);

	NoiseLayer() = delete;
	~NoiseLayer() = default;

};