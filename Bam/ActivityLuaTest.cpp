#include "common.h"

#include "ActivityLuaTest.h"

#include "GameState.h"
#include <optional>
#include "lua.h"
#include "Saver.h"
#include "Loader.h"
#include <iostream>
#include "RenderInfo.h"
#include "StringHelpers.h"

#include "LuaActivity.h"

#include <sol/sol.hpp>

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

//static std::string getNameString(sol::object& object) {
//	std::string name = object.as<std::string>();
//	if (!object.is<int>()) {
//		name = "\"" + name + "\"";
//	}
//	return name;
//};

bool ActivityLuaTest::valid(Handle h) {
	return std::binary_search(this->validTargets.begin(), this->validTargets.end(), h);
}

bool ActivityLuaTest::applyMove(Handle h, int32_t type) {
	if (!this->valid(h)) {
		return false;
	}
	else {
		return WeakReference<Activity, Activity>(h).get()->applyMoveRoot(*gameStateRef, static_cast<ACTIVITY::DIR>(type), 9);
	}
}

bool ActivityLuaTest::applyActivity(Handle h, int32_t type) {
	if (!this->valid(h)) {
		return false;
	}
	else {
		return WeakReference<Activity, Activity>(h).get()->applyActivityLocal(*gameStateRef, type, 9);
	}
}

void ActivityLuaTest::setPrintFunction(std::function<void(std::string line)> f) {
	this->printFunction = f;
}

void ActivityLuaTest::prepare(GameState& gameState) {
	gameStateRef = &gameState;
	std::vector<Activity*> members;
	this->target->getTreeMembers(members);

	this->validTargets.clear();
	for (auto member : members) {
		this->validTargets.push_back(member->getHandle());
	}

	std::sort(this->validTargets.begin(), this->validTargets.end());
}

void ActivityLuaTest::save(Saver& saver) {
	//std::unordered_set<size_t> saved;
	//std::vector<std::pair<sol::object, sol::object>> cache;

	//for (auto& global : state.globals()) {
	//	std::unordered_set<size_t> visited;
	//	if (ignore.find(getNameString(global.first)) == ignore.end() && isSimpleValue(global.second, visited)) {
	//		cache.push_back(global);
	//	}
	//}
	//saver.store(static_cast<int32_t>(cache.size()));

	//for (auto& global : cache) {
	//	saver.storeObject(global.first, saved);
	//	saver.storeObject(global.second, saved);
	//}
}

void ActivityLuaTest::load(Loader& loader) {
	//std::unordered_map<size_t, sol::object> cache;
	//int32_t size;
	//loader.retrieve(size);

	//for (int i = 0; i < size; i++) {
	//	sol::object key = loader.retrieveObject(state, cache);
	//	sol::object value = loader.retrieveObject(state, cache);
	//	Locator<Log>::ref().putStreamLine(std::stringstream() << "key: " << key.as<std::string>() << " value: " << value.as<std::string>());
	//}
}

ActivityLuaTest::ActivityLuaTest() {
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

ActivityLuaTest::ActivityLuaTest(LuaActivity& ptr) :
	ActivityLuaTest() {
	this->target = &ptr;
}

void ActivityLuaTest::appendRenderInfoInternal(GameState& gameState, RenderInfo& renderInfo) {
}

void ActivityLuaTest::execute(std::string s) {
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

void ActivityLuaTest::run(GameState& gameState) {
	this->prepare(gameState);

	this->execute("run()");
}

void ActivityLuaTest::init(GameState& gameState) {
	this->prepare(gameState);

	this->execute("init()");
}

void ActivityLuaTest::setScript(std::string script_, GameState& gameState) {
	this->prepare(gameState);

	this->script = script_;
	this->execute(script_);
	this->init(gameState);
}

std::string const& ActivityLuaTest::getScript() {
	return this->script;
}


