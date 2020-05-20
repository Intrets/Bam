#pragma once
#include "ScreenRectangle.h"
#include "ReferenceManager.h"


class UIOBase;

class UIOBase
{
protected:
	ScreenRectangle screenRectangle;
	std::vector<WeakReference<UIOBase, UIOBase>> elements;
	//std::vector<ManagedReference<UIOBase, UIOBase>> elements;
	//std::vector<std::unique_ptr<UIOBase>> elements;

	void addElement(std::unique_ptr<UIOBase> element);
	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) = 0;



};

