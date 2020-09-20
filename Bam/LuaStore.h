#pragma once

#include "sol/sol.hpp"

namespace LUASTORE
{
	using SeenTablesType = std::vector<void const*>;

	bool contains(SeenTablesType& seenTables, void const* tablePointer);

	class Val
	{
	public:
		virtual int push(lua_State* L) = 0;
		virtual sol::object getObject(lua_State* L) const = 0;

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

		virtual sol::object getObject(lua_State* L) const override;

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

		virtual sol::object getObject(lua_State* L) const override;
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

		virtual sol::object getObject(lua_State* L) const override;
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

		virtual sol::object getObject(lua_State* L) const override;
	};

	class Nil : public Val
	{
	public:
		virtual int push(lua_State* L) override;

		virtual sol::object getObject(lua_State* L) const override;
	};


	class Table : public Val
	{
	private:
		std::vector<std::pair<std::unique_ptr<Val>, std::unique_ptr<Val>>> val;

	public:
		virtual int push(lua_State* L) override;

		Table() = default;
		Table(sol::table table, SeenTablesType& seenTables);
		virtual	~Table() override = default;

		virtual sol::object getObject(lua_State* L) const override;
	};

	std::unique_ptr<Val> makeVal(sol::object a, SeenTablesType& seenTables);

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

int sol_lua_push(lua_State* L, LUASTORE::Args const& things);
