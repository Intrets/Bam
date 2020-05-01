#include "common.h"

#include "PerlinNoise.h"
#include "StaticWorldChunk.h"

#include <iostream>

const float mix(float a, float b, float x) {
	return a + glm::smoothstep(0.0f, 1.0f, x) * (b - a);
}

void PerlinNoise::fillTexture(bool fillTexture) {
	std::vector<glm::i8vec4> tex;
	if (fillTexture) {
		tex = std::vector<glm::i8vec4>(dpos.x * dpos.y);
	}
	block = std::vector<std::vector<bool>>(dpos.x, std::vector<bool>(dpos.y, false));
	values = std::vector<std::vector<float>>(dpos.x, std::vector<float>(dpos.y, 0.0f));

	std::vector<NoiseLayer> layers;
	std::vector<float> weights{ 1.4f, 1.2f, 1.0f };
	std::vector<float> scales{ 32.0f, 64.0f, 256.0f };

	for (auto scale : scales) {
		layers.push_back(NoiseLayer(200994617, pos, dpos, glm::vec2(scale)));
	}

	for (int i = 0; i < dpos.x; i++) {
		for (int j = 0; j < dpos.y; j++) {
			float value = 1.0f;
			for (size_t k = 0; k < layers.size(); k++) {
				// TODO: value cant handle big coords?
				value *= glm::pow(layers[k].value(static_cast<float>(pos.x + i), static_cast<float>(pos.y + j)), weights[k]);
			}
			value = glm::pow(value, 1 / 3.6f);

			value = 1 - value;
			values[i][j] = value;
			if (value > 0.85f) {
				value = 1.0f;
				block[i][j] = true;
			}
			else {
				value = 0.0f;
			}
			
			value *= 255.0f;
			if (fillTexture) {
				tex[i + dpos.x * j] = glm::u8vec4((glm::u8) value);
			}
		}
	}
	if (fillTexture) {
		glGenTextures(1, &texture.ID);
		// "Bind" the newly created texture : all future texture functions will modify this texture
		glBindTexture(GL_TEXTURE_2D, texture.ID);

		glTexImage2D(
			GL_TEXTURE_2D,
			0, GL_RGBA,
			dpos.x, dpos.y, 0,
			GL_RGBA, GL_UNSIGNED_BYTE,
			&tex[0][0]
		);

		// Poor filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
}

float PerlinNoise::dotGridGradient(int ix, int iy, float x, float y) {
	// Compute the distance vector
	float dx = x - static_cast<float>(ix);
	float dy = y - static_cast<float>(iy);

	// Compute the dot-product
	return (dx * gradient[iy][ix][0] + dy * gradient[iy][ix][1]);
}

PerlinNoise::PerlinNoise(glm::ivec2 _pos, glm::ivec2 _dpos) : pos(_pos), dpos(_dpos) {
	//int seed = 200994617;
	//int seed = 987654;
}

// TODO: value cant handle big coords?
float NoiseLayer::value(float worldx, float worldy) {
	float x = (worldx) / scale.x;
	float y = (worldy) / scale.y;

	// grid coords
	int x0 = static_cast<int>(glm::floor(x));
	int x1 = x0 + 1;
	int y0 = static_cast<int>(glm::floor(y));
	int y1 = y0 + 1;

	float sx = x - static_cast<float>(x0);
	float sy = y - static_cast<float>(y0);

	float n0, n1, ix0, ix1, value;

	n0 = dotGridGradient(x0, y0, x, y);
	n1 = dotGridGradient(x1, y0, x, y);
	ix0 = mix(n0, n1, sx);

	n0 = dotGridGradient(x0, y1, x, y);
	n1 = dotGridGradient(x1, y1, x, y);
	ix1 = mix(n0, n1, sx);

	value = mix(ix0, ix1, sy);
	value /= sqrt(2.0f) / 2.0f;
	value = abs(value);
	return value;
}

float NoiseLayer::dotGridGradient(int ix, int iy, float x, float y) {
	// Compute the distance vector
	float dx = x - static_cast<float>(ix);
	float dy = y - static_cast<float>(iy);

	// Compute the dot-product
	return (dx * gradient[ix - ipos.x][iy - ipos.y].x + dy * gradient[ix - ipos.x][iy - ipos.y].y);
}

glm::vec2 NoiseLayer::hashGradient(int x, int y) {
	//TODO: make better point hashing?
	int s = x + seed * y;

	std::mt19937 step(s);

	std::uniform_real_distribution<float> range(0.0f, 360.0f);

	float angle = glm::radians(range(step));

	return glm::vec2(glm::sin(angle), glm::cos(angle));
}

NoiseLayer::NoiseLayer(int _seed, glm::vec2 _pos, glm::vec2 _dpos, glm::vec2 _scale) :
	seed(_seed),
	pos(_pos),
	dpos(_dpos),
	scale(_scale) {

	di.x = 3 + static_cast<int>(glm::floor(dpos.x / scale.x));
	di.y = 3 + static_cast<int>(glm::floor(dpos.y / scale.y));

	ipos.x = static_cast<int>(glm::floor(pos.x / scale.x));
	ipos.y = static_cast<int>(glm::floor(pos.y / scale.y));

	gradient = std::vector<std::vector<glm::vec2>>(di.x, std::vector<glm::vec2>(di.y));

	for (int i = 0; i < di.x; i++) {
		glm::ivec2 dpos1;
		dpos1.x = ipos.x + i;
		for (int j = 0; j < di.y; j++) {
			dpos1.y = ipos.y + j;
			gradient[i][j] = hashGradient(dpos1.x, dpos1.y);
		}
	}
}
