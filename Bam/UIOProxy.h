#pragma once

#include "UIOBase.h"

class UIOProxy : public UIOBaseSingle
{
public:
	UIOProxy(Handle self);
	virtual ~UIOProxy() = default;

	[[nodiscard]]
	UniqueReference<UIOBase, UIOBase> getMain();
};

