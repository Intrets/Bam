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

	using BindType = std::function<MaybeSequencer(GameState&, LogicSequencer*)>;

	CONTINUATION runBinds(ControlState& controlState, GameState& gameState);

	void addBind(BindControl bindControl, BindType bind);

	LogicSequencer(bool blocking_) : LogicSequencer() { blocking = blocking_; };
	LogicSequencer();
	~LogicSequencer();

private:
	bool blocking;
	std::optional<std::unique_ptr<LogicSequencer>> next;

	std::unordered_map<BindControl, BindType, BindControlHash> binds;

public:
	int test = 0;
};

inline void LogicSequencer::addBind(BindControl bindControl, BindType bind) {
	binds.insert(std::make_pair(bindControl, bind));
}
