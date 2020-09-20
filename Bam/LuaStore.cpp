#include "common.h"

#include "LuaStore.h"

#include <initializer_list>

int sol_lua_push(sol::types<LUASTORE::Args>, lua_State* L, LUASTORE::Args const& things) {
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
		switch (a.get_type()) {
			case sol::type::string:
				this->args.push_back(std::make_unique<LUASTORE::String>(a.as<std::string>()));
				break;
			case sol::type::number:
				if (a.is<int>()) {
					this->args.push_back(std::make_unique<LUASTORE::Integer>(a.as<int64_t>()));
				}
				else {
					this->args.push_back(std::make_unique<LUASTORE::Float>(a.as<double>()));
				}
				break;
			case sol::type::boolean:
				this->args.push_back(std::make_unique<LUASTORE::Bool>(a.as<bool>()));
				break;
			default:
				break;
		}
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

int LUASTORE::String::push(lua_State* L) {
	return sol::stack::push(L, this->val);
}

LUASTORE::String::String(std::string s) : val(s) {
}

int LUASTORE::Float::push(lua_State* L) {
	return sol::stack::push(L, this->val);
}

LUASTORE::Float::Float(double f) : val(f) {
}

int LUASTORE::Bool::push(lua_State* L) {
	return sol::stack::push(L, this->val);
}

LUASTORE::Bool::Bool(bool b) : val(b) {
}
