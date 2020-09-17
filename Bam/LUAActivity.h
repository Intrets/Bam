#pragma once

#include "SingleBlockActivity.h"
#include "sol/sol.hpp"

class LuaActivity : public SingleBlockActivity
{
private:
	bool interrupt = false;

	sol::state state;
	GameState* gameStateRef;

	std::string script;

	std::vector<std::string> watchedVars = { "state" };
	std::vector<std::string> labels;

	std::function<void(std::string line)> printFunction = [](std::string)
	{};

	std::optional<sol::function> luaRunFunction = std::nullopt;

	int32_t memberLabelCacheTick = -1;
	int32_t rootLabelCacheTick = -1;

	void run();

	void prepare(GameState& gameState);
	void execute(std::string);
	void refreshRunFunction();

	void initializeLuaState();

	bool applyMove(Handle h, int32_t type);
	bool applyActivity(Handle h, int32_t type);

	friend class ACTIVITYCOPIER;

public:
	void start(GameState& gameState);
	void stop();

	void setWatchedVars(std::vector<std::string>& vars);
	std::vector<std::string> const& getWatchedVars();

	void setScript(std::string script, GameState& gameState);
	std::string const& getScript();

	bool valid(Handle h);

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
	virtual void afterActivityStopLocal(GameState& gameState) override;
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
