#pragma once

#include "BufferWrappers.h"
#include "Fonts.h"

constexpr auto MAX_CHARACTER_DRAW = 1 << 16;

struct TextRenderInfo;
struct CameraInfo;

class TextRenderer
{
public:
	void render(TextRenderInfo& textRenderInfo, Fonts& fonts, GLuint target, CameraInfo& camInfo);

	TextRenderer();
	~TextRenderer();
};

