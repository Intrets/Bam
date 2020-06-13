#pragma once

#include "UIOBase.h"

class UIOPad : public UIOBase
{
public:
	UIOBase* main;

	std::optional<std::variant<int32_t, float>> top;
	std::optional<std::variant<int32_t, float>> bottom;
	std::optional<std::variant<int32_t, float>> left;
	std::optional<std::variant<int32_t, float>> right;

	UIOPad(Handle self, UniqueReference<UIOBase, UIOBase> main_);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
};

