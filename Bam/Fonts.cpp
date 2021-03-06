#include "common.h"

#include "Fonts.h"

#include "BlitRenderer.h"
#include "TextRenderInfo.h"
#include "CameraInfo.h"
#include "Locator.h"
#include "PathManager.h"

FontInfo Fonts::loadMonospacedFont(std::string name, glm::ivec2 charDim, glm::ivec2 gridDim) {
	bwo::Texture tex;
	tex.ID = Locator<PathManager>::ref().LoadFont(name);
	std::vector<glm::vec4> uvs;
	std::vector<glm::vec4> worlds;

	this->laneWidth = glm::max(this->laneWidth, charDim.y / static_cast<float>(this->fontAtlas.size.y));

	glm::ivec2 texSize;
	glBindTexture(GL_TEXTURE_2D, tex.ID);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &texSize.x);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &texSize.y);

	int count = 0;
	for (int32_t y = 0; y < gridDim.y; y++) {
		for (int32_t x = 0; x < gridDim.x; x++) {
			glm::vec2 uvTop = glm::vec2(charDim.x * x, charDim.y * y) / glm::vec2(texSize);
			glm::vec2 uvSize = glm::vec2(charDim) / glm::vec2(texSize);

			uvs.push_back(glm::vec4(uvTop, uvSize));
			count++;
			if (count == 128) {
				break;
			}
		}
	}

	FontInfo fontInfoResult;
	fontInfoResult.name = name;

	glm::vec2 worldSize = 2.0f * glm::vec2(charDim) / glm::vec2(this->fontAtlas.size);
	//worldSize.y *= -1.0f;

	for (int32_t i = 0; i < 128; i++) {
		glm::vec2 worldTop = this->pos;
		worldTop.y -= worldSize.y;

		worlds.push_back(glm::vec4(worldTop, worldSize));

		fontInfoResult.charSize[i] = charDim;
		glm::vec2 uv = worldTop;
		uv.y -= worldSize.y;
		fontInfoResult.charUV[i] = glm::vec4((uv + 1.0f) / 2.0f, worldSize / 2.0f);

		this->pos.x += worldSize.x;

		if (this->pos.x + worldSize.x > 1.0f) {
			this->pos.x = -1.0f;
			this->pos.y -= worldSize.y;
		}
		if (this->pos.y < -1.0f) {
			assert(0);
		}
	}

	Locator<BlitRenderer>::ref().render(
		uvs,
		worlds,
		this->buffer.ID,
		glm::ivec4(0, 0, this->fontAtlas.size.x, this->fontAtlas.size.y),
		tex.ID,
		std::nullopt,
		true,
		glm::vec2(0.0f)
	);

	return fontInfoResult;
}

FontInfo& Fonts::getFont(FONTS::FONT font) {
	return this->fontInfos[static_cast<int32_t>(font)];
}

Fonts::Fonts() :
	fontAtlas(bwo::Texture2DHelper::makeNoFiltering(glm::ivec2(256))) {

	this->buffer.bindTexture(GL_COLOR_ATTACHMENT0, this->fontAtlas, 0);

	this->pos = glm::vec2(-1.0f, 1.0f);

	this->fontInfos[static_cast<size_t>(FONTS::FONT::ROBOTO_12)] = loadMonospacedFont("roboto_mono_12px_7x17_36x4.dds", { 7,17 }, { 36,4 });
	this->fontInfos[static_cast<size_t>(FONTS::FONT::ROBOTO_14)] = loadMonospacedFont("roboto_mono_14px_8x19_32x4.dds", { 8,19 }, { 32,4 });
	this->fontInfos[static_cast<size_t>(FONTS::FONT::ROBOTO_16)] = loadMonospacedFont("roboto_mono_16px_10x22_25x6.dds", { 10,22 }, { 25,6 });
}

Fonts::~Fonts() {
}
