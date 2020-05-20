#pragma once

#include "common.h"

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <type_traits>
#include <optional>
#include <iostream>

typedef int32_t Handle;

template <class B, class T> class ManagedReference;
class ManagedReferenceBase;

class WeakReferenceBase
{
public:
	Handle handle = 0;

	void clear() { handle = 0; };

	bool isNotNull() { return handle != 0; };
	bool isNull() { return handle == 0; };

	virtual ~WeakReferenceBase() {};
};

template <class B, class T>
class WeakReference : public WeakReferenceBase
{
public:
	T* get();

	template<typename N>
	operator WeakReference<B, N>() const {
		//static_assert(std::is_base_of<T, N>::value, "WeakReference implicit cast: not a super class.");
		return WeakReference<B, N>(handle);
	};

	void deleteObject();

	WeakReference(T* t);
	WeakReference(Handle h) { handle = h; };
	WeakReference() = default;
	virtual ~WeakReference() = default;
};

template<class B, class T>
class UniqueReference : public WeakReference<B, T>
{
public:
	UniqueReference(Handle h) : WeakReference<B, T>(h) {};
	UniqueReference() = default;
	virtual ~UniqueReference();

	UniqueReference(UniqueReference&& other);
	UniqueReference<B, T>& operator= (UniqueReference&& other);

	NOCOPY(UniqueReference);
};

class ManagedReferenceBase
{
public:
	Handle handle = 0;
	bool valid = false;
	bool isValid();

	void validate();
	void invalidate();
	virtual ~ManagedReferenceBase() = default;
};

template <class B, class T>
class ManagedReference : public ManagedReferenceBase
{
	static_assert(std::is_base_of<B, T>::value, "WeakReference constructor, not a super class");
public:
	T* get();

	void unset();
	void set(Handle h);
	void set(WeakReference<B, T>& r);

	ManagedReference(Handle h);
	ManagedReference(WeakReference<B, T>& r);
	ManagedReference() = default;
	virtual ~ManagedReference();
	NOCOPYMOVE(ManagedReference);
};

// base class, like window and activity
template <class B>
class ReferenceManager
{
private:
	void freeData(Handle h);
	Handle getFreeHandle();

public:
	int32_t size;
	typedef std::unordered_multimap<Handle, ManagedReferenceBase*> ManagedReferencesType;

	ManagedReferencesType managedReferences;
	std::unordered_map<std::string, Handle> indexMap;
	std::unordered_map<Handle, std::unique_ptr<B>> data;
	std::set<Handle> freeHandles;
	bool freeHandlesSorted = true;

	std::vector<bool> usedHandle;

	std::optional<WeakReference<B, B>> getRef(Handle h);

	template <class T, class... Args>
	WeakReference<B, T> makeRef(Args&&... args);
	template <class T, class... Args>
	WeakReference<B, T> makeNamedRef(std::string name, Args&&... args);

	bool storeReference(Handle h, B* ref);

	template<class T>
	void subscribe(ManagedReference<B, T>& toManage, Handle h);
	template<class T>
	void unsubscribe(ManagedReference<B, T>& managedReference);

	void deleteReference(Handle h);
	void deleteReference(WeakReferenceBase* b);

	Handle storeObject(B* obj);


	ReferenceManager(int32_t size_) : size(size_), usedHandle(size) {
		for (int32_t i = 1; i < size; i++) {
			freeHandles.insert(i);
		}
	};
	ReferenceManager() : ReferenceManager(1024) {};
	~ReferenceManager() = default;
};

template<class B, class T>
inline T* WeakReference<B, T>::get() {
	auto& t = Locator<ReferenceManager<B>>::getService()->data;
	return static_cast<T*>(t[handle].get());
}

template<class B, class T>
inline void WeakReference<B, T>::deleteObject() {
	Locator<ReferenceManager<B>>::getService()->deleteReference(handle);
	handle = 0;
}

template<class B, class T>
inline WeakReference<B, T>::WeakReference(T* t) {
	handle = Locator<ReferenceManager<B>>::getService()->storeObject(t);
}

template<class B, class T>
inline T* ManagedReference<B, T>::get() {
	return static_cast<T*>(Locator<ReferenceManager<B>>::getService()->data[handle].get());
}

template<class B, class T>
inline void ManagedReference<B, T>::unset() {
	if (isValid()) {
		Locator<ReferenceManager<B>>::getService()->unsubscribe(*this);
	}
	invalidate();
	handle = 0;
}

