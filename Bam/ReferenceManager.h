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

	void clear() {
		handle = 0;
	};

	bool isNotNull() const {
		return handle != 0;
	};
	bool isNull() const {
		return handle == 0;
	};

	virtual ~WeakReferenceBase() {
	};
};

template <class B, class T>
class WeakReference : public WeakReferenceBase
{
public:
	T* get() const;

	template<typename N>
	operator WeakReference<B, N>() const {
		//static_assert(std::is_base_of<T, N>::value, "WeakReference implicit cast: not a super class.");
		return WeakReference<B, N>(handle);
	};

	void deleteObject();

	WeakReference() = default;

	WeakReference(Handle h) {
		handle = h;
	};
	virtual ~WeakReference() = default;
};

template<class B, class T>
class UniqueReference : public WeakReference<B, T>
{
public:
	operator WeakReference<B, T>() const {
		//static_assert(std::is_base_of<T, N>::value, "WeakReference implicit cast: not a super class.");
		return WeakReference<B, T>(WeakReference<B, T>::handle);
	};

	UniqueReference() = default;
	UniqueReference(Handle h);
	virtual ~UniqueReference();

	template<class N>
	UniqueReference(UniqueReference<B, N>&& other);

	template<class N>
	UniqueReference<B, T>& operator= (UniqueReference<B, N>&& other);

	NOCOPY(UniqueReference);
};

class ManagedReferenceBase
{
private:
	bool valid = false;

private:
	template<class T, class S>
	friend class ManagedReference;

	Handle handle = 0;

public:
	bool isValid() const;
	Handle getHandle() const;

	void validate();
	void invalidate();
	virtual ~ManagedReferenceBase() = default;
};

template <class B, class T>
class ManagedReference : public ManagedReferenceBase
{
public:
	T* get() const;

	void set(Handle h);
	void set(WeakReference<B, T> r);
	void unset();

	template<typename N>
	operator WeakReference<B, N>() const {
		//static_assert(std::is_base_of<T, N>::value, "WeakReference implicit cast: not a super class.");
		return WeakReference<B, N>(handle);
	};

	ManagedReference() = default;
	ManagedReference(Handle h);
	ManagedReference(WeakReference<B, T> r);

	ManagedReference(ManagedReference&& other);
	ManagedReference<B, T>& operator= (ManagedReference&& other);

	ManagedReference(const ManagedReference&);
	ManagedReference& operator=(const ManagedReference&);

	virtual ~ManagedReference();
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
	std::unordered_map<Handle, std::unique_ptr<B>> data;
	std::set<Handle> freeHandles;
	bool freeHandlesSorted = true;

	std::vector<bool> usedHandle;

	std::optional<WeakReference<B, B>> getRef(Handle h);

	template <class T, class... Args>
	WeakReference<B, T> makeRef(Args&&... args);

	template <class T, class... Args>
	UniqueReference<B, T> makeUniqueRef(Args&&... args);

	bool storeReference(Handle h, B* ref);

	template<class T>
	void subscribe(ManagedReference<B, T>& toManage);
	template<class T>
	void unsubscribe(ManagedReference<B, T>& managedReference);

	void deleteReference(Handle h);
	void deleteReference(WeakReferenceBase* b);

	ReferenceManager(int32_t size_) : size(size_), usedHandle(size) {
		for (int32_t i = 1; i < size; i++) {
			freeHandles.insert(i);
		}
	};
	ReferenceManager() : ReferenceManager(1024) {
	};
	~ReferenceManager();
};

template<class B, class T>
inline T* WeakReference<B, T>::get() const {
	auto& t = Locator<ReferenceManager<B>>::get()->data;
	return static_cast<T*>(t[handle].get());
}

template<class B, class T>
inline void WeakReference<B, T>::deleteObject() {
	Locator<ReferenceManager<B>>::get()->deleteReference(handle);
	handle = 0;
}

