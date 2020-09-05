#pragma once

#include "SingleBlockActivity.h"
#include "sol/sol.hpp"

class LuaActivity : public SingleBlockActivity
{
private:
	bool interrupt;

	sol::state state;
	GameState* gameStateRef;

	std::string script;

	std::vector<Handle> validTargets;
	std::vector<std::string> watchedVars = { "state" };

	std::function<void(std::string line)> printFunction = [](std::string)
	{};

	std::optional<sol::function> luaRunFunction;

	void run();

	void prepare(GameState& gameState);
	void execute(std::string);
	void refreshRunFunction();

	void initializeLuaState();

public:
	void start(GameState& gameState);
	void stop();

	void setWatchedVars(std::vector<std::string>& vars);
	std::vector<std::string> const& getWatchedVars();

	void init();

	void setScript(std::string script, GameState& gameState);
	std::string const& getScript();

	bool valid(Handle h);

	bool applyMove(Handle h, int32_t type);
	bool applyActivity(Handle h, int32_t type);

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
	virtual void removeActivityTracesLocal(GameState& gameState) override;
	virtual void leaveActivityTracesLocal(GameState& gameState) override;

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
