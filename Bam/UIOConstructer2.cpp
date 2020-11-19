#include "common.h"

#include "UIOConstructer2.h"

UniqueReference<UIOBase, UIOBase> UIO2::Global::root;
std::vector<WeakReference<UIOBase, UIOBase>> UIO2::Global::stack;

UIO2::List::List(UIO::DIR dir) {
	auto ref = Locator<ReferenceManager<UIOBase>>::ref().makeUniqueRef<UIOList>(dir);
	UIO2::Global::up(ref);
	UIO2::Global::add(std::move(ref));
}

UIO2::List::~List() {
}

void UIO2::Global::up(WeakReference<UIOBase, UIOBase> const& ref) {
	UIO2::Global::stack.push_back(ref);
}

void UIO2::Global::add(UniqueReference<UIOBase, UIOBase> ref) {
	UIO2::Global::stack.back().get()->addElement(std::move(ref));
}

void UIO2::Global::down() {
	UIO2::Global::stack.pop_back();
}
