#include "common.h"

#include "Network.h"
#include "GameState.h"
#include "Loader.h"
#include "Saver.h"
#include "ReferenceManager.h"
#include "LUAActivity.h"
#include "Coordinator.h"
#include "Locator.h"
#include "Log.h"

void Operation::run(GameState& gameState, COORDINATOR::Coordinator& coordinator) {
}

void Operation::load(Loader& loader) {
	this->loadDerived(loader);
}

void Operation::save(Saver& saver) {
	saver.store(static_cast<int32_t>(this->type));
	this->saveDerived(saver);
}

void LuaActivitySetScript::run(GameState& gameState, COORDINATOR::Coordinator& coordinator) {
	WeakReference<Activity, LuaActivity>(gameState.getActivityManager(), this->h).get()->setScript(this->text, gameState);
}

void LuaActivitySetScript::loadDerived(Loader& loader) {
	loader.retrieve(this->text);
	loader.retrieve(this->h);
}

void LuaActivitySetScript::saveDerived(Saver& saver) {
	saver.store(this->text);
	saver.store(this->h);
}

LuaActivitySetScript::LuaActivitySetScript(int32_t h_, std::string const& text_) : text(text_), h(h_) {
	this->type = NWT::TYPE::LUAACTIVITY_SETSCRIPT;
}

void LuaActivityStart::run(GameState& gameState, COORDINATOR::Coordinator& coordinator) {
	WeakReference<Activity, LuaActivity>(gameState.getActivityManager(), this->h).get()->start();
}

void LuaActivityStart::loadDerived(Loader& loader) {
	loader.retrieve(this->h);
}

void LuaActivityStart::saveDerived(Saver& saver) {
	saver.store(this->h);
}

LuaActivityStart::LuaActivityStart(int32_t h_) : h(h_) {
	this->type = NWT::TYPE::LUAACTIVITY_START;
}

void LuaActivityStop::run(GameState& gameState, COORDINATOR::Coordinator& coordinator) {
	WeakReference<Activity, LuaActivity>(gameState.getActivityManager(), this->h).get()->stop();
}

void LuaActivityStop::loadDerived(Loader& loader) {
	loader.retrieve(this->h);
}

void LuaActivityStop::saveDerived(Saver& saver) {
	saver.store(this->h);
}

LuaActivityStop::LuaActivityStop(int32_t h_) : h(h_) {
	this->type = NWT::TYPE::LUAACTIVITY_STOP;
}

#define LOAD(TYPE) res = std::make_unique<TYPE>();\
				res->type = type;\
				res->load(loader);

std::unique_ptr<Operation> OPERATION::loadOperation(Loader& loader) {
	int32_t t;
	loader.retrieve(t);
	auto type = static_cast<NWT::TYPE>(t);

	std::unique_ptr<Operation> res;

	switch (type) {
		case NWT::TYPE::LUAACTIVITY_SETSCRIPT:
			LOAD(LuaActivitySetScript)
				break;
		case NWT::TYPE::LUAACTIVITY_START:
			LOAD(LuaActivityStart)
				break;
		case NWT::TYPE::LUAACTIVITY_STOP:
			LOAD(LuaActivityStop)
				break;
		case NWT::TYPE::GAME_LOAD:
			LOAD(GameLoad)
				break;
		default:
			assert(0);
			break;
	}

	return res;
}

void GameLoad::run(GameState& gameState, COORDINATOR::Coordinator& coordinator) {
	if (Loader(this->saveBuffer, gameState).loadGame()) {
		Locator<Log>::ref().putLine("Successfully loaded the world in Coordinator\n");
	}
	else {
		Locator<Log>::ref().putLine("Failed to load the world in Coordinator\n");
	}
	coordinator.reset(gameState.tick);
}

void GameLoad::loadDerived(Loader& loader) {
	this->saveBuffer << loader.getBuffer().rdbuf();
}

void GameLoad::saveDerived(Saver& saver) {
	saver.getBuffer() << this->saveBuffer.rdbuf();
}

GameLoad::GameLoad() {
	this->type = NWT::TYPE::GAME_LOAD;
}
