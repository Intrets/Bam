#include "common.h"

#include "LuaActivity.h"
#include "GameState.h"
#include "StringHelpers.h"
#include "Saver.h"
#include "Loader.h"

#include <initializer_list>

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
	this->applyActivityLocal(gameState, 0, 5);
}

void LuaActivity::stop() {
	this->interrupt = true;
}

LuaActivity::LuaActivity(Handle self, GameState& gameState, glm::ivec2 pos) :
	SingleBlockActivity(self, pos, "debug.dds") {
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
	this->run();
}

void LuaActivity::postActivityStopLocal(GameState& gameState) {
	if (!this->interrupt) {
		this->start(gameState);
	}
	else {
		this->interrupt = false;
	}
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

	this->setScript(this->script, loader.getGameStateRef());

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

bool LuaActivity::applyMove(int32_t index, int32_t type) {
	if (index < 0 || index >= this->labelLists.size()) {
		return false;
	}
	else {
		bool res = true;
		for (auto h : this->labelLists[index]) {
			res &= WeakReference<Activity, Activity>(h).get()->applyMoveRoot(*gameStateRef, static_cast<ACTIVITY::DIR>(type), 4);
		}
		return res;
	}
}

bool LuaActivity::applyActivity(int32_t index, int32_t type) {
	if (index < 0 || index >= this->labelLists.size()) {
		return false;
	}
	else {
		bool res = true;
		for (auto h : this->labelLists[index]) {
			res &= WeakReference<Activity, Activity>(h).get()->applyActivityLocal(*gameStateRef, type, 4);
		}
		return res;
	}
}

void LuaActivity::setPrintFunction(std::function<void(std::string line)> f) {
	this->printFunction = f;
}

void LuaActivity::run() {
	if (this->memberLabelCacheTick != this->getRootPtr()->memberCache.getMembersTick() ||
		this->rootLabelCacheTick != this->memberCache.getRootTick()) {

		this->memberLabelCacheTick = this->getRootPtr()->memberCache.getMembersTick();
		this->rootLabelCacheTick = this->memberCache.getRootTick();

		for (auto const& l : this->labels) {
			state[l] = sol::nil;
		}

		this->labels.clear();
		this->labelLists.clear();

		for (auto const& member : this->getRootPtr()->getTreeMembers()) {
			auto& memberLabel = member->getLabel();
			if (memberLabel.size() > 0) {
				auto it = this->labels.find(memberLabel);
				if (it == this->labels.end()) {
					int32_t index = static_cast<int32_t>(this->labels.size());
					this->labels[memberLabel] = index;
					this->labelLists.emplace_back(std::initializer_list<int32_t>{member->getHandle()});

					try {
						state[member->getLabel()] = index;
					}
					catch (const sol::error& e) {
						Locator<Log>::ref().putLine(e.what());
						this->stop();
					}
					catch (...) {
						Locator<Log>::ref().putLine("non-sol::error when setting label values");
						this->stop();
					}
				}
				else {
					this->labelLists[it->second].push_back(member->getHandle());
				}

			}
		}
	}

	if (this->luaRunFunction.has_value()) {
		try {
			this->luaRunFunction.value()();
		}
		catch (const sol::error& e) {
			Locator<Log>::ref().putLine(e.what());
			this->stop();
		}
		catch (...) {
			Locator<Log>::ref().putLine("non-sol::error when executing script");
			this->stop();
		}
	}
	else {
		Locator<Log>::ref().putLine("No run() function found in lua script.");
	}
}

void LuaActivity::prepare(GameState& gameState) {
	gameStateRef = &gameState;
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

void LuaActivity::setScript(std::string script_, GameState& gameState) {
	this->gameStateRef = &gameState;
	this->script = script_;
	this->execute(this->script);
	this->execute("init()");
	this->refreshRunFunction();
}

void LuaActivity::refreshRunFunction() {
	sol::object func = this->state["run"];
	if (func.get_type() == sol::type::function) {
		this->luaRunFunction = func.as<sol::function>();
	}
}

std::string const& LuaActivity::getScript() {
	return this->script;
}
