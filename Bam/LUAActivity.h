#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>

#include "SingleBlockActivity.h"
#include "sol/sol.hpp"
#include "LuaStore.h"

class GameState;

class LuaActivity : public SingleBlockActivity
{
private:
	bool interrupt = false;
	bool running = false;

	std::optional<LUASTORE::Args> args;

	sol::state state;
	GameState* gameStateRef;

	std::string script;

	std::vector<std::string> watchedVars = { "state" };
	std::unordered_map<std::string, int32_t> labels;
	std::vector<std::vector<int32_t>> labelLists;

	std::function<void(std::string line)> printFunction = [](std::string)
	{};

	std::optional<sol::function> luaRunFunction = std::nullopt;
	std::optional<sol::function> luaReceiveMessageFunction = std::nullopt;

	int32_t memberLabelCacheTick = -1;
	int32_t rootLabelCacheTick = -1;

	void updateLabels();
	void run();
	bool processMessage();
	bool runLoop();

	void prepare(GameState& gameState);
	void execute(std::string);
	void refreshRunFunction();

	void initializeLuaState();

	bool validIndex(int32_t index) const;

	bool applyMove(int32_t index, int32_t type);
	bool applyActivity(int32_t index, int32_t type);
	bool sendMessage(int32_t index, sol::variadic_args& va);

	bool canReceiveMessage() const;
	bool receiveMessage(LUASTORE::Args& args_);

	friend class ACTIVITYCOPIER;
	friend class Forwarder;

public:
	void stop();
	void start();

	void setWatchedVars(std::vector<std::string>& vars);
	std::vector<std::string> const& getWatchedVars();

	void setScript(std::string script, GameState& gameState);
	std::string const& getScript();

	void setPrintFunction(std::function<void(std::string line)> f);

	inline sol::object getLuaObject(std::string name);

	// Activity Functions
	LuaActivity();
	LuaActivity(Handle self, GameState& gameState, glm::ivec2 pos);
	virtual ~LuaActivity() = default;

	virtual bool moveableIdleLocal() override;

	// Placement
	// Already implemented in SingleBlockActivity

	// Activity
	virtual bool canActivityLocal(GameState& gameState, int32_t type) override;
	virtual void applyActivityLocalForced(GameState& gameState, int32_t type, int32_t pace) override;

	// Moveable
	// Already implemented in SingleBlockActivity

	// Traces Placement
	// Already implemented in SingleBlockActivity

	// Traces Activity
	virtual void postActivityStopLocal(GameState& gameState) override;

	// Traces Moveable
	// Already implemented in SingleBlockActivity

	// Tree Information
	// Already implemented in SingleBlockActivity

	// Serial
	virtual ACTIVITY::TYPE getType() override;
	virtual void save(Saver& saver) override;
	virtual bool load(Loader& loader) override;
};

sol::object LuaActivity::getLuaObject(std::string name) {
	return this->state[name];
}
