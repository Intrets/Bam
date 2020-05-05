#pragma once

#include <sstream>
#include <unordered_map>
#include <iostream>
#include "GameState.h"
#include "Anchor.h"
#include "ReferenceManager.h"

class Modifyable
{
public:
	bool readOnly = false;

	virtual std::ostream& toStream(void* base_, std::ostream& out) { return out; };

	std::string toString(void* base_) {
		std::stringstream out;
		toStream(base_, out);
		return out.str();
	};

	virtual bool modify(void* base, GameState& gameState, std::vector<std::string>& value) {
		return false;
	};
};

class ModifyableAddChild : public Modifyable
{
public:
	using SetterType = void (Anchor::*)(int);
	SetterType setter;

	virtual std::ostream& toStream(void* base_, std::ostream& out) override {
		Anchor* base = static_cast<Anchor*>(base_);
		for (auto& child : base->children) {
			out << child.handle << " ";
		}
		return out;
	}

	virtual bool modify(void* base_, GameState& gameState, std::vector<std::string>& value) {
		if (readOnly) return false;
		Anchor* base = static_cast<Anchor*>(base_);
		int h;
		try {
			h = std::stoi(value[0]);
		}
		catch (std::exception) {
			return false;
		}
		auto ref = Locator<ReferenceManager<Activity>>::getService()->getRef(h);
		if (ref) {
			(base->*setter)(h);
		}
		return true;
	}

	ModifyableAddChild(SetterType setter_) : setter(setter_) {};
	~ModifyableAddChild() = default;
};

template <class T>
class ModifyableDoActivity : public Modifyable
{
public:
	using SetterType = bool (T::*)(GameState&, int, int);

	virtual bool modify(void* base_, GameState& gameState, std::vector<std::string>& value) {
		if (readOnly) return false;
		T* base = static_cast<T*>(base_);
		int type;
		try {
			type = std::stoi(value[0]);
		}
		catch (std::exception) {
			return false;
		}
		int pace = 10;
		if (value.size() > 1) {
			try {
				pace = std::stoi(value[1]);
			}
			catch (std::exception) {

			}
		}
		(base->*setter)(gameState, type, pace);
		return true;
	}

	SetterType setter;

	ModifyableDoActivity(SetterType setter_) : setter(setter_) {};
	~ModifyableDoActivity() = default;
};

template <class T>
class ModifyableDoRootMove : public Modifyable
{
public:
	using SetterType = bool (T::*)(GameState&, MOVEABLE::DIR, int);

	SetterType setter;

	virtual bool modify(void* base_, GameState& gameState, std::vector<std::string>& value) {
		if (readOnly) return false;
		T* base = static_cast<T*>(base_);
		MOVEABLE::DIR direction;
		if (value.size() == 0) {
			return false;
		}
		if (value[0] == "up") {
			direction = MOVEABLE::DIR::UP;
		}
		else if (value[0] == "down") {
			direction = MOVEABLE::DIR::DOWN;
		}
		else if (value[0] == "left") {
			direction = MOVEABLE::DIR::LEFT;
		}
		else if (value[0] == "right") {
			direction = MOVEABLE::DIR::RIGHT;
		}
		else {
			return false;
		}
		int pace = 10;
		if (value.size() > 1) {
			try {
				pace = std::stoi(value[1]);
			}
			catch (std::exception) {

			}
		}
		(base->*setter)(gameState, direction, pace);
		return true;
	}

	ModifyableDoRootMove(SetterType setter_) : setter(setter_) {};
	~ModifyableDoRootMove() = default;
};

template <class T, class R>
class ModifyableFuncWeakRef : public Modifyable
{
public:
	using SetType = WeakReference<Activity, Activity>;
	using SetterType = bool (T::*)(SetType);
	using GetType = WeakReference<Activity, R>;
	using GetterLocation = GetType(T::*);

	SetterType setter;
	GetterLocation getter;

	virtual std::ostream& toStream(void* base_, std::ostream& out) override {
		T* base = static_cast<T*>(base_);
		auto t = static_cast<GetType*>(&(base->*getter));
		out << t->handle;
		return out;
	}
	virtual bool modify(void* base_, GameState& gameState, std::vector<std::string>& value) {
		if (readOnly) return false;
		T* base = static_cast<T*>(base_);
		MOVEABLE::DIR direction;
		if (value.size() == 0) {
			return false;
		}
		int h;
		try {
			h = std::stoi(value[0]);
		}
		catch (std::exception) {
			return false;
		}
		(base->*setter)(WeakReference<Activity, Activity>(h));
		return true;
	}
	
