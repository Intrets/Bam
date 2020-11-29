#pragma once

#include "UIOBase.h"

class UIODestructible : public UIOBaseSingle
{
public:
	bool destruct = false;

	UIODestructible(Handle self);
	virtual ~UIODestructible() = default;
};

