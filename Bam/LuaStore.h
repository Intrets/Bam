#pragma once

#include "sol/sol.hpp"

namespace LUASTORE
{
	class Val
	{

	};

	class Integer : public Val
	{

	};

	class Args
	{
	private:
		std::vector<std::unique_ptr<Val>> args;
	public:
		Args() = default;
		~Args() = default;
	};
}

int sol_lua_push(sol::types<LUASTORE::Args>, lua_State* L, LUASTORE::Args const& things);
