#pragma once

#include "common.h"
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <type_traits>
#include <optional>

typedef int Handle;

template <class B, class T> class ManagedReference;
class _ManagedReferenceBase;

class _WeakReferenceBase
{
public:
	Handle handle = 0;

	void clear() { handle = 0; };

	//explicit operator bool() const {
	//	return handle != 0;
	//};

	bool isNotNull() { return handle != 0; };
	bool isNull() { return handle == 0; };
};

template <class B, class T>
class WeakReference : public _WeakReferenceBase
{
public:
	T* get();

	template<typename N>
	operator WeakReference<B, N>() const {
		//static_assert(std::is_base_of<T, N>::value, "WeakReference implicit cast: not a super class.");
		return WeakReference<B, N>(handle);
	};

	void deleteObject();

	WeakReference(Handle h) { handle = h; };
	WeakReference() = default;
	~WeakReference() = default;

};

class _ReferenceManagerBase
{

};

// base class, like window and activity
template <class B>
class ReferenceManager : public _ReferenceManagerBase
{
private:
	void freeData(Handle h);
	Handle getFreeHandle();

public:
	int size;
	typedef std::unordered_multimap<Handle, _ManagedReferenceBase*> ManagedReferencesType;

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
	void deleteReference(_WeakReferenceBase* b);

	ReferenceManager(int size_) : size(size_), usedHandle(size) {
		for (int i = 1; i < size; i++) {
			freeHandles.insert(i);
		}
	};
	ReferenceManager() : ReferenceManager(1024) {};
	~ReferenceManager() = default;
};

class _ManagedReferenceBase
{
public:
	Handle handle = 0;
	bool valid = false;
	bool isValid();

	void validate();
	void invalidate();
};

template <class B, class T>
class ManagedReference : public _ManagedReferenceBase
{
public:
	T* get();

	void unset();
	void set(Handle h);

	void set(WeakReference<B, T>& r);

	ManagedReference(Handle h);
	ManagedReference() = default;
	~ManagedReference();
	deleteDefaults(ManagedReference);

private:
	void __staticAssertions() {
		static_assert(std::is_base_of<B, T>::value, "WeakReference constructor, not a super class");
	};
};

template <class B, class T>
class ManagedNamedReference : public ManagedReference<B, T>
{
public:
	std::string name;

	bool isValid();

	void unset() = delete;
	void set(Handle h) = delete;
	void set(WeakReference<B, T>& r) = delete;

	ManagedNamedReference(std::string name_);
	ManagedNamedReference() = delete;
	~ManagedNamedReference() = default;

	deleteDefaults(ManagedNamedReference);
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
		if (it->second == static_cast<_ManagedReferenceBase*>(&managedReference)) {
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
inline void ReferenceManager<B>::deleteReference(_WeakReferenceBase* b) {
	deleteReference(b->handle);
	b->handle = 0;
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
inline bool ManagedNamedReference<B, T>::isValid() {
	if (!_ManagedReferenceBase::valid) {
		return false;
	}
	if (_ManagedReferenceBase::handle == 0) {
		ReferenceManager<B>* t = Locator<ReferenceManager<B>>::getService();
		auto it = t->indexMap.find(name);
		if (it == t->indexMap.end()) {
			_ManagedReferenceBase::valid = false;
			return false;
		}
		_ManagedReferenceBase::handle = it->second;
	}
	return true;
}

template<class B, class T>
inline ManagedNamedReference<B, T>::ManagedNamedReference(std::string name_) :
	name(name_) {
	_ManagedReferenceBase::validate();
}
