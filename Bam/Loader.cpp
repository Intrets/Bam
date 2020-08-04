#include "common.h"
#include "GameState.h"
#include "Loader.h"
#include "Anchor.h"
#include "Platform.h"
#include "ReferenceManager.h"
#include "StaticWorldChunk.h"
#include "ActivityLoaderHelper.h"

sol::object Loader::retrieveObject(sol::state& state, std::unordered_map<size_t, sol::object>& cache) {
	sol::type type;
	retrieve(type);

	bool primitive;
	retrieve(primitive);

	std::optional<size_t> maybeHash;
	if (!primitive) {
		size_t hash;
		retrieve(hash);
		if (cache.count(hash) != 0) {
			return cache[hash];
		}
		maybeHash = hash;
	}

	sol::object object;
	if (type == sol::type::boolean) {
		bool value;
		retrieve(value);
		object = sol::make_object(state, value);
	}
	else if (type == sol::type::number) {
		bool integer;
		retrieve(integer);
		if (integer) {
			int64_t value;
			retrieve(value);
			object = sol::make_object(state, value);
		}
		else {
			double value;
			retrieve(value);
			object = sol::make_object(state, value);
		}
	}
	else if (type == sol::type::string) {
		std::string value;
		retrieve(value);
		object = sol::make_object(state, value);
	}
	else if (type == sol::type::table) {
		int32_t count;
		retrieve(count);

		sol::table table = state.create_table();
		object = table;
		if (maybeHash.has_value()) {
			cache[maybeHash.value()] = object;
		}
		for (int32_t i = 0; i < count; i++) {
			sol::object key = retrieveObject(state, cache);
			sol::object value = retrieveObject(state, cache);
			table[key] = value;
		}
		object = table;
	}
	else {
		assert(false);
	}

	if (maybeHash.has_value()) {
		cache[maybeHash.value()] = object;
	}

	return object;
}

bool Loader::retrieveString(std::string& str) {
	size_t s;
	retrieve<size_t>(s);
	str.resize(s);
	in.read(&str[0], s);
	return false;
}

bool Loader::loadGame(GameState& gameState) {
	auto manager = new ReferenceManager<Activity>(1024);
	load(*this, *manager);
	Locator<ReferenceManager<Activity>>::provide(manager);

	gameState.load(*this);
	return true;
}

Loader::Loader(std::string file) {
	Locator<PathManager>::get()->openSave(in, file);
}

Loader::Loader() {
}

Loader::~Loader() {
	in.close();
}
