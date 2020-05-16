#pragma once

#include <fstream>
#include "Activity.h"
#include "Piston.h"
#include "RailCrane.h"
#include <functional>

#ifndef SOL_DEFINE
#define SOL_DEFINE
#include "sol/sol.hpp"
#endif 

class GameState;

class Saver
{
public:
	std::ofstream out;

	template<class T>
	bool store(T t);

	template<class A, class B>
	bool store(WeakReference<A, B> t);

	bool storeObject(sol::object object, std::unordered_set<size_t>& saved);

	bool storeString(std::string s);

	bool saveGame(GameState& gameState);

	void close();

	Saver(std::string file);
	Saver();
	~Saver();
};

template<>
inline bool Saver::store(int64_t t) {
	out.write(reinterpret_cast<char*>(&t), sizeof(t));
	return true;
}

template<>
inline bool Saver::store(int32_t t) {
	out.write(reinterpret_cast<char*>(&t), sizeof(t));
	return true;
}

template<>
inline bool Saver::store(int8_t t) {
	out.write(reinterpret_cast<char*>(&t), sizeof(t));
	return true;
}

template<>
inline bool Saver::store(size_t t) {
	store(static_cast<int64_t>(t));
	return true;
}

template<>
inline bool Saver::store(glm::ivec2 t) {
	store(static_cast<int32_t>(t.x));
	store(static_cast<int32_t>(t.y));
	return true;
}

template<>
inline bool Saver::store(bool t) {
	int8_t s = static_cast<int8_t>(t);
	store(s);
	return true;
}

template<>
inline bool Saver::store(ACTIVITY::TYPE t) {
	int32_t s = static_cast<int32_t>(t);
	store(s);
	return true;
}

template<>
inline bool Saver::store(MOVEABLE::DIR t) {
	int32_t s = static_cast<int32_t>(t);
	store(s);
	return true;
}

template<>
inline bool Saver::store(PISTON::DIR t) {
	int32_t s = static_cast<int32_t>(t);
	store(s);
	return true;
}

template<>
inline bool Saver::store(RAILCRANE::DIR t) {
	int32_t s = static_cast<int32_t>(t);
	store(s);
	return true;
}

template<>
inline bool Saver::store(std::string t) {
	return storeString(t);
}

template<>
inline bool Saver::store(sol::type type) {
	return store(static_cast<int32_t>(type));
}

inline bool Saver::storeObject(sol::object t, std::unordered_set<size_t>& saved) {
	auto type = t.get_type();
	store(type);

	bool found = false;
	// primitive
	if (t.pointer() == nullptr) {
		store<bool>(true);
	}
	// reference
	else {
		store<bool>(false);
		size_t hash = hashVoidPtr{}(const_cast<void*>(t.pointer()));
		store(hash);
		found = saved.count(hash) != 0;
		saved.insert(hash);
	}

	if (!found) {
		if (type == sol::type::boolean) {
			store(t.as<bool>());
		}
		else if (type == sol::type::number) {
			if (t.is<int>()) {
				store<bool>(true);
				store(t.as<int64_t>());
			}
			else {
				store<bool>(false);
				storeString(std::to_string(t.as<double>()));
			}
		}
		else if (type == sol::type::string) {
			storeString(t.as<std::string>());
		}
		else if (type == sol::type::table) {
			auto table = t.as<sol::table>();
			int32_t count = static_cast<int32_t>(std::distance(table.begin(), table.end()));
			store(count);

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
	return true;
}

template<class T>
inline bool Saver::store(T t) {
	assert(false);
	//out.write(reinterpret_cast<char*>(&t), sizeof(t));
	return true;
}

template<class A, class B>
inline bool Saver::store(WeakReference<A, B> t) {
	store(t.handle);
	return true;
}
