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

enum BIND_RESULT
{
	CONTINUE = 1 << 0,
	CLOSE = 1 << 1,
	STOP = 1 << 2,
	FOCUS = 1 << 3,
};

typedef int32_t CallBackBindResult;

typedef std::function<CallBackBindResult(GameState& gameState, ControlState& controlState, UIOBase* self)> CallBack;

class UIOBase
{
protected:
	Handle selfHandle;

	std::vector<UniqueReference<UIOBase, UIOBase>> elements;

	using Bind = std::pair<BindControl, CallBack>;

	void addElement(UniqueReference<UIOBase, UIOBase> element);
	std::list<Bind> binds;

public:
	ScreenRectangle screenRectangle;

	void translate(glm::vec2 p);
	void setScreenPixels(glm::ivec2 px);
	void moveTopLeftTo(glm::vec2 p);
	bool contains(glm::vec2 p);
	void addBind(BindControl bindControl, CallBack callBack);
	virtual CallBackBindResult runBinds(ControlState& controlState, GameState& gameState);
	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) = 0;

	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth);
	virtual ~UIOBase() = default;
};

