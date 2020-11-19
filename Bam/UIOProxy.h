#pragma once

#include "UIOBase.h"

class UIOProxy : public UIOBaseSingle
{
public:
	UIOProxy(Handle self);

	UniqueReference<UIOBase, UIOBase> getMain();
};

