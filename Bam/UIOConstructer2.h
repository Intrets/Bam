#pragma once

#include "ReferenceManager.h"

#include "UIOBase.h"
#include "UIOList.h"

struct UIOSizeType;
class UIOTextDisplay;
class UIOButton;
class UIOConstrainSize;
class UIOWindow;
class UIOHideable;
class UIOProxy;

namespace UIO2
{
	class Global
	{
	public:
		static UniqueReference<UIOBase, UIOProxy> root;
		static std::vector<WeakReference<UIOBase, UIOBase>> stack;

		static UniqueReference<UIOBase, UIOBase> singlesRoot;
		static WeakReference<UIOBase, UIOBase> singlesLeaf;

	public:
		static void down();
		static void start(UniqueReference<UIOBase, UIOBase> ref);
		static void start();
		static void addSingle(UniqueReference<UIOBase, UIOBase> ref);
		static void addSingle(UniqueReference<UIOBase, UIOBase> ref, WeakReference<UIOBase, UIOBase> leaf);
		static void addMulti(UniqueReference<UIOBase, UIOBase> ref);

		static UniqueReference<UIOBase, UIOBase> end();
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

	UIOTextDisplay* text(std::string const& t);
	UIOConstrainSize* constrainHeight(UIOSizeType height);
	UIOButton* button();
	UIOWindow* window(std::string const& title, Rectangle size, int32_t types);
	UIOHideable* hideable();
}



























