#pragma once

#include "UIOBase.h"

#include "UIOSizeType.h"

class UIOConstrainSize : public UIOBase
{
public:
	enum class ALIGNMENT
	{
		TOP,
		BOTTOM,
		LEFT,
		RIGHT,
		TOPLEFT,
		TOPRIGHT,
		BOTTOMLEFT,
		BOTTOMRIGHT,
		CENTER,
	};

	UIOBase* main;

	std::optional<UIOSizeType> maybeHeight;
	std::optional<UIOSizeType> maybeWidth;

	UIOConstrainSize::ALIGNMENT alignment = UIOConstrainSize::ALIGNMENT::CENTER;

	UIOConstrainSize(Handle self, UniqueReference<UIOBase, UIOBase> main_);
	UIOConstrainSize() = default;
	~UIOConstrainSize() = default;

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
};

