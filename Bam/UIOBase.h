#pragma once
#include "ScreenRectangle.h"
#include "ReferenceManager.h"

class UIOBase;
struct RenderInfo;

class UIOBase
{
protected:
	Handle selfHandle;

	ScreenRectangle screenRectangle;
	std::vector<UniqueReference<UIOBase, UIOBase>> elements;
	//std::vector<WeakReference<UIOBase, UIOBase>> elements;
	//std::vector<ManagedReference<UIOBase, UIOBase>> elements;
	//std::vector<std::unique_ptr<UIOBase>> elements;

	//void addElement(std::unique_ptr<UIOBase> element);
	void addElement(UniqueReference<UIOBase, UIOBase> element);

public:
	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) = 0;

	virtual int32_t addRenderInfo(RenderInfo& renderInfo, int32_t depth);
	virtual ~UIOBase() = default;
};

