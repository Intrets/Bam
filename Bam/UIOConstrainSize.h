#pragma once

#include "UIOBase.h"

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

	std::optional<int32_t> height;
	std::optional<int32_t> width;

	CONSTRAIN_ALIGNMENT alignment;

	UIOConstrainSize(Handle self, UniqueReference<UIOBase, UIOBase> main_);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
};

