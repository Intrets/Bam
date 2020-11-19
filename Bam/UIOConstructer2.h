#pragma once

#include "ReferenceManager.h"

#include "UIOBase.h"
#include "UIOList.h"

struct UIOSizeType;

namespace UIO2
{
	class Global
	{
	public:
		static UniqueReference<UIOBase, UIOBase> root;
		static std::vector<WeakReference<UIOBase, UIOBase>> stack;

		static UniqueReference<UIOBase, UIOBase> singlesRoot;
		static WeakReference<UIOBase, UIOBase> singlesLeaf;

	public:
		static void down();
		static void start(UniqueReference<UIOBase, UIOBase> ref);
		static void addSingle(UniqueReference<UIOBase, UIOBase> ref);
		static void addSingle(UniqueReference<UIOBase, UIOBase> ref, WeakReference<UIOBase, UIOBase> leaf);
		static void addMulti(UniqueReference<UIOBase, UIOBase> ref);

		static UniqueReference<UIOBase, UIOBase> finish();
	};

	class Scope
	{
	public:
		virtual ~Scope();
	};

	class List : public Scope
	{
	public:
		List(UIO::DIR dir);
	};

	class Grid : public Scope
	{
	public:
		Grid(int32_t x, int32_t y);
	};

	void text(std::string const& t);
	void constrainHeight(UIOSizeType height);
	void button();
	void window(std::string const& title, Rectangle size, int32_t types);
}



























