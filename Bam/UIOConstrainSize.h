#pragma once

#include "UIOBase.h"

#include "UIOSizeType.h"

enum class CONSTRAIN_ALIGNMENT
{
	TOP,
	BOTTOM,
	LEFT,
	RIGHT,
	TOPLEFT,
	TOPRIGHT,
	BOTTOMLEFT,
	BOTTOMRIGHT,
};

class UIOConstrainSize : public UIOBase
{
public:
	UIOBase* main;

	std::optional<UIOSizeType> maybeHeight;
	std::optional<UIOSizeType> maybeWidth;

	CONSTRAIN_ALIGNMENT alignment;

	UIOConstrainSize(Handle self, UniqueReference<UIOBase, UIOBase> main_);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
};

