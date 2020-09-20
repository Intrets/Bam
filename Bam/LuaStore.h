#pragma once

#include "sol/sol.hpp"

namespace LUASTORE
{
	class Val
	{
	public:
		virtual int push(lua_State* L) = 0;

		Val() = default;
		virtual ~Val() = default;

	};

	class Integer : public Val
	{
	private:
		int64_t val;

	public:
		virtual int push(lua_State* L) override;

		Integer() = default;
		Integer(int64_t i);
		virtual	~Integer() override = default;
	};

	class Bool : public Val
	{
	private:
		bool val;

	public:
		virtual int push(lua_State* L) override;

		Bool() = default;
		Bool(bool b);
		virtual	~Bool() override = default;
	};

	class Float : public Val
	{
	private:
		double val;

	public:
		virtual int push(lua_State* L) override;

		Float() = default;
		Float(double f);
		virtual	~Float() override = default;
	};

	class String : public Val
	{
	private:
		std::string val;

	public:
		virtual int push(lua_State* L) override;

		String() = default;
		String(std::string s);
		virtual	~String() override = default;
	};

	class Args
	{
	private:
		std::vector<std::unique_ptr<Val>> args;

	public:
		std::vector<std::unique_ptr<Val>> const& getArgs() const;

		Args() = default;
		Args(sol::variadic_args va);
		~Args() = default;

		Args(Args&& other);
		Args& operator=(Args&& other);

		NOCOPY(Args);
	};
}

int sol_lua_push(sol::types<LUASTORE::Args>, lua_State* L, LUASTORE::Args const& things);
