#include "common.h"

#include "LuaActivity.h"
#include "GameState.h"
#include "StringHelpers.h"
#include "Saver.h"
#include "Loader.h"

static bool isSimpleValue(sol::object t, std::unordered_set<size_t>& visited) {
	auto type = t.get_type();
	switch (type) {
		case sol::type::table:
			{
				size_t hash = hashVoidPtr{}(const_cast<void*>(t.pointer()));
				if (visited.count(hash) != 0) {
					return true;
				}
				visited.insert(hash);
				auto table = t.as<sol::table>();
				std::vector<sol::object> cache;
				for (auto& kvPair : table) {
					auto keyName = kvPair.first.as<std::string>();
					cache.push_back(kvPair.second);
				}
				for (auto& value : cache) {
					if (!isSimpleValue(value, visited)) {
						return false;
					}
				}
				return true;
			}
		case sol::type::string:
		case sol::type::number:
		case sol::type::boolean:
			return true;
		case sol::type::none:
		case sol::type::lua_nil:
		case sol::type::thread:
		case sol::type::function:
		case sol::type::userdata:
		case sol::type::lightuserdata:
		case sol::type::poly:
		default:
			return false;
	}
}

void LuaActivity::start(GameState& gameState) {
	//this->applyActivityLocal(gameState, 0, gameState.smallRandom.randRange(10, 20));
	this->applyActivityLocal(gameState, 0, 3);
}

void LuaActivity::stop() {
	this->interrupt = true;
}

LuaActivity::LuaActivity(Handle self, GameState& gameState, glm::ivec2 pos) :
	SingleBlockActivity(self, gameState, pos, "debug.dds") {
	this->initializeLuaState();
}

bool LuaActivity::moveableIdleLocal() {
	return !this->moving && this->inWorld;
}

bool LuaActivity::canActivityLocal(GameState& gameState, int32_t type) {
	return true;
}

void LuaActivity::applyActivityLocalForced(GameState& gameState, int32_t type, int32_t pace) {
	this->Activity::applyActivityLocalForced(gameState, type, pace);
	this->run(gameState);
}

void LuaActivity::removeActivityTracesLocal(GameState& gameState) {
	if (!this->interrupt) {
		this->start(gameState);
	}
	else {
		this->interrupt = false;
	}
}

void LuaActivity::leaveActivityTracesLocal(GameState& gameState) {
}

ACTIVITY::TYPE LuaActivity::getType() {
	return ACTIVITY::TYPE::LUA;
}

void LuaActivity::save(Saver& saver) {
	this->SingleBlockActivity::save(saver);

	saver.store(this->interrupt);
	saver.store(this->script);

	std::unordered_set<size_t> simpleCache;
	std::unordered_set<size_t> cache;

	saver.store(this->watchedVars.size());
	for (auto& s : this->watchedVars) {
		saver.store(s);
		auto object = this->getLuaObject(s);
		if (isSimpleValue(object, simpleCache)) {
			saver.store(true);
			saver.storeObject(object, cache);
		}
		else {
			saver.store(false);
		}
	}
}

bool LuaActivity::load(Loader& loader) {
	this->SingleBlockActivity::load(loader);

	loader.retrieve(this->interrupt);
	loader.retrieve(this->script);
	this->execute(this->script);

	this->validTargets.clear();
	this->watchedVars.clear();

	size_t size;
	loader.retrieve(size);

	std::unordered_map<size_t, sol::object> cache;

	this->watchedVars.reserve(size);
	for (size_t i = 0; i < size; i++) {
		std::string s;
		loader.retrieve(s);
		bool hasValue;
		loader.retrieve(hasValue);
		if (hasValue) {
			this->state[s] = loader.retrieveObject(this->state, cache);
		}

		this->watchedVars.push_back(s);
	}
	return true;
}

bool LuaActivity::valid(Handle h) {
	return std::binary_search(this->validTargets.begin(), this->validTargets.end(), h);
}

bool LuaActivity::applyMove(Handle h, int32_t type) {
	if (!this->valid(h)) {
		return false;
	}
	else {
		return WeakReference<Activity, Activity>(h).get()->applyMoveRoot(*gameStateRef, static_cast<ACTIVITY::DIR>(type), 2);
	}
}

bool LuaActivity::applyActivity(Handle h, int32_t type) {
	if (!this->valid(h)) {
		return false;
	}
	else {
		return WeakReference<Activity, Activity>(h).get()->applyActivityLocal(*gameStateRef, type, 2);
	}
}

void LuaActivity::setPrintFunction(std::function<void(std::string line)> f) {
	this->printFunction = f;
}

void LuaActivity::prepare(GameState& gameState) {
	gameStateRef = &gameState;
	std::vector<Activity*> members;
	this->getRoot().get()->getTreeMembers(members);

	this->validTargets.clear();
	for (auto member : members) {
		this->validTargets.push_back(member->getHandle());
	}

	std::sort(this->validTargets.begin(), this->validTargets.end());
}

LuaActivity::LuaActivity() {
	this->initializeLuaState();
}

void LuaActivity::execute(std::string s) {
	try {
		this->state.safe_script(s);
	}
	catch (const sol::error& e) {
		Locator<Log>::ref().putLine(e.what());
	}
	catch (...) {
		Locator<Log>::ref().putLine("non-sol::error when executing script");
	}
}

void LuaActivity::initializeLuaState() {
	state.open_libraries(sol::lib::base, sol::lib::table, sol::lib::string);
	state.script("");
	state.set_function("move", [this](Handle h, int32_t type) -> bool
	{
		return this->applyMove(h, type);
	});

	state.set_function("activate", [this](Handle h, int32_t type) -> bool
	{
		return this->applyActivity(h, type);
	});

	state.set_function("stop", [this]() -> void
	{
		this->stop();
	});

	state.set_function("print", [this](sol::variadic_args va)
	{
		std::stringstream out;
		for (const auto& a : va) {
			auto type = a.get_type();
			switch (type) {
				case sol::type::string:
				case sol::type::number:
				case sol::type::boolean:
					out << a.as<std::string>();
					break;
				case sol::type::none:
					out << "~[none type]~";
					break;
				case sol::type::lua_nil:
					out << "~[nil]~";
					break;
				case sol::type::thread:
					out << "~[thread]~";
					break;
				case sol::type::function:
					out << "~[function]~";
					break;
				case sol::type::userdata:
					out << "~[userdata]~";
					break;
				case sol::type::lightuserdata:
					out << "~[lightuserdata]~";
					break;
				case sol::type::table:
					out << "~[table]~";
					break;
				case sol::type::poly:
					out << "~[poly]~";
					break;
				default:
					break;
			}
			out << " ";
		}
		this->printFunction(out.str());
	});
}

void LuaActivity::setWatchedVars(std::vector<std::string>& vars) {
	this->watchedVars = vars;
}

std::vector<std::string> const& LuaActivity::getWatchedVars() {
	return this->watchedVars;
}

void LuaActivity::run(GameState& gameState) {
	this->prepare(gameState);

	this->execute("run()");
}

void LuaActivity::init(GameState& gameState) {
	this->prepare(gameState);

	this->execute("init()");
}

void LuaActivity::setScript(std::string script_, GameState& gameState) {
	this->prepare(gameState);

	this->script = script_;
	this->execute(script_);
	this->init(gameState);
}

std::string const& LuaActivity::getScript() {
	return this->script;
}
