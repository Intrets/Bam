#pragma once

#include "BufferWrappers.h"

class Fonts
{
public:
	enum Font
	{
		GENERAL,
		FONT_MAX,
	};

	bwo::Texture fonts;

	int32_t const mipmapLayers = 8;
	int32_t const texSize = 1024;

	Fonts();
	~Fonts();
};

