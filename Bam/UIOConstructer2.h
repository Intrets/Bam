#pragma once

#include "ReferenceManager.h"

#include "UIOBase.h"
#include "UIOList.h"

//auto window = []()
//{
//	return 1;
//}();


//auto window = Window();
//{
//	auto list = List();
//	{
//
//
//	}
//
//}

//{
//	// window gets created and triggers something in global state
//	Window window;
//
//	{
//		// list gets created and triggers something in global state
//		List list;
//
//		{
//			// add buttons etc
//		}
//
//		{
//			// another entry in the list
//		}
//
//		// list gets destructed and this triggers something in global state
//	}
//
//
//
//	// window gets destructed and this triggers something in global state
//}

//class UIOConstructer2
//{
//};


namespace UIO2
{
	class Global
	{
	private:
		static UniqueReference<UIOBase, UIOBase> root;
		static std::vector<WeakReference<UIOBase, UIOBase>> stack;

	public:
		static void up(WeakReference<UIOBase, UIOBase> const& ref);
		static void add(UniqueReference<UIOBase, UIOBase> ref);
		static void down();
	};

	class List
	{
	public:
		List(UIO::DIR dir);
		~List();
	};
}



























