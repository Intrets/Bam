#pragma once
#include "ScreenRectangle.h"
#include "ReferenceManager.h"
#include <optional>
#include <variant>
#include "ControlState.h"

class UIOBase;
struct RenderInfo;
class GameState;

class UIOBase;

class UIOBase
{
protected:
	Handle selfHandle;
	ScreenRectangle screenRectangle;

	std::vector<UniqueReference<UIOBase, UIOBase>> elements;

	using CallBack = std::function<bool(GameState& gameState, UIOBase* self)>;
	using Bind = std::pair<BindControl, CallBack>;

	void addElement(UniqueReference<UIOBase, UIOBase> element);
	std::list<Bind> binds;

public:
	void scale(glm::vec2 s);
	bool contains(glm::vec2 p);
	void addBind(BindControl bindControl, CallBack callBack);
	void runBinds(ControlState& controlState, GameState& gameState);
	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) = 0;

	virtual int32_t addRenderInfo(RenderInfo& renderInfo, int32_t depth);
	virtual ~UIOBase() = default;
};

