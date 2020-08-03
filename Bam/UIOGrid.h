#pragma once

#include "UIOBase.h"

class UIOGrid : public UIOBase
{
public:
	glm::ivec2 size;

	UIOGrid(Handle self, glm::ivec2 size_);

	// Inherited via UIOBase
	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
};

