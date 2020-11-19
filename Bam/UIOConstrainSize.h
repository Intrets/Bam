#pragma once

#include "UIOBase.h"

#include "UIOSizeType.h"

namespace UIO
{
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
}

class UIOConstrainSize : public UIOBaseSingle
{
private:
	template<class T>
	friend class UIOConstructer;

	std::optional<UIOSizeType> maybeHeight;
	std::optional<UIOSizeType> maybeWidth;

	UIO::ALIGNMENT alignment = UIO::ALIGNMENT::CENTER;

public:
	UIOConstrainSize(Handle self, UniqueReference<UIOBase, UIOBase> main_);
	UIOConstrainSize() = default;
	~UIOConstrainSize() = default;

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
};

