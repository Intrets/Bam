#pragma once

#include "ReferenceManager.h"

#include "UIOBase.h"
#include "UIOList.h"
#include "Enums.h"

struct UIOSizeType;
class UIOTextDisplay;
class UIOButton;
class UIOConstrainSize;
class UIOWindow;
class UIOHideable;
class UIOProxy;
class UIOGrid;
class UIOColoredBackground;
class UIOPad;

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

		template<class T>
		static T* addOrModifySingle();

		template<class T>
		static T* addSingle();

		static void addSingle(UniqueReference<UIOBase, UIOBase> ref);
		static void addSingle(UniqueReference<UIOBase, UIOBase> ref, WeakReference<UIOBase, UIOBase> leaf);
		static void addMulti(UniqueReference<UIOBase, UIOBase> ref);
		static void addEnd(UniqueReference<UIOBase, UIOBase> ref);

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

	UIOTextDisplay* text(std::string const& t, bool shrinkToFit = true);
	UIOButton* button(bool shrinkToFit = false);
	UIOWindow* window(std::string const& title, Rectangle size, int32_t types);
	UIOHideable* hideable();

	UIOColoredBackground* background(glm::vec4 color);

	UIOConstrainSize* constrainHeight(UIOSizeType height);
	UIOConstrainSize* constrainWidth(UIOSizeType width);
	UIOConstrainSize* align(UIO::ALIGNMENT alignment);
	UIOConstrainSize* alignCenter();
	UIOConstrainSize* alignTop();
	UIOConstrainSize* alignBottom();
	UIOConstrainSize* alignLeft();
	UIOConstrainSize* alignRight();
	UIOConstrainSize* alignBottomLeft();
	UIOConstrainSize* alignBottomRight();
	UIOConstrainSize* alignTopLeft();
	UIOConstrainSize* alignTopRight();

	UIOPad* pad(UIOSizeType padding);
	UIOPad* padTop(UIOSizeType padding);
	UIOPad* padBot(UIOSizeType padding);
	UIOPad* padLeft(UIOSizeType padding);
	UIOPad* padRight(UIOSizeType padding);

	UIOList* startList(UIO::DIR dir);
	UIOGrid* startGrid(int32_t x, int32_t y);

	UIOButton* textButton(std::string const& text);

	UIOTextDisplay* textEditSingle(std::string const& text);
	UIOTextDisplay* textEditMulti(std::vector<std::string> const& text);
	UIOTextDisplay* textDisplaySingle(std::string const& text, bool shrinkToFit = true);
	UIOTextDisplay* textDisplayMulti(std::vector<std::string> const& text);
	UIOTextDisplay* textDisplayMulti(std::string const& text);

	void endList();
	void endGrid();

	template<class T, class... Args> T* makeSingle(Args&&... args);

	template<class T> T* makeEnd(UniqueReference<UIOBase, T> ref);

	template<class T, class... Args> T* makeEnd(Args&&... args);
}

template<class T, class... Args>
T* UIO2::makeSingle(Args&&... args) {
	static_assert(std::is_base_of<UIOBase, T>::value);
	auto ref = Locator<ReferenceManager<UIOBase>>::ref().makeUniqueRef<T>(std::forward<Args>(args)...);
	auto ptr = ref.get();

	UIO2::Global::addSingle(std::move(ref));

	return ptr;
}

template<class T>
T* UIO2::makeEnd(UniqueReference<UIOBase, T> ref) {
	static_assert(std::is_base_of<UIOBase, T>::value);
	auto ptr = ref.get();

	UIO2::Global::addEnd(std::move(ref));

	return ptr;
}

template<class T, class... Args>
T* UIO2::makeEnd(Args&&... args) {
	static_assert(std::is_base_of<UIOBase, T>::value);
	auto ref = Locator<ReferenceManager<UIOBase>>::ref().makeUniqueRef<T>(std::forward<Args>(args)...);
	auto ptr = ref.get();

	UIO2::Global::addEnd(std::move(ref));

	return ptr;
}


template<class T>
T* UIO2::Global::addOrModifySingle() {
	static_assert(UIO::GET_TYPE<T>() != UIO::TYPE::UNSPECIFIED);

	WeakReference<UIOBase, UIOBase> leaf;

	if (UIO2::Global::singlesLeaf.isNull() ||
		UIO2::Global::singlesLeaf.get()->getUIOType() != UIO::GET_TYPE<T>()) {

		auto ref = Locator<ReferenceManager<UIOBase>>::ref().makeUniqueRef<T>();
		UIO2::Global::addSingle(std::move(ref));
	}

	return static_cast<T*>(UIO2::Global::singlesLeaf.get());
}

template<class T>
inline T* UIO2::Global::addSingle() {
	auto ref = Locator<ReferenceManager<UIOBase>>::ref().makeUniqueRef<T>();
	auto ptr = ref.get();
	UIO2::Global::addSingle(std::move(ref));

	return ptr;
}























