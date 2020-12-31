#pragma once

#include "ReferenceManager.h"

#include "UIOBase.h"
#include "UIOList.h"
#include "Enums.h"
#include "UIOSizeType.h"
#include "UIOProxy.h"

struct UIOSizeType;
class UIOTextDisplay;
class UIOButton;
class UIOConstrainSize;
class UIOWindow;
class UIOHideable;
class UIOGrid;
class UIOColoredBackground;
class UIOFreeSize;
class UIOPad;
class UIODestructible;

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

		ReferenceManager<UIOBase>& getManager();

		template<class T>
		void pop();

		template<class T>
		WeakReference<UIOBase, T> addOrModifySingle();

		template<class T>
		WeakReference<UIOBase, T> addSingle();
	};

	class Global
	{
	public:
		static std::vector<std::unique_ptr<ConstructerState>> states;

		static ConstructerState* getState();

		static void push();

		static UniqueReference<UIOBase, UIOBase> pop();
	};

	WeakReference<UIOBase, UIOTextDisplay> text(std::string const& t, bool shrinkToFit = true);
	WeakReference<UIOBase, UIOButton> button(bool shrinkToFit = false);
	WeakReference<UIOBase, UIOWindow> window(std::string const& title, Rectangle size, int32_t types);
	WeakReference<UIOBase, UIOHideable> hideable();

	WeakReference<UIOBase, UIOColoredBackground> background(glm::vec4 color);

	WeakReference<UIOBase, UIOConstrainSize> constrainHeight(UIOSizeType height);
	WeakReference<UIOBase, UIOConstrainSize> constrainWidth(UIOSizeType width);
	WeakReference<UIOBase, UIOConstrainSize> constrainSize(UIOSizeType size);
	WeakReference<UIOBase, UIOConstrainSize> align(UIO::ALIGNMENT alignment);
	WeakReference<UIOBase, UIOConstrainSize> alignCenter();
	WeakReference<UIOBase, UIOConstrainSize> alignTop();
	WeakReference<UIOBase, UIOConstrainSize> alignBottom();
	WeakReference<UIOBase, UIOConstrainSize> alignLeft();
	WeakReference<UIOBase, UIOConstrainSize> alignRight();
	WeakReference<UIOBase, UIOConstrainSize> alignBottomLeft();
	WeakReference<UIOBase, UIOConstrainSize> alignBottomRight();
	WeakReference<UIOBase, UIOConstrainSize> alignTopLeft();
	WeakReference<UIOBase, UIOConstrainSize> alignTopRight();

	WeakReference<UIOBase, UIOFreeSize> free();

	WeakReference<UIOBase, UIODestructible> destructible();

	WeakReference<UIOBase, UIOPad> pad(UIOSizeType padding);
	WeakReference<UIOBase, UIOPad> padTop(UIOSizeType padding);
	WeakReference<UIOBase, UIOPad> padBot(UIOSizeType padding);
	WeakReference<UIOBase, UIOPad> padLeft(UIOSizeType padding);
	WeakReference<UIOBase, UIOPad> padRight(UIOSizeType padding);

	WeakReference<UIOBase, UIOList> startList(UIO::DIR dir);
	WeakReference<UIOBase, UIOGrid> startGrid(int32_t x, int32_t y);
	WeakReference<UIOBase, UIOList> menu(std::string const& text, std::optional<UIOSizeType> width, std::function<void()> f);

	WeakReference<UIOBase, UIOButton> textButton(std::string const& text);
	std::pair<WeakReference<UIOBase, UIOButton>, WeakReference<UIOBase, UIOTextDisplay>> textButton2(std::string const& text);

	WeakReference<UIOBase, UIOTextDisplay> textEditSingle(std::string const& text);
	WeakReference<UIOBase, UIOTextDisplay> textEditMulti(std::vector<std::string> const& text, bool lineWrap = true);
	WeakReference<UIOBase, UIOTextDisplay> textEditMulti(std::string const& text, bool lineWrap = true);
	WeakReference<UIOBase, UIOTextDisplay> textDisplaySingle(std::string const& text, bool shrinkToFit = true);
	WeakReference<UIOBase, UIOTextDisplay> textDisplayMulti(std::vector<std::string> const& text, bool lineWrap = true);
	WeakReference<UIOBase, UIOTextDisplay> textDisplayMulti(std::string const& text, bool lineWrap = true);

	void endList();
	void endGrid();

	template<class T, class... Args> WeakReference<UIOBase, T> makeSingle(Args&&... args);

	template<class T> WeakReference<UIOBase, T> makeEnd(UniqueReference<UIOBase, T> ref);

	template<class T, class... Args> WeakReference<UIOBase, T> makeEnd(Args&&... args);
}

template<class T, class... Args>
WeakReference<UIOBase, T> UIO2::makeSingle(Args&&... args) {
	static_assert(std::is_base_of<UIOBase, T>::value);
	auto ref = Locator<ReferenceManager<UIOBase>>::ref().makeUniqueRef<T>(std::forward<Args>(args)...);
	auto ptr = ref.get();

	UIO2::Global::getState()->addSingle(std::move(ref));

	return ptr;
}

template<class T>
WeakReference<UIOBase, T> UIO2::makeEnd(UniqueReference<UIOBase, T> ref) {
	static_assert(std::is_base_of<UIOBase, T>::value);
	auto res = ref.as<T>();

	UIO2::Global::getState()->addEnd(std::move(ref));

	return res;
}

template<class T, class... Args>
WeakReference<UIOBase, T> UIO2::makeEnd(Args&&... args) {
	static_assert(std::is_base_of<UIOBase, T>::value);
	auto ref = UIO2::Global::getState()->getManager().makeUniqueRef<T>(std::forward<Args>(args)...);
	auto res = ref.as<T>();

	UIO2::Global::getState()->addEnd(std::move(ref));

	return res;
}

template<class T>
inline void UIO2::ConstructerState::pop() {
	assert(this->stack.size() > 1);
	assert(this->stack.back().get()->getUIOType() == UIO::GET_TYPE<T>());

	UIO2::ConstructerState::stack.pop_back();
}

template<class T>
WeakReference<UIOBase, T> UIO2::ConstructerState::addOrModifySingle() {
	static_assert(UIO::GET_TYPE<T>() != UIO::TYPE::UNSPECIFIED);

	WeakReference<UIOBase, UIOBase> leaf;

	if (this->singlesLeaf.isNull() ||
		this->singlesLeaf.get()->getUIOType() != UIO::GET_TYPE<T>()) {

		this->addSingle(std::move(UIO2::Global::getState()->getManager().makeUniqueRef<T>()));
	}

	return this->singlesLeaf.as<T>();
}

template<class T>
inline WeakReference<UIOBase, T> UIO2::ConstructerState::addSingle() {
	auto ref = UIO2::Global::getState()->getManager().makeUniqueRef<T>();
	auto res = ref.as<T>();

	this->addSingle(std::move(ref));

	return res;
}























