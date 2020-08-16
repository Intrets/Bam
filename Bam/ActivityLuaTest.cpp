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

#include "ActivitySelector.h"

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

static std::string getNameString(sol::object& object) {
	std::string name = object.as<std::string>();
	if (!object.is<int>()) {
		name = "\"" + name + "\"";
	}
	return name;
};

bool ActivityLuaTest::applyActivity(Handle h, int32_t type) {
	return WeakReference<Activity, Activity>(h).get()->applyMoveRoot(*gameStateRef, static_cast<Activity::DIR>(type), 10);
}

void ActivityLuaTest::runScript(GameState& gameState, Handle h) {
	gameStateRef = &gameState;
	state["h"] = h;
}

void ActivityLuaTest::save(Saver& saver) {
	std::unordered_set<size_t> saved;
	std::vector<std::pair<sol::object, sol::object>> cache;

	for (auto& global : state.globals()) {
		std::unordered_set<size_t> visited;
		if (ignore.find(getNameString(global.first)) == ignore.end() && isSimpleValue(global.second, visited)) {
			cache.push_back(global);
		}
	}
	saver.store(static_cast<int32_t>(cache.size()));

	for (auto& global : cache) {
		saver.storeObject(global.first, saved);
		saver.storeObject(global.second, saved);
	}
}

void ActivityLuaTest::load(Loader& loader) {
	std::unordered_map<size_t, sol::object> cache;
	int32_t size;
	loader.retrieve(size);

	for (int i = 0; i < size; i++) {
		sol::object key = loader.retrieveObject(state, cache);
		sol::object value = loader.retrieveObject(state, cache);
		Locator<Log>::ref() << Log::OPEN{} << "key: " << key.as<std::string>() << " value: " << value.as<std::string>() << "\n" << Log::CLOSE{};
		// TODO: error? no global operator[] found for sol::object
		//state[key] = value;
	}
}

ActivityLuaTest::ActivityLuaTest() {
	state.open_libraries(sol::lib::base, sol::lib::table);
	state.script("");
	state.set_function("test", [this](Handle h, int32_t type) -> bool
	{
		return this->applyActivity(h, type);
	});

	for (auto& test : state) {
		ignore.insert(getNameString(test.first));
	}
}

void ActivityLuaTest::appendRenderInfoInternal(GameState& gameState, RenderInfo& renderInfo) {
}

