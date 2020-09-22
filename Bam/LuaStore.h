#pragma once

#include "sol/sol.hpp"

class Saver;
class Loader;

namespace LUASTORE
{
	namespace VAL
	{
		enum class TYPE
		{
			INTEGER,
			FLOAT,
			BOOL,
			NIL,
			STRING,
			TABLE,
		};
	}

	using SeenTablesType = std::vector<void const*>;

	bool contains(SeenTablesType& seenTables, void const* tablePointer);

	class Val
	{
	public:
		virtual int push(lua_State* L) const = 0;
		virtual sol::object getObject(lua_State* L) const = 0;

		Val() = default;
		virtual ~Val() = default;

		virtual bool save(Saver& saver) = 0;
	};

	class Integer : public Val
	{
	private:
		int64_t val;

	public:
		virtual int push(lua_State* L) const override;

		Integer() = default;
		Integer(int64_t i);
		virtual	~Integer() override = default;

		virtual sol::object getObject(lua_State* L) const override;

		virtual bool save(Saver& saver) override;
	};

	class Bool : public Val
	{
	private:
		bool val;

	public:
		virtual int push(lua_State* L) const override;

		Bool() = default;
		Bool(bool b);
		virtual	~Bool() override = default;

		virtual sol::object getObject(lua_State* L) const override;

		virtual bool save(Saver& saver) override;
	};

	class Float : public Val
	{
	private:
		double val;

	public:
		virtual int push(lua_State* L) const override;

		Float() = default;
		Float(double f);
		virtual	~Float() override = default;

		virtual sol::object getObject(lua_State* L) const override;

		virtual bool save(Saver& saver) override;
	};

	class String : public Val
	{
	private:
		std::string val;

	public:
		virtual int push(lua_State* L) const override;

		String() = default;
		String(std::string s);
		virtual	~String() override = default;

		virtual sol::object getObject(lua_State* L) const override;

		virtual bool save(Saver& saver) override;
	};

	class Nil : public Val
	{
	public:
		virtual int push(lua_State* L) const override;

		virtual sol::object getObject(lua_State* L) const override;

		virtual bool save(Saver& saver) override;
	};


	class Table : public Val
	{
	public:
		std::vector<std::pair<std::unique_ptr<Val>, std::unique_ptr<Val>>> val;

		virtual int push(lua_State* L) const override;

		Table() = default;
		Table(sol::table table, SeenTablesType& seenTables);
		virtual	~Table() override = default;

		virtual sol::object getObject(lua_State* L) const override;

		virtual bool save(Saver& saver) override;
	};

	std::unique_ptr<Val> makeVal(sol::object a, SeenTablesType& seenTables);
	std::unique_ptr<Val> loadVal(Loader& loader);

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

		bool save(Saver& saver);
		void load(Loader& loader);

		NOCOPY(Args);
	};
}

int sol_lua_push(lua_State* L, LUASTORE::Args const& things);
int sol_lua_push(lua_State* L, LUASTORE::Val const& thing);
