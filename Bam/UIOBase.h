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
struct State;

enum BIND_RESULT
{
	CONTINUE = 1 << 0,
	CLOSE = 1 << 1,
	STOP = 1 << 2,
	FOCUS = 1 << 3,
	CONSUME = 1 << 4,
};

typedef int32_t CallBackBindResult;

typedef State UIOCallBackParams;

typedef std::function<CallBackBindResult(UIOCallBackParams& UIOCallBackParams, UIOBase* self)> CallBack;

class UIOBase
{
protected:
	Handle selfHandle;

	std::vector<UniqueReference<UIOBase, UIOBase>> elements;

	using Bind = std::pair<BindControl, CallBack>;

	void addElement(UniqueReference<UIOBase, UIOBase> element);
	std::vector<Bind> globalBinds;
	std::vector<Bind> focussedBinds;

public:
	ScreenRectangle screenRectangle;

	virtual void translate(glm::vec2 p);
	void setScreenPixels(glm::ivec2 px);
	void moveTopLeftTo(glm::vec2 p);
	bool contains(glm::vec2 p);

	void addGlobalBind(BindControl bindControl, CallBack callBack);
	void addFocussedBind(BindControl bindControl, CallBack callBack);

	virtual CallBackBindResult runGlobalBinds(State& state);
	virtual CallBackBindResult runFocussedBinds(State& state);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) = 0;

	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth);
	virtual ~UIOBase() = default;
};

