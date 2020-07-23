#pragma once

#include "BufferWrappers.h"

struct TextRenderInfo;
struct CameraInfo;

struct FontInfo
{
	std::string name;
	std::array<glm::vec4, 128> charUV;
	std::array<glm::ivec2, 128> charSize;
};

class Fonts
{
private:
	glm::vec2 pos;
	float laneWidth;

	FontInfo loadMonospacedFont(std::string name, glm::ivec2 charDim, glm::ivec2 gridDim);

public:
	enum class Font
	{
		ROBOTO_12,
		ROBOTO_14,
		ROBOTO_16,
		FONT_MAX,
	};

	bwo::Buffer buffer;
	bwo::Texture fontAtlas;

	std::array<FontInfo, static_cast<int32_t>(Font::FONT_MAX)> fontInfos;

	int32_t atlasSize;

	FontInfo& getFont(Font font);

	Fonts();
	~Fonts();
};

