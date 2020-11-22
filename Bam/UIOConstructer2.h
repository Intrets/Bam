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
class UIOFreeSize;
class UIOPad;

namespace UIO2
{
	struct ConstructerState
	{
		UniqueReference<UIOBase, UIOProxy> root;
		std::vector<WeakReference<UIOBase, UIOBase>> stack;

		UniqueReference<UIOBase, UIOBase> singlesRoot;
		WeakReference<UIOBase, UIOBase> singlesLeaf;

		void addSingle(UniqueReference<UIOBase, UIOBase> ref);
		void addSingle(UniqueReference<UIOBase, UIOBase> ref, WeakReference<UIOBase, UIOBase> leaf);
		void addMulti(UniqueReference<UIOBase, UIOBase> ref);
		void addEnd(UniqueReference<UIOBase, UIOBase> ref);

		template<class T>
		void pop();

		template<class T>
		T* addOrModifySingle();

		template<class T>
		T* addSingle();
	};

	class Global
	{
	public:
		static std::vector<std::unique_ptr<ConstructerState>> states;

		static ConstructerState* getState();

		static void push();

		static UniqueReference<UIOBase, UIOBase> pop();
	};

	UIOTextDisplay* text(std::string const& t, bool shrinkToFit = true);
	UIOButton* button(bool shrinkToFit = false);
	UIOWindow* window(std::string const& title, Rectangle size, int32_t types);
	UIOHideable* hideable();

	UIOColoredBackground* background(glm::vec4 color);

	UIOConstrainSize* constrainHeight(UIOSizeType height);
	UIOConstrainSize* constrainWidth(UIOSizeType width);
	UIOConstrainSize* constrainSize(UIOSizeType size);
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

	UIOFreeSize* free();

	UIOPad* pad(UIOSizeType padding);
	UIOPad* padTop(UIOSizeType padding);
	UIOPad* padBot(UIOSizeType padding);
	UIOPad* padLeft(UIOSizeType padding);
	UIOPad* padRight(UIOSizeType padding);

	UIOList* startList(UIO::DIR dir);
	UIOGrid* startGrid(int32_t x, int32_t y);

	UIOButton* textButton(std::string const& text);
	std::pair<UIOButton*, UIOTextDisplay*> textButton2(std::string const& text);

	UIOTextDisplay* textEditSingle(std::string const& text);
	UIOTextDisplay* textEditMulti(std::vector<std::string> const& text, bool lineWrap = true);
	UIOTextDisplay* textEditMulti(std::string const& text, bool lineWrap = true);
	UIOTextDisplay* textDisplaySingle(std::string const& text, bool shrinkToFit = true);
	UIOTextDisplay* textDisplayMulti(std::vector<std::string> const& text, bool lineWrap = true);
	UIOTextDisplay* textDisplayMulti(std::string const& text, bool lineWrap = true);

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

	UIO2::Global::getState()->addSingle(std::move(ref));

	return ptr;
}

template<class T>
T* UIO2::makeEnd(UniqueReference<UIOBase, T> ref) {
	static_assert(std::is_base_of<UIOBase, T>::value);
	auto ptr = ref.get();

	UIO2::Global::getState()->addEnd(std::move(ref));

	return ptr;
}

template<class T, class... Args>
T* UIO2::makeEnd(Args&&... args) {
	static_assert(std::is_base_of<UIOBase, T>::value);
	auto ref = Locator<ReferenceManager<UIOBase>>::ref().makeUniqueRef<T>(std::forward<Args>(args)...);
	auto ptr = ref.get();

	UIO2::Global::getState()->addEnd(std::move(ref));

	return ptr;
}

template<class T>
inline void UIO2::ConstructerState::pop() {
	assert(this->stack.size() > 1);
	assert(this->stack.back().get()->getUIOType() == UIO::GET_TYPE<T>());

	UIO2::ConstructerState::stack.pop_back();
}

template<class T>
T* UIO2::ConstructerState::addOrModifySingle() {
	static_assert(UIO::GET_TYPE<T>() != UIO::TYPE::UNSPECIFIED);

	WeakReference<UIOBase, UIOBase> leaf;

	if (this->singlesLeaf.isNull() ||
		this->singlesLeaf.get()->getUIOType() != UIO::GET_TYPE<T>()) {

		auto ref = Locator<ReferenceManager<UIOBase>>::ref().makeUniqueRef<T>();
		this->addSingle(std::move(ref));
	}

	return static_cast<T*>(this->singlesLeaf.get());
}

template<class T>
inline T* UIO2::ConstructerState::addSingle() {
	auto ref = Locator<ReferenceManager<UIOBase>>::ref().makeUniqueRef<T>();
	auto ptr = ref.get();
	this->addSingle(std::move(ref));

	return ptr;
}























