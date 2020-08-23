#pragma once

#include "UIOBase.h"
#include "UIOSizeType.h"

class UIOPad : public UIOBase
{
private:
	UIOBase* main;

private:
	template<class T>
	friend class UIOConstructer;

	// TODO: refactor hotbar class
	friend class UIOHotbar;

	std::optional<UIOSizeType> top;
	std::optional<UIOSizeType> bottom;
	std::optional<UIOSizeType> left;
	std::optional<UIOSizeType> right;

public:
	UIOPad(Handle self, UniqueReference<UIOBase, UIOBase> main_);
	UIOPad(Handle self, UniqueReference<UIOBase, UIOBase> main_, UIOSizeType padding);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
};

