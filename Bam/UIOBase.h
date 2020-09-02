#pragma once

#include <optional>
#include <variant>

#include "Rectangle.h"
#include "ReferenceManager.h"
#include "ControlState.h"

class UIOBase;
struct RenderInfo;
class GameState;

class UIOBase;
struct State;

namespace BIND
{
	enum RESULT
	{
		CONTINUE = 1 << 0,
		CLOSE = 1 << 1,
		STOP = 1 << 2,
		FOCUS = 1 << 3,
		CONSUME = 1 << 4,
		HIDE = 1 << 5,
	};
}

typedef int32_t CallBackBindResult;

typedef State UIOCallBackParams;

typedef std::function<CallBackBindResult(UIOCallBackParams& UIOCallBackParams, UIOBase* self)> CallBack;

class UIOBase
{
protected:
	bool active = false;

	Handle selfHandle;

	std::vector<UniqueReference<UIOBase, UIOBase>> elements;

	using Bind = std::pair<BindControl, CallBack>;

	std::vector<Bind> onHoverBinds;
	std::vector<Bind> activeBinds;
	std::vector<Bind> focussedBinds;
	std::vector<Bind> globalBinds;
	std::vector<Bind> gameWorldBinds;

protected:
	template<class T>
	friend class UIOConstructer;

	ScreenRectangle screenRectangle;

public:
	ScreenRectangle const& getScreenRectangle() const;

	void addElement(UniqueReference<UIOBase, UIOBase> element);

	void activate();
	void deactivate();

	virtual void translate(glm::vec2 p);
	void setScreenPixels(glm::ivec2 px);
	void moveTopLeftTo(glm::vec2 p);
	bool contains(glm::vec2 p) const;

	void addGlobalBind(BindControl bindControl, CallBack callBack);
	void addFocussedBind(BindControl bindControl, CallBack callBack);
	void addOnHoverBind(BindControl bindControl, CallBack callBack);
	void addActiveBind(BindControl bindControl, CallBack callBack);
	void addGameWorldBind(BindControl bindControl, CallBack callBack);

	virtual CallBackBindResult runGlobalBinds(State& state);
	virtual CallBackBindResult runFocussedBinds(State& state);
	virtual CallBackBindResult runOnHoverBinds(State& state);
	virtual CallBackBindResult runActiveBinds(State& state);
	virtual CallBackBindResult runGameWorldBinds(State& state);

	virtual ScreenRectangle updateSize(ScreenRectangle newScreenRectangle) = 0;

	virtual int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth);

	UIOBase() = default;
	virtual ~UIOBase() = default;
};

