#include "common.h"

#include "Loader.h"

#include "GameState.h"
#include "Anchor.h"
#include "Platform.h"
#include "ReferenceManager.h"
#include "StaticWorldChunk.h"
#include "ActivitySerializerHelper.h"
#include "InventorySerializerHelper.h"
#include "InventoryItem.h"
#include "Inventory.h"

void Loader::addIncompleteActivityRef(Handle handle, Reference* ref) {
	ref->manager = &this->gameStateRef->getActivityManager();
	this->gameStateRef->getActivityManager().addIncomplete(handle, ref);
}

void Loader::addIncompleteInventoryRef(Handle handle, Reference* ref) {
	ref->manager = &this->gameStateRef->getInventoryItemManager();
	this->gameStateRef->getInventoryItemManager().addIncomplete(handle, ref);
}

std::istream& Loader::getBuffer() {
	return this->in;
}

void Loader::retrieveActivityPointer(Activity*& ptr) {
	int32_t handle;
	this->retrieve(handle);

	if (handle == 0) {
		ptr = nullptr;
	}
	else {
		this->gameStateRef->getActivityManager().addIncomplete(handle, ptr);
	}
}

sol::object Loader::retrieveObject(sol::state& state, std::unordered_map<size_t, sol::object>& cache) {
	sol::type type;
	this->retrieve(type);

	bool primitive;
	this->retrieve(primitive);

	std::optional<size_t> maybeHash;
	if (!primitive) {
		size_t hash;
		this->retrieve(hash);
		if (cache.count(hash) != 0) {
			return cache[hash];
		}
		maybeHash = hash;
	}

	sol::object object;
	if (type == sol::type::boolean) {
		bool value;
		this->retrieve(value);
		object = sol::make_object(state, value);
	}
	else if (type == sol::type::number) {
		bool integer;
		this->retrieve(integer);
		if (integer) {
			int64_t value;
			this->retrieve(value);
			object = sol::make_object(state, value);
		}
		else {
			double value;
			this->retrieve(value);
			object = sol::make_object(state, value);
		}
	}
	else if (type == sol::type::string) {
		std::string value;
		this->retrieve(value);
		object = sol::make_object(state, value);
	}
	else if (type == sol::type::table) {
		int32_t count;
		this->retrieve(count);

		sol::table table = state.create_table();
		object = table;
		if (maybeHash.has_value()) {
			cache[maybeHash.value()] = object;
		}
		for (int32_t i = 0; i < count; i++) {
			sol::object key = this->retrieveObject(state, cache);
			sol::object value = this->retrieveObject(state, cache);
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

void Loader::retrieveString(std::string& str) {
	size_t s;
	this->retrieve<size_t>(s);
	str.resize(s);
	this->in.read(&str[0], s);
}

bool Loader::loadGame() {
	gameStateRef->clear();

	try {
		ACTIVITYSERIALIZER::load(*this, gameStateRef->getActivityManager());
		INVENTORYSERIALIZER::load(*this, gameStateRef->getInventoryItemManager());

		this->gameStateRef->load(*this);
	}
	catch (...) {
		gameStateRef->clear();
		return false;
	}

	gameStateRef->getActivityManager().completeReferences();
	gameStateRef->getInventoryItemManager().completeReferences();

	for (auto& [h, ref] : gameStateRef->getActivityManager().data) {
		assert(h == ref.get()->getHandle());
	}

	for (auto& [h, ref] : gameStateRef->getInventoryItemManager().data) {
		assert(h == ref.get()->selfHandle);
	}

	return true;
}

GameState& Loader::getGameStateRef() {
	return *this->gameStateRef;
}

Loader::Loader(std::istream& in_, GameState& gameState) : in(in_), gameStateRef(&gameState) {
	//this->in.exceptions(std::istream::badbit | std::istream::failbit | std::istream::eofbit);
}

Loader::Loader(std::istream& in_) : in(in_) {
}

Loader::~Loader() {
}
