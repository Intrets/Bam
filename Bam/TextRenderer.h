#pragma once

#include "BufferWrappers.h"

constexpr auto MAX_CHARACTER_DRAW = 1 << 16;

struct TextRenderInfo;
struct CameraInfo;
class Fonts;

class TextRenderer
{
public:
	void render(TextRenderInfo const& textRenderInfo, Fonts const& fonts, GLuint target, CameraInfo const& camInfo);

	TextRenderer();
	~TextRenderer();
};

