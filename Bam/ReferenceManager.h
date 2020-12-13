#pragma once

#include "common.h"

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <type_traits>
#include <optional>
#include <iostream>

#define RTTI_CHECKS

#ifdef RTTI_CHECKS
#include <typeinfo>
#endif

typedef int32_t Handle;

template <class B>
class ReferenceManager;

class Reference
{
public:
	void* ptr = nullptr;

	bool operator==(Reference const& other) const;
	bool operator==(void* other) const;

	void clear() {
		this->ptr = nullptr;
	};

	bool isNotNull() const {
		return this->ptr != nullptr;
	};
	bool isNull() const {
		return this->ptr == nullptr;
	};

	virtual ~Reference() = default;
};

template <class B, class T>
class WeakReference : public Reference
{
public:
	T* get() const;

	Handle getHandle() const;

	template<class R>
	R* getAs() const;

	template<typename N>
	WeakReference<B, N> as() const;

	operator bool() const;

	template<typename N>
	operator WeakReference<B, N>() const;

	void deleteObject();

	WeakReference() = default;

	WeakReference(Handle h);
	WeakReference(void* p);
	WeakReference(T& o);

	virtual ~WeakReference() = default;
};

template<class B, class T>
class UniqueReference : public WeakReference<B, T>
{
public:
	UniqueReference() = default;

	UniqueReference(WeakReference<B, T> ref);

	UniqueReference<B, T>& operator= (WeakReference<B, T> const& other);

	virtual ~UniqueReference();

	template<class N>
	UniqueReference(UniqueReference<B, N>&& other);

	template<class N>
	UniqueReference<B, T>& operator= (UniqueReference<B, N>&& other);

	NOCOPY(UniqueReference);
};

template <class B, class T>
class ManagedReference : private WeakReference<B, T>
{
private:
	friend class ReferenceManager<B>;

	void setVoid(void* p);

public:
	WeakReference<B, T> getRef() const;

	void set(WeakReference<B, T> r);

	bool isValid() const;
	void invalidate();

	void unset();

	ManagedReference() = default;

	ManagedReference(WeakReference<B, T> r);
	ManagedReference(T&);

	ManagedReference(ManagedReference&& other) noexcept;
	ManagedReference<B, T>& operator= (ManagedReference&& other) noexcept;

	ManagedReference(const ManagedReference&);
	ManagedReference& operator=(const ManagedReference&);

	virtual ~ManagedReference();
};

template <class B>
class ReferenceManager
{
private:
	void freeData(Handle h);
	Handle getFreeHandle();

	std::vector<std::pair<Handle, Reference*>> incomplete;

public:
	void addIncomplete(Handle h, Reference* ptr);
	void completeReferences();

	int32_t size;
	typedef std::unordered_multimap<Handle, Reference*> ManagedReferencesType;

	ManagedReferencesType managedReferences;
	std::unordered_map<Handle, std::unique_ptr<B>> data;
	std::set<Handle> freeHandles;
	bool freeHandlesSorted = true;

	std::vector<bool> usedHandle;

	template<class T>
	T* getPtr(Handle h);

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

	template<class T>
	void deleteReference(WeakReference<B, T>& ref);

	template<class T>
	void deleteReference(UniqueReference<B, T>& ref);

	template<class T>
	void deleteReference(ManagedReference<B, T>& ref);

	ReferenceManager(int32_t size_) : size(size_), usedHandle(size) {
		for (int32_t i = 1; i < size; i++) {
			freeHandles.insert(i);
		}
	};
	ReferenceManager() : ReferenceManager(10'000) {
	};
	~ReferenceManager();
};

template<class B, class T>
inline T* WeakReference<B, T>::get() const {
	return static_cast<T*>(this->ptr);
}

template<class B, class T>
inline Handle WeakReference<B, T>::getHandle() const {
	return this->get()->selfHandle;
}

template<class B, class T>
inline WeakReference<B, T>::operator bool() const {
	return this->isNotNull();
}

template<class B, class T>
inline void WeakReference<B, T>::deleteObject() {
	if (this->isNotNull()) {
		Locator<ReferenceManager<B>>::get()->deleteReference(this->getHandle());
		this->ptr = nullptr;
	}
}

