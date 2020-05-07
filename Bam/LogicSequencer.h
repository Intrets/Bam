#pragma once
#include <optional>
#include "ControlState.h"

enum class CONTINUATION
{
	EXIT,
	STOP,
	CONTINUE
};

class GameState;

class LogicSequencer;

class LogicSequencer
{
public:
	using MaybeSequencer = std::pair<CONTINUATION, std::optional<std::unique_ptr<LogicSequencer>>>;
	using BindType = std::function<MaybeSequencer(GameState&)>;

	CONTINUATION runBinds(ControlState& controlState, GameState& gameState);
	void addBind(BindControl bindControl, BindType bind);

	LogicSequencer(bool blocking_) : LogicSequencer() { blocking = blocking_; };
	LogicSequencer();
	~LogicSequencer();

private:
	bool blocking;
	std::optional<std::unique_ptr<LogicSequencer>> next;

	std::unordered_map<BindControl, BindType, BindControlHash> binds;

};

