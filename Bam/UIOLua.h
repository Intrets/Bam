#pragma once

#include "UIOBase.h"

#include "ReferenceManager.h"
#include "LuaActivity.h"

class UIOLua : public UIOBase
{
private:
	ManagedReference<Activity, LuaActivity> watched;

public:
	void setWatched(WeakReference<Activity, LuaActivity> ref);
	ManagedReference<Activity, LuaActivity> const& getWatched();

	UIOLua(Handle self, WeakReference<Activity, LuaActivity> ref);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) override;
};