template<class B, class T>
inline WeakReference<B, T>::WeakReference(Handle h) {
	this->ptr = Locator<ReferenceManager<B>>::get()->data[h].get();
}

template<class B, class T>
inline WeakReference<B, T>::WeakReference(void* p) {
	this->ptr = p;
}

template<class B, class T>
inline WeakReference<B, T>::WeakReference(T& o) {
	this->ptr = &o;
}

template<class B, class T>
inline void ManagedReference<B, T>::setVoid(void* p) {
	if (this->isValid()) {
		Locator<ReferenceManager<B>>::get()->unsubscribe(*this);
	}
	this->ptr = p;
	Locator<ReferenceManager<B>>::get()->subscribe(*this);
}

template<class B, class T>
inline bool ManagedReference<B, T>::isValid() const {
	return this->WeakReference<B, T>::isNotNull();
}

template<class B, class T>
inline void ManagedReference<B, T>::invalidate() {
	this->ptr = nullptr;
}

template<class B, class T>
inline void ManagedReference<B, T>::unset() {
	if (this->isValid()) {
		Locator<ReferenceManager<B>>::get()->unsubscribe(*this);
	}
	this->invalidate();
}

template<class B, class T>
inline ManagedReference<B, T>::ManagedReference(WeakReference<B, T> r) {
	this->set(r);
}

template<class B, class T>
inline ManagedReference<B, T>::ManagedReference(T& o) {
	this->set(o);
}

template<class B, class T>
inline WeakReference<B, T> ManagedReference<B, T>::getRef() const {
	return *this;
}

template<class B, class T>
inline void ManagedReference<B, T>::set(WeakReference<B, T> r) {
	this->setVoid(r.ptr);
}

template<class B, class T>
inline ManagedReference<B, T>::ManagedReference(const ManagedReference& other) {
	if (auto o = other.getRef()) {
		this->set(o);
	}
}

template<class B, class T>
inline ManagedReference<B, T>& ManagedReference<B, T>::operator=(const ManagedReference& other) {
	if (this != &other) {
		if (auto o = other.getRef()) {
			this->set(o);
		}
	}
	return *this;
}

template<class B, class T>
inline ManagedReference<B, T>::~ManagedReference() {
	if (this->isValid()) {
		Locator<ReferenceManager<B>>::get()->unsubscribe(*this);
	}
}

template<class B>
template<class T>
inline T* ReferenceManager<B>::getPtr(Handle h) {
	return static_cast<T*>(data[h].get());
}

template<class B>
template<class T, class ...Args>
inline WeakReference<B, T> ReferenceManager<B>::makeRef(Args&& ...args) {
	Handle h = this->getFreeHandle();
	this->data[h] = std::make_unique<T>(h, std::forward<Args>(args)...);
	this->usedHandle[h] = true;
	return WeakReference<B, T>(this->data[h].get());
}

template<class B>
template<class T, class ...Args>
inline UniqueReference<B, T> ReferenceManager<B>::makeUniqueRef(Args&& ...args) {
	Handle h = this->getFreeHandle();
	this->data[h] = std::make_unique<T>(h, std::forward<Args>(args)...);
	this->usedHandle[h] = true;
	return UniqueReference<B, T>(this->data[h].get());
}

template<class B>
template<class T>
inline void ReferenceManager<B>::subscribe(ManagedReference<B, T>& toManage) {
#ifdef _DEBUG
	auto range = managedReferences.equal_range(toManage.getHandle());

	auto it = range.first;
	auto end = range.second;

	for (; it != end; it++) {
		assert(it->second != static_cast<Reference*>(&toManage));
	}
#endif

	this->managedReferences.insert(std::make_pair(toManage.getHandle(), static_cast<Reference*>(&toManage)));
}

template<class B>
template<class T>
inline void ReferenceManager<B>::unsubscribe(ManagedReference<B, T>& managedReference) {
	auto range = managedReferences.equal_range(managedReference.getHandle());

#ifdef _DEBUG
	{
		auto it = range.first;
		auto end = range.second;
		int32_t count = 0;

		for (; it != end; it++) {
			if (it->second == dynamic_cast<Reference*>(&managedReference)) {
				count++;
			}
		}

		assert(count == 1);
	}
#endif

	auto it = range.first;
	auto end = range.second;

	for (; it != end; it++) {
		if (it->second == dynamic_cast<Reference*>(&managedReference)) {
			it = managedReferences.erase(it);
			break;
		}
	}
}

