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

namespace FONTS
{
	enum class FONT
	{
		ROBOTO_12,
		ROBOTO_14,
		ROBOTO_16,
		FONT_MAX,
	};
}

class Fonts
{
private:
	glm::vec2 pos;
	float laneWidth;

	FontInfo loadMonospacedFont(std::string name, glm::ivec2 charDim, glm::ivec2 gridDim);

public:
	bwo::FrameBuffer buffer;
	bwo::Texture2D fontAtlas;

	std::array<FontInfo, static_cast<int32_t>(FONTS::FONT::FONT_MAX)> fontInfos;

	FontInfo& getFont(FONTS::FONT font);

	Fonts();
	~Fonts();
};

