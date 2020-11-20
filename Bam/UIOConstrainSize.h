#pragma once

#include "UIOBase.h"

#include "UIOSizeType.h"
#include "Enums.h"

class UIOConstrainSize : public UIOBaseSingle
{
private:
	template<class T>
	friend class UIOConstructer;

	std::optional<UIOSizeType> maybeHeight;
	std::optional<UIOSizeType> maybeWidth;

	UIO::ALIGNMENT alignment = UIO::ALIGNMENT::CENTER;

public:
	void setHeight(UIOSizeType height);
	void setWidth(UIOSizeType width);
	void setAlignment(UIO::ALIGNMENT alignment);

	UIOConstrainSize(Handle self, UniqueReference<UIOBase, UIOBase> main_);
	UIOConstrainSize(Handle self);
	UIOConstrainSize() = default;
	~UIOConstrainSize() = default;

	virtual UIO::TYPE getUIOType() override;

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
};

