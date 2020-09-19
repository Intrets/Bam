#include "common.h"

#include "LuaStore.h"

int sol_lua_push(sol::types<LUASTORE::Args>, lua_State* L, LUASTORE::Args const& things) {
	int amount = sol::stack::push(L, 1);
	amount += sol::stack::push(L, 2);
	return amount;
}
