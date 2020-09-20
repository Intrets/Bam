#include "common.h"

#include "LuaStore.h"

#include <initializer_list>
#include <algorithm>

int sol_lua_push(lua_State* L, LUASTORE::Args const& things) {
	int amount = 0;
	for (auto const& arg : things.getArgs()) {
		amount += arg->push(L);
	}
	return amount;
}

std::vector<std::unique_ptr<LUASTORE::Val>> const& LUASTORE::Args::getArgs() const {
	return this->args;
}

LUASTORE::Args::Args(sol::variadic_args va) {
	for (auto a : va) {
		SeenTablesType seenTables;
		this->args.push_back(LUASTORE::makeVal(a.as<sol::object>(), seenTables));
	}
}

LUASTORE::Args::Args(Args&& other) {
	this->args = std::move(other.args);
}

LUASTORE::Args& LUASTORE::Args::operator=(Args&& other) {
	if (this == &other) {
		return *this;
	}
	else {
		this->args = std::move(other.args);
		return *this;
	}
}

int LUASTORE::Integer::push(lua_State* L) {
	return sol::stack::push(L, this->val);
}

LUASTORE::Integer::Integer(int64_t i) : val(i) {
}

sol::object LUASTORE::Integer::getObject(lua_State* L) const {
	return sol::make_object(L, this->val);
}

int LUASTORE::String::push(lua_State* L) {
	return sol::stack::push(L, this->val);
}

LUASTORE::String::String(std::string s) : val(s) {
}

sol::object LUASTORE::String::getObject(lua_State* L) const {
	return sol::make_object(L, this->val);
}

int LUASTORE::Float::push(lua_State* L) {
	return sol::stack::push(L, this->val);
}

LUASTORE::Float::Float(double f) : val(f) {
}

sol::object LUASTORE::Float::getObject(lua_State* L) const {
	return sol::make_object(L, this->val);
}

int LUASTORE::Bool::push(lua_State* L) {
	return sol::stack::push(L, this->val);
}

LUASTORE::Bool::Bool(bool b) : val(b) {
}

sol::object LUASTORE::Bool::getObject(lua_State* L) const {
	return sol::make_object(L, this->val);
}

int LUASTORE::Table::push(lua_State* L) {
	return this->getObject(L).push();
}

LUASTORE::Table::Table(sol::table table, SeenTablesType& seenTables) {
	//std::cout << table.pointer() << "\n";
	seenTables.push_back(table.pointer());
	for (auto const& kvPair : table) {
		this->val.push_back({ LUASTORE::makeVal(kvPair.first, seenTables), LUASTORE::makeVal(kvPair.second, seenTables) });
	}
	seenTables.pop_back();
}

sol::object LUASTORE::Table::getObject(lua_State* L) const {
	auto table = sol::table(L, sol::new_table{});
	for (auto& kvPair : this->val) {
		table[kvPair.first->getObject(L)] = kvPair.second->getObject(L);
	}
	return table;
}

bool LUASTORE::contains(SeenTablesType& seenTables, void const* tablePointer) {
	return std::any_of(seenTables.begin(), seenTables.end(), [tablePointer](auto ptr) -> bool
	{
		return ptr == tablePointer;
	});
}

std::unique_ptr<LUASTORE::Val> LUASTORE::makeVal(sol::object a, SeenTablesType& seenTables) {
	switch (a.get_type()) {
		case sol::type::string:
			return std::make_unique<LUASTORE::String>(a.as<std::string>());
			break;
		case sol::type::number:
			if (a.is<int>()) {
				return std::make_unique<LUASTORE::Integer>(a.as<int64_t>());
			}
			else {
				return std::make_unique<LUASTORE::Float>(a.as<double>());
			}
			break;
		case sol::type::boolean:
			return std::make_unique<LUASTORE::Bool>(a.as<bool>());
			break;
		case sol::type::table:
			{
				if (contains(seenTables, a.pointer())) {
					return std::make_unique<LUASTORE::Nil>();
				}
				else {
					return std::make_unique<LUASTORE::Table>(a.as<sol::table>(), seenTables);
				}
				break;
			}
		default:
			return std::make_unique<LUASTORE::Nil>();
			break;
	}
}

int LUASTORE::Nil::push(lua_State* L) {
	return 0;
}

sol::object LUASTORE::Nil::getObject(lua_State* L) const {
	return sol::make_object(L, sol::nil);
}

