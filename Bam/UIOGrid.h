#pragma once

#include "UIOBase.h"

class UIOGrid : public UIOBase
{
private:
	glm::ivec2 size;

public:
	UIOGrid() = default;
	UIOGrid(Handle self, glm::ivec2 size_);
	virtual	~UIOGrid() = default;

	// Inherited via UIOBase
	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
};