	ModifyableFuncWeakRef(SetterType setter_, GetterLocation getter_) : setter(setter_), getter(getter_) {};
	~ModifyableFuncWeakRef() = default;
};

template <class T>
class ModifyableAnchorRef : public Modifyable
{
public:
	using Member = WeakReference<Activity, Anchor>;
	using Type = Member(T::*);

	Type ptr;

	virtual std::ostream& toStream(void* base_, std::ostream& out) {
		T* base = static_cast<T*>(base_);
		auto t = static_cast<Member*>(&(base->*ptr));
		out << t->handle;
		return out;
	};

	virtual bool modify(void* base_, GameState& gameState, std::vector<std::string>& value) {
		try {
			int h = std::stoi(value[0]);

			T* base = static_cast<T*>(base_);
			auto t = static_cast<Member*>(&(base->*ptr));

			auto ref = Locator<ReferenceManager<Activity>>::getService()->getRef(h);

			if (ref) {
				*t = ref.value();
			}
		}
		catch (std::exception) {
			return false;
		}
		return true;
	};

	ModifyableAnchorRef(Type ptr_) : ptr(ptr_) {
		readOnly = false;
	};
	~ModifyableAnchorRef() = default;
};

template <class T>
class ModifyableInt : public Modifyable
{
public:
	int T::* ptr;

	virtual std::ostream& toStream(void* base_, std::ostream& out) {
		T* base = static_cast<T*>(base_);
		auto t = static_cast<int*>(&(base->*ptr));
		out << *t;
		return out;
	};

	virtual bool modify(void* base_, GameState& gameState, std::vector<std::string>& value) override {
		if (readOnly) return false;
		T* base = static_cast<T*>(base_);
		auto t = static_cast<int*>(&(base->*ptr));
		try {
			*t = std::stoi(value[0]);
			return true;
		}
		catch (std::exception) {
			return false;
		}
	};
	ModifyableInt(int T::* ptr_) : ptr(ptr_) {};
	~ModifyableInt() = default;
};

template <class T>
class ModifyableDIR : public Modifyable
{
public:
	using Member = MOVEABLE::DIR;
	using Type = Member(T::*);

	Type ptr;

	virtual bool modify(void* base_, GameState& gameState, std::vector<std::string>& value) override {
		if (readOnly) return false;
		if (value.size() == 0) {
			return false;
		}
		T* base = static_cast<T*>(base_);
		auto t = static_cast<Member*>(&(base->*ptr));
		MOVEABLE::DIR direction;
		if (value[0] == "up") {
			direction = MOVEABLE::DIR::UP;
		}
		else if (value[0] == "down") {
			direction = MOVEABLE::DIR::DOWN;
		}
		else if (value[0] == "left") {
			direction = MOVEABLE::DIR::LEFT;
		}
		else if (value[0] == "right") {
			direction = MOVEABLE::DIR::RIGHT;
		}
		else {
			return false;
		}
		*t = direction;
		return true;
	};

	virtual std::ostream& toStream(void* base_, std::ostream& out) {
		T* base = static_cast<T*>(base_);
		auto t = static_cast<Member*>(&(base->*ptr));
		out << MOVEABLE::DIR_NAMES[*t];
		return out;
	};

	ModifyableDIR(Type ptr_) : ptr(ptr_) {};
	~ModifyableDIR() = default;
};

template <class T>
class ModifyableIVec2 : public Modifyable
{
public:
	glm::ivec2 T::* ptr;

	virtual std::ostream& toStream(void* base_, std::ostream& out) {
		T* base = static_cast<T*>(base_);
		auto t = static_cast<glm::ivec2*>(&(base->*ptr));
		out << t->x << " " << t->y;
		return out;
	}

	virtual bool modify(void* base_, GameState& gameState, std::vector<std::string>& value) override {
		if (readOnly) return false;
		T* base = static_cast<T*>(base_);
		auto t = static_cast<glm::ivec2*>(&(base->*ptr));
		glm::ivec2 newt;
		try {
			newt.x = std::stoi(value[0]);
			newt.y = std::stoi(value[1]);
		}
		catch (std::exception) {
			return false;
		}
		*t = newt;
		return true;
	};

	ModifyableIVec2(glm::ivec2 T::* ptr_) : ptr(ptr_) {};
	~ModifyableIVec2() = default;
};


class ModifyerBase
{
public:
	std::unordered_map<std::string, std::unique_ptr<Modifyable>> modifyables;
};

template <class T>
class Modifyer : public ModifyerBase
{

};