template<class B, class T>
inline T* ManagedReference<B, T>::get() const {
	return static_cast<T*>(Locator<ReferenceManager<B>>::get()->data[this->handle].get());
}

template<class B, class T>
inline void ManagedReference<B, T>::unset() {
	if (isValid()) {
		Locator<ReferenceManager<B>>::get()->unsubscribe(*this);
	}
	invalidate();
	handle = 0;
}

template<class B, class T>
inline ManagedReference<B, T>::ManagedReference(Handle h) {
	set(h);
}

template<class B, class T>
inline ManagedReference<B, T>::ManagedReference(WeakReference<B, T> r) {
	set(r);
}

template<class B, class T>
inline void ManagedReference<B, T>::set(Handle h) {
	if (isValid()) {
		Locator<ReferenceManager<B>>::get()->unsubscribe(*this);
	}
	handle = h;
	validate();
	Locator<ReferenceManager<B>>::get()->subscribe(*this);
}

template<class B, class T>
inline void ManagedReference<B, T>::set(WeakReference<B, T> r) {
	set(r.handle);
}

template<class B, class T>
inline ManagedReference<B, T>::ManagedReference(const ManagedReference& other) {
	if (other.isValid()) {
		set(other.handle);
	}
}

template<class B, class T>
inline ManagedReference<B, T>& ManagedReference<B, T>::operator=(const ManagedReference& other) {
	if (this != &other) {
		if (other.isValid()) {
			set(other.handle);
		}
	}
	return *this;
}

template<class B, class T>
inline ManagedReference<B, T>::~ManagedReference() {
	if (isValid()) {
		Locator<ReferenceManager<B>>::get()->unsubscribe(*this);
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
inline UniqueReference<B, T> ReferenceManager<B>::makeUniqueRef(Args&& ...args) {
	Handle h = getFreeHandle();
	data[h] = std::make_unique<T>(h, std::forward<Args>(args)...);
	usedHandle[h] = true;
	return UniqueReference<B, T>(h);
}

template<class B>
template<class T>
inline void ReferenceManager<B>::subscribe(ManagedReference<B, T>& toManage) {
	managedReferences.insert(std::make_pair(toManage.getHandle(), &toManage));
}

template<class B>
template<class T>
inline void ReferenceManager<B>::unsubscribe(ManagedReference<B, T>& managedReference) {
	auto range = managedReferences.equal_range(managedReference.getHandle());
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
	for_each(range.first, range.second, [](ManagedReferencesType::value_type& ref) -> void
	{
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
inline ReferenceManager<B>::~ReferenceManager() {
	for (auto [_, managed] : this->managedReferences) {
		managed->invalidate();
	}
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
inline UniqueReference<B, T>::UniqueReference(Handle h) : WeakReference<B, T>(h) {
}

template<class B, class T>
inline UniqueReference<B, T>::~UniqueReference() {
	WeakReference<B, T>::deleteObject();
}

template<class B, class T>
template<class N>
inline UniqueReference<B, T>::UniqueReference(UniqueReference<B, N>&& other) {
	this->handle = other.handle;
	other.handle = 0;
}

template<class B, class T>
template<class N>
inline UniqueReference<B, T>& UniqueReference<B, T>::operator=(UniqueReference<B, N>&& other) {
	if constexpr (std::is_same<T, N>::value) {
		if (this == &other) {
			return *this;
		}
	}
	this->deleteObject();
	this->handle = other.handle;
	other.handle = 0;
	return *this;
}

template<class B, class T>
inline ManagedReference<B, T>::ManagedReference(ManagedReference&& other) {
	if (other.isValid()) {
		this->set(other.handle);
	}
	other.unset();
}

template<class B, class T>
inline ManagedReference<B, T>& ManagedReference<B, T>::operator=(ManagedReference&& other) {
	if (this != &other) {
		if (other.isValid()) {
			this->set(other.handle);
		}
		other.unset();
	}
	return *this;
}