template<class B, class T>
inline void ManagedReference<B, T>::set(Handle h) {
	if (isValid()) {
		Locator<ReferenceManager<B>>::getService()->unsubscribe(*this);
	}
	handle = h;
	validate();
	Locator<ReferenceManager<B>>::getService()->subscribe(*this, h);
}

template<class B, class T>
inline void ManagedReference<B, T>::set(WeakReference<B, T>& r) {
	set(r.handle);
}

template<class B, class T>
inline ManagedReference<B, T>::ManagedReference(Handle h) {
	set(h);
}

template<class B, class T>
inline ManagedReference<B, T>::ManagedReference(WeakReference<B, T>& r) {
	set(r);
}

template<class B, class T>
inline ManagedReference<B, T>::~ManagedReference() {
	if (isValid()) {
		Locator<ReferenceManager<B>>::getService()->unsubscribe(*this);
	}
}

template<class B>
template<class T, class ...Args>
inline WeakReference<B, T> ReferenceManager<B>::makeRef(Args&& ...args) {
	Handle h = getFreeHandle();
	data[h] = std::make_unique<T>(h, std::forward<Args>(args)...);
	usedHandle[h] = true;
	return WeakReference<B, T>(h);
}

template<class B>
template<class T, class ...Args>
inline WeakReference<B, T> ReferenceManager<B>::makeNamedRef(std::string name, Args&& ...args) {
	WeakReference<B, T> ref = makeRef<T>(std::forward<Args>(args)...);
	indexMap[name] = ref.handle;
	return ref;
}

template<class B>
template<class T>
inline void ReferenceManager<B>::subscribe(ManagedReference<B, T>& toManage, Handle h) {
	managedReferences.insert(std::make_pair(h, &toManage));
}

template<class B>
template<class T>
inline void ReferenceManager<B>::unsubscribe(ManagedReference<B, T>& managedReference) {
	auto range = managedReferences.equal_range(managedReference.handle);
	auto it = range.first;
	auto end = range.second;
	for (; it != end; it++) {
		if (it->second == static_cast<ManagedReferenceBase*>(&managedReference)) {
			managedReferences.erase(it);
			break;
		}
	}
}

template<class B>
inline std::optional<WeakReference<B, B>> ReferenceManager<B>::getRef(Handle h) {
	if (indexInVector(h, usedHandle) && usedHandle[h]) {
		return WeakReference<B, B>(h);
	}
	return {};
}

template<class B>
inline bool ReferenceManager<B>::storeReference(Handle h, B* ref) {
	if (freeHandles.count(h) == 0) {
		return false;
	}
	freeHandles.erase(h);
	usedHandle[h] = true;
	data[h] = std::unique_ptr<B>(ref);
	return true;
}

template<class B>
inline void ReferenceManager<B>::deleteReference(Handle h) {
	if (h == 0) {
		return;
	}
	auto range = managedReferences.equal_range(h);
	for_each(range.first, range.second, [](ManagedReferencesType::value_type& ref) -> void {
		ref.second->invalidate();
	});
	freeData(h);
}

template<class B>
inline void ReferenceManager<B>::deleteReference(WeakReferenceBase* b) {
	deleteReference(b->handle);
	b->handle = 0;
}

template<class B>
inline Handle ReferenceManager<B>::storeObject(B* obj) {
	Handle h = getFreeHandle();
	data[h] = std::unique_ptr<B>(obj);
	usedHandle[h] = true;
	return h;
}

template<class B>
inline void ReferenceManager<B>::freeData(Handle h) {
	data.erase(h);
	usedHandle[h] = false;
	freeHandles.insert(h);
}

template<class B>
inline Handle ReferenceManager<B>::getFreeHandle() {
	auto it = freeHandles.begin();
	Handle h = *it;
	freeHandles.erase(it);
	return h;
}

template<class B, class T>
inline UniqueReference<B, T>::~UniqueReference() {
	std::cout << "deleting UniqueReference\n";
	WeakReference<B, T>::deleteObject();
}

template<class B, class T>
inline UniqueReference<B, T>::UniqueReference(UniqueReference&& other) {
	WeakReferenceBase::handle = other.handle;
	other.handle = 0;
}

template<class B, class T>
inline UniqueReference<B, T>& UniqueReference<B, T>::operator=(UniqueReference&& other) {
	if (this != &other) {
		WeakReference<B, T>::deleteObject();
		WeakReference<B, T>::handle = other.handle;
		other.handle = 0;
	}
	return *this;
}
