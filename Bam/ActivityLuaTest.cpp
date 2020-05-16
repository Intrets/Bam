#include "common.h"

#include "ActivityLuaTest.h"

#include "GameState.h"
#include <optional>
#include "lua.h"
#include "Saver.h"
#include "Loader.h"

static std::string typeToString(sol::type type) {
	switch (type) {
		case sol::type::none:
			return "none";
			break;
		case sol::type::lua_nil:
			return "lua_nil";
			break;
		case sol::type::string:
			return "string";
			break;
		case sol::type::number:
			return "number";
			break;
		case sol::type::thread:
			return "thread";
			break;
		case sol::type::boolean:
			return "boolean";
			break;
		case sol::type::function:
			return "function";
			break;
		case sol::type::userdata:
			return "userdata";
			break;
		case sol::type::lightuserdata:
			return "lightuserdata";
			break;
		case sol::type::table:
			return "table";
			break;
		case sol::type::poly:
			return "poly";
			break;
		default:
			return "unkown";
			break;
	}
}

static bool isSimpleTable(sol::table& table) {
	for (auto& e : table) {
		sol::type t = e.second.get_type();
		if (t == sol::type::table) {
			sol::table next = e.second;
			return isSimpleTable(next);
		}
		if (!(t == sol::type::boolean || t == sol::type::number || t == sol::type::string)) {
			return false;
		}
	}
	return true;
}

static bool storeTable(Saver& saver, sol::object& table_) {
	auto table = table_.as<sol::table>();
	int32_t count = static_cast<int32_t>(std::distance(table.begin(), table.end()));

	saver.store(count);

	for (auto& e : table) {
		auto typeName = typeToString(e.first.get_type());
		saver.store<sol::object>(e.first);

		sol::type t = e.second.get_type();
		if (t == sol::type::table) {
			sol::table next = e.second;
			return storeTable(saver, next);
		}
		else {
			saver.store(e.second);
		}
	}
	return true;
}

static bool retrieveTable(sol::state& state, Loader& loader, sol::table& table) {
	int32_t count;
	loader.retrieve(count);

	for (int32_t i = 0; i < count; i++) {
		sol::type keyType;
		loader.retrieve(keyType);

		sol::object key;
		if (keyType == sol::type::boolean) {
			bool keyValue;
			loader.retrieve(keyValue);
			key = sol::make_object(state, keyValue);
		}
		else if (keyType == sol::type::number) {
			double keyValue;
			loader.retrieve(keyValue);
			key = sol::make_object(state, keyValue);
		}
		else if (keyType == sol::type::string) {
			std::string keyValue;
			loader.retrieve(keyValue);
			key = sol::make_object(state, keyValue);
		}
		else {
			assert(false);
		}

		sol::type valueType;
		loader.retrieve(valueType);
		sol::object value;
		if (valueType == sol::type::boolean) {
			bool valueValue;
			loader.retrieve(valueValue);
			value = sol::make_object(state, valueValue);
		}
		else if (valueType == sol::type::number) {
			double valueValue;
			loader.retrieve(valueValue);
			value = sol::make_object(state, valueValue);
		}
		else if (valueType == sol::type::string) {
			std::string valueValue;
			loader.retrieve(valueValue);
			value = sol::make_object(state, valueValue);
		}
		else {
			assert(false);
		}

		table[key] = value;
	}
	return true;
}

bool ActivityLuaTest::applyActivity(Handle h, int32_t type) {
	return WeakReference<Activity, Activity>(h).get()->applyActivityLocal(*gameStateRef, type, 10);
}

void ActivityLuaTest::runScript(GameState& gameState) {
	if (!target.isValid()) {
		std::cout << "no target\n";
		return;
	}
	gameStateRef = &gameState;
	state["h"] = target.handle;
}

ActivityLuaTest::ActivityLuaTest() {
	state.open_libraries(sol::lib::base, sol::lib::table);

	std::unordered_set<std::string> ignore;
	for (auto& test : state.globals()) {
		ignore.insert(test.first.as<std::string>());
	}

	std::cout << "--------\n";
	state.script(R"(
	simpleTable = {
	[1] = 1,
	[2] = 2,
	["1"] = "1",
	} 
)");

	{
		Saver saver("lua_serial.save");

		sol::object table = state["simpleTable"];

		storeTable(saver, table);
	}

	Loader loader("lua_serial.save");
	sol::table a = state.create_table();
	retrieveTable(state, loader, a);

	state["copiedTable"] = a;

	//state.create_named_table("created");
	//auto tt = state["create"];
	//tt["1"] = state.create_table();

	for (auto& test : state.globals()) {
		if (ignore.find(test.first.as<std::string>()) == ignore.end()) {
			auto name = test.first.as<std::string>();
			auto val = typeToString(test.second.get_type());

			auto s = state[name].get_type();
			switch (s) {
				case sol::type::table:
					{
						sol::table r = state[name];
						std::cout << "table name: " << name << "\n";
						std::cout << "simple table: " << isSimpleTable(r) << "\n";

						for (auto& i : r) {
							std::cout << typeToString(i.second.get_type()) << "\n";
						}
						break;
					}
				default:
					std::cout << "name: " << name << "\n";
					std::cout << typeToString(s) << "\n";
					break;
			}
			std::cout << "\n";
		}
	}

	addBind({ CONTROLS::ACTION3, CONTROLSTATE::CONTROLSTATE_PRESSED }, [](GameState& gameState, LogicSequencer* self_) {
		auto self = static_cast<ActivityLuaTest*>(self_);
		//self->state["testCall"] = 

		self->runScript(gameState);
		return std::make_pair(CONTINUATION::CONTINUE, std::nullopt);
	});
}
