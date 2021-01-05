#pragma once

#include <fstream>
#include <functional>
#include <unordered_set>
#include <optional>

#include <sol/sol.hpp>

#include "Enums.h"

#include "ReferenceManager.h"

class GameState;
class Activity;

class Saver
{
private:
	std::ostream& out;

	GameState& gameStateRef;

	template<class A, class B>
	void store(ManagedReference<A, B> const& t);

public:
	void storeActivityPointer(Activity* ptr);

	template<class T>
	void store(T t);

	template<class A, class B>
	void store(WeakReference<A, B> const& t);

	template<class A, class B>
	void store(UniqueReference<A, B> const& t);

	template<class T>
	void store(std::optional<T>& maybe);

	void storeObject(sol::object object, std::unordered_set<size_t>& saved);

	void storeString(std::string s);

	bool saveGame();

	Saver(std::ostream& out_, GameState& gameState);
	Saver() = delete;
	~Saver();
};

template<>
inline void Saver::store(double t) {
	this->storeString(std::to_string(t));
}

template<>
inline void Saver::store(int64_t t) {
	this->out.write(reinterpret_cast<char*>(&t), sizeof(t));
}

template<>
inline void Saver::store(int32_t t) {
	this->out.write(reinterpret_cast<char*>(&t), sizeof(t));
}

template<>
inline void Saver::store(int8_t t) {
	this->out.write(reinterpret_cast<char*>(&t), sizeof(t));
}

template<>
inline void Saver::store(size_t t) {
	this->store(static_cast<int64_t>(t));
}

template<>
inline void Saver::store(glm::ivec2 t) {
	this->store(static_cast<int32_t>(t.x));
	this->store(static_cast<int32_t>(t.y));
}

template<>
inline void Saver::store(glm::vec2 t) {
	this->store(static_cast<double>(t.x));
	this->store(static_cast<double>(t.y));
}

template<>
inline void Saver::store(bool t) {
	int8_t s = static_cast<int8_t>(t);
	this->store(s);
}

template<>
inline void Saver::store(ACTIVITY::TYPE t) {
	int32_t s = static_cast<int32_t>(t);
	this->store(s);
}

template<>
inline void Saver::store(ACTIVITY::DIR t) {
	int32_t s = static_cast<int32_t>(t);
	this->store(s);
}

template<>
inline void Saver::store(PISTON::DIR t) {
	int32_t s = static_cast<int32_t>(t);
	this->store(s);
}

template<>
inline void Saver::store(RAILCRANE::DIR t) {
	int32_t s = static_cast<int32_t>(t);
	this->store(s);
}

template<>
inline void Saver::store(INVENTORYITEM::TYPE t) {
	int32_t s = static_cast<int32_t>(t);
	this->store(s);
}

template<>
inline void Saver::store(std::string t) {
	this->storeString(t);
}

template<>
inline void Saver::store(sol::type type) {
	this->store(static_cast<int32_t>(type));
}

inline void Saver::storeObject(sol::object t, std::unordered_set<size_t>& saved) {
	auto type = t.get_type();
	this->store(type);

	bool found = false;
	// primitive
	if (t.pointer() == nullptr) {
		this->store<bool>(true);
	}
	// reference
	else {
		this->store<bool>(false);
		size_t hash = hashVoidPtr{}(const_cast<void*>(t.pointer()));
		this->store(hash);
		found = saved.count(hash) != 0;
		saved.insert(hash);
	}

	if (!found) {
		if (type == sol::type::boolean) {
			this->store(t.as<bool>());
		}
		else if (type == sol::type::number) {
			if (t.is<int>()) {
				this->store<bool>(true);
				this->store(t.as<int64_t>());
			}
			else {
				this->store<bool>(false);
				this->store(t.as<double>());
			}
		}
		else if (type == sol::type::string) {
			storeString(t.as<std::string>());
		}
		else if (type == sol::type::table) {
			auto table = t.as<sol::table>();
			int32_t count = static_cast<int32_t>(std::distance(table.begin(), table.end()));
			this->store(count);

			std::vector<std::pair<sol::object, sol::object>> cache;
			for (auto& e : table) {
				cache.push_back(e);
			}
			for (auto& e : cache) {
				storeObject(e.first, saved);
				storeObject(e.second, saved);
			}
		}
		else {
			assert(false);
		}

	}
}

template<class T>
inline void Saver::store(T t) {
	assert(0);
}

template<class A, class B>
inline void Saver::store(WeakReference<A, B> const& t) {
	if (t) {
		this->store(t.getHandle());
	}
	else {
		this->store(0);
	}
}

template<class A, class B>
inline void Saver::store(UniqueReference<A, B> const& t) {
	if (t) {
		this->store(t.getHandle());
	}
	else {
		this->store(0);
	}
}

template<class A, class B>
inline void Saver::store(ManagedReference<A, B> const& t) {
	assert(0);
}

template<class T>
inline void Saver::store(std::optional<T>& maybe) {
	if (maybe.has_value()) {
		this->store(true);
		maybe.value().save(*this);
	}
	else {
		this->store(false);
	}

}
