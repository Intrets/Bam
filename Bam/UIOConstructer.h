#pragma once

#include "ReferenceManager.h"

#include "UIOPad.h"
#include "UIOConstrainSize.h"
#include "UIOColoredBackground.h"
#include "UIOButton.h"

template<class T>
class UIOConstructer
{
private:
	UniqueReference<UIOBase, T> object;

public:
	UIOConstructer(UniqueReference<UIOBase, T> obj);
	UIOConstructer() = default;
	~UIOConstructer() = default;

	UIOConstructer<T>& setPtr(T*& ptr);
	UIOConstructer<UIOPad> addPadding(UIOSizeType padding);

	UIOConstructer<UIOConstrainSize> constrainHeight(UIOSizeType height);
	UIOConstructer<UIOConstrainSize> constrainWidth(UIOSizeType width);
	UIOConstructer<UIOConstrainSize> align(UIOConstrainSize::ALIGNMENT align);
	UIOConstructer<UIOColoredBackground> background(glm::vec4 color);
	UIOConstructer<UIOButton> button();

	UniqueReference<UIOBase, T> get();

	template<class ...Args>
	static UIOConstructer<T> makeConstructor(Args&& ...args);
};

template<class T>
inline UIOConstructer<T>::UIOConstructer(UniqueReference<UIOBase, T> obj) {
	this->object = std::move(obj);
}

template<class T>
inline UIOConstructer<T>& UIOConstructer<T>::setPtr(T*& ptr) {
	ptr = object.get();
	return *this;
}

template<class T>
inline UIOConstructer<UIOPad> UIOConstructer<T>::addPadding(UIOSizeType padding) {
	auto refMan = Locator<ReferenceManager<UIOBase>>::get();
	return UIOConstructer<UIOPad>(refMan->makeUniqueRef<UIOPad>(std::move(object), padding));
}

template<>
inline UIOConstructer<UIOConstrainSize> UIOConstructer<UIOConstrainSize>::constrainHeight(UIOSizeType height) {
	this->object.get()->maybeHeight = height;
	return UIOConstructer<UIOConstrainSize>(std::move(this->object));
}

template<class T>
inline UIOConstructer<UIOConstrainSize> UIOConstructer<T>::constrainHeight(UIOSizeType height) {
	auto refMan = Locator<ReferenceManager<UIOBase>>::get();
	auto res = refMan->makeUniqueRef<UIOConstrainSize>(std::move(this->object));
	res.get()->maybeHeight = height;
	return UIOConstructer<UIOConstrainSize>(std::move(res));
}

template<>
inline UIOConstructer<UIOConstrainSize> UIOConstructer<UIOConstrainSize>::constrainWidth(UIOSizeType width) {
	this->object.get()->maybeWidth = width;
	return UIOConstructer<UIOConstrainSize>(std::move(this->object));
}

template<class T>
inline UIOConstructer<UIOConstrainSize> UIOConstructer<T>::constrainWidth(UIOSizeType width) {
	auto refMan = Locator<ReferenceManager<UIOBase>>::get();
	auto res = refMan->makeUniqueRef<UIOConstrainSize>(std::move(this->object));
	res.get()->maybeWidth = width;
	return UIOConstructer<UIOConstrainSize>(std::move(res));
}

template<>
inline UIOConstructer<UIOConstrainSize> UIOConstructer<UIOConstrainSize>::align(UIOConstrainSize::ALIGNMENT align) {
	this->object.get()->alignment = align;
	return UIOConstructer<UIOConstrainSize>(std::move(this->object));
}

template<class T>
inline UIOConstructer<UIOConstrainSize> UIOConstructer<T>::align(UIOConstrainSize::ALIGNMENT align) {
	auto refMan = Locator<ReferenceManager<UIOBase>>::get();
	auto res = refMan->makeUniqueRef<UIOConstrainSize>(std::move(this->object));
	res.get()->alignment = align;
	return UIOConstructer<UIOConstrainSize>(std::move(res));
}

template<class T>
inline UIOConstructer<UIOColoredBackground> UIOConstructer<T>::background(glm::vec4 color) {
	auto refMan = Locator<ReferenceManager<UIOBase>>::get();
	return UIOConstructer<UIOColoredBackground>(refMan->makeUniqueRef<UIOColoredBackground>(std::move(this->object), color));
}

template<class T>
inline UIOConstructer<UIOButton> UIOConstructer<T>::button() {
	auto refMan = Locator<ReferenceManager<UIOBase>>::get();
	auto button = refMan->makeUniqueRef<UIOButton>(std::move(this->object));
	return UIOConstructer<UIOButton>(std::move(button));
}

template<class T>
inline UniqueReference<UIOBase, T> UIOConstructer<T>::get() {
	return std::move(this->object);
}

template<class T>
template<class ...Args>
inline UIOConstructer<T> UIOConstructer<T>::makeConstructor(Args && ...args) {
	return UIOConstructer(Locator<ReferenceManager<UIOBase>>::get()->makeUniqueRef<T>(std::forward<Args>(args)...));
}