template<class B>
template<class T>
inline void ReferenceManager<B>::deleteReference(WeakReference<B, T>& ref) {
	this->deleteReference(ref.getHandle());
	ref.clear();
}

template<class B>
template<class T>
inline void ReferenceManager<B>::deleteReference(UniqueReference<B, T>& ref) {
	this->deleteReference(ref.getHandle());
	ref.clear();
}

template<class B>
template<class T>
inline void ReferenceManager<B>::deleteReference(ManagedReference<B, T>& ref) {
	this->deleteReference(ref.getHandle());
	ref.clear();
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
	auto range = this->managedReferences.equal_range(h);
	for_each(range.first, range.second, [](ManagedReferencesType::value_type& ref) -> void
	{
		ref.second->clear();
	});

	this->managedReferences.erase(range.first, range.second);

	this->freeData(h);
}

template<class B>
inline ReferenceManager<B>::~ReferenceManager() {
	for (auto [_, managed] : this->managedReferences) {
		managed->clear();
	}

	while (!this->data.empty()) {
		this->data.erase(this->data.begin());
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

template<class B>
inline void ReferenceManager<B>::addIncomplete(Handle h, Reference* ptr) {
	this->incomplete.push_back({ h, ptr });
}

template<class B>
inline void ReferenceManager<B>::completeReferences() {
	for (auto [h, ptr] : this->incomplete) {
		ptr->ptr = this->data[h].get();
	}
	this->incomplete.clear();
}

template<class B, class T>
inline UniqueReference<B, T>::UniqueReference(WeakReference<B, T> ref) : WeakReference<B, T>(ref) {
}

template<class B, class T>
inline UniqueReference<B, T>& UniqueReference<B, T>::operator=(WeakReference<B, T> const& other) {
	assert(this->isNull());
	this->ptr = other.ptr;
	return *this;
}

template<class B, class T>
inline UniqueReference<B, T>::~UniqueReference() {
	WeakReference<B, T>::deleteObject();
}

template<class B, class T>
template<class N>
inline UniqueReference<B, T>::UniqueReference(UniqueReference<B, N>&& other) {
	this->ptr = other.ptr;
	other.ptr = nullptr;
}

template<class B, class T>
template<class N>
inline UniqueReference<B, T>& UniqueReference<B, T>::operator=(UniqueReference<B, N>&& other) {
	if constexpr (std::is_same<T, N>::value) {
		if (this == &other) {
			return *this;
		}
	}
	assert(this->ptr != other.ptr);
	this->deleteObject();
	this->ptr = other.ptr;
	other.ptr = nullptr;
	return *this;
}

template<class B, class T>
inline ManagedReference<B, T>::ManagedReference(ManagedReference&& other) noexcept {
	if (auto o = other.getRef()) {
		this->setVoid(o.ptr);
	}
	other.unset();
}

template<class B, class T>
inline ManagedReference<B, T>& ManagedReference<B, T>::operator=(ManagedReference&& other) noexcept {
	if (this != &other) {
		if (auto o = other.getRef()) {
			this->setVoid(o.ptr);
		}
		other.unset();
	}
	return *this;
}

template<class B, class T>
template<class R>
inline R* WeakReference<B, T>::getAs() const {
#ifdef RTTI_CHECKS
	if (R* p = dynamic_cast<R*>(this->get())) {
		return p;
	}
	else {
		assert(0);
	}
#endif
	return static_cast<R*>(this->ptr);
}

template<class B, class T>
template<typename N>
inline WeakReference<B, N> WeakReference<B, T>::as() const {
#ifdef RTTI_CHECKS
	if (N* p = dynamic_cast<N*>(this->get())) {
	}
	else {
		assert(0);
	}
#endif
	return WeakReference<B, N>(this->ptr);
}

template<class B, class T>
template<typename N>
inline WeakReference<B, T>::operator WeakReference<B, N>() const {
	static_assert(std::is_base_of<N, T>::value, "WeakReference implicit cast: not a super class.");
	return WeakReference<B, N>(this->ptr);
}

inline bool Reference::operator==(Reference const& other) const {
	return this->ptr == other.ptr;
}

inline bool Reference::operator==(void* other) const {
	return this->ptr == other;
}
