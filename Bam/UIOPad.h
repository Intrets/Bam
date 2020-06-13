#pragma once

#include "UIOBase.h"

class UIOPad : public UIOBase
{
public:
	UIOBase* main;

	glm::ivec2 top;
	glm::ivec2 bot;

	UIOPad(Handle self, UniqueReference<UIOBase, UIOBase> main_);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
};

