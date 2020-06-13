#pragma once

#include "UIOBase.h"
#include <variant>

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

	std::optional<std::variant<int32_t, float>> height;
	std::optional<std::variant<int32_t, float>> width;

	CONSTRAIN_ALIGNMENT alignment;

	UIOConstrainSize(Handle self, UniqueReference<UIOBase, UIOBase> main_);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
};

