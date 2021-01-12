#pragma once

class GameState;
class Loader;
class Saver;

class Operation;

namespace COORDINATOR
{
	class Coordinator;
}

namespace NWT
{
	enum class TYPE
	{
		LUAACTIVITY_SETSCRIPT,
		LUAACTIVITY_START,
		LUAACTIVITY_STOP,
		GAME_LOAD,
	};
}

namespace OPERATION
{
	std::unique_ptr<Operation> loadOperation(Loader& loader);
}

class Operation
{
public:
	NWT::TYPE type;

	virtual void run(GameState& gameState, COORDINATOR::Coordinator& coordinator) = 0;
	void load(Loader& loader);
	void save(Saver& saver);

	virtual void loadDerived(Loader& loader) = 0;
	virtual void saveDerived(Saver& saver) = 0;

	Operation() = default;
	virtual ~Operation() = default;
};

class LuaActivitySetScript : public Operation
{
public:
	std::string text;
	int32_t h;

	virtual void run(GameState& gameState, COORDINATOR::Coordinator& coordinator) override;

	virtual void loadDerived(Loader& loader) override;
	virtual void saveDerived(Saver& saver) override;

	LuaActivitySetScript() = default;
	LuaActivitySetScript(int32_t h_, std::string const& text_);
	virtual ~LuaActivitySetScript() = default;
};

class LuaActivityStart : public Operation
{
public:
	int32_t h;

	virtual void run(GameState& gameState, COORDINATOR::Coordinator& coordinator) override;

	virtual void loadDerived(Loader& loader) override;
	virtual void saveDerived(Saver& saver) override;

	LuaActivityStart() = default;
	LuaActivityStart(int32_t h_);
	virtual ~LuaActivityStart() = default;
};

class LuaActivityStop : public Operation
{
public:
	int32_t h;

	virtual void run(GameState& gameState, COORDINATOR::Coordinator& coordinator) override;

	virtual void loadDerived(Loader& loader) override;
	virtual void saveDerived(Saver& saver) override;

	LuaActivityStop() = default;
	LuaActivityStop(int32_t h);
	virtual ~LuaActivityStop() = default;
};

class GameLoad : public Operation
{
public:
	std::stringstream saveBuffer;

	virtual void run(GameState& gameState, COORDINATOR::Coordinator& coordinator) override;

	virtual void loadDerived(Loader& loader) override;
	virtual void saveDerived(Saver& saver) override;

	GameLoad();
	virtual ~GameLoad() = default;
};

