#pragma once

#include <unordered_set>
#include <string>
#include <sol/sol.hpp>

class GameState;
class Saver;
class Loader;
struct RenderInfo;
typedef int32_t Handle;
class LuaActivity;

class ActivityLuaTest
{
private:
	sol::state state;
	GameState* gameStateRef;

	std::string script;

	// The target Activity and its group to act on.
	LuaActivity* target;

	std::vector<Handle> validTargets;

	std::function<void(std::string line)> printFunction = [](std::string)
	{};

	void prepare(GameState& gameState);
	void execute(std::string);

	friend class LuaActivity;

public:
	void run(GameState& gameState);
	void init(GameState& gameState);

	void setScript(std::string script, GameState& gameState);
	std::string const& getScript();

	bool valid(Handle h);

	bool applyMove(Handle h, int32_t type);
	bool applyActivity(Handle h, int32_t type);

	void setPrintFunction(std::function<void(std::string line)> f);

	void save(Saver& saver);
	void load(Loader& loader);

	inline sol::object getLuaObject(std::string name);

	ActivityLuaTest();
	ActivityLuaTest(LuaActivity& ptr);
	~ActivityLuaTest() = default;

protected:
	virtual void appendRenderInfoInternal(GameState& gameState, RenderInfo& renderInfo);
};

inline sol::object ActivityLuaTest::getLuaObject(std::string name) {
	return this->state[name];
}

