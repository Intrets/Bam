#include "common.h"

#include "Loader.h"

#include "GameState.h"
#include "Anchor.h"
#include "Platform.h"
#include "ReferenceManager.h"
#include "StaticWorldChunk.h"
#include "ActivityLoaderHelper.h"
#include "InventorySerializerHelper.h"
#include "InventoryItem.h"
#include "Inventory.h"

void Loader::addIncompleteActivityRef(Handle handle, Reference* ref) {
	ref->manager = &this->gameStateRef.activityManager;
	this->gameStateRef.activityManager.addIncomplete(handle, ref);
}

void Loader::addIncompleteInventoryRef(Handle handle, Reference* ref) {
	ref->manager = Locator<ReferenceManager<InventoryItem>>::get();
	Locator<ReferenceManager<InventoryItem>>::get()->addIncomplete(handle, ref);
}

bool Loader::retrieveActivityPointer(Activity*& ptr) {
	int32_t handle;
	this->retrieve(handle);

	if (handle == 0) {
		ptr = nullptr;
	}
	else {
		this->gameStateRef.activityManager.addIncomplete(handle, ptr);
	}

	return true;
}

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

bool Loader::loadGame() {
	Locator<Inventory>::destroy();
	Locator<ReferenceManager<InventoryItem>>::destroy();

	{
		while (!gameStateRef.activityManager.data.empty()) {
			auto& [handle, begin] = *gameStateRef.activityManager.data.begin();

			begin.get()->getRootRef().deleteObject();
		}
	}

	gameStateRef.activityManager.clear();
	Locator<ReferenceManager<InventoryItem>>::provide(new ReferenceManager<InventoryItem>(1024));
	Locator<Inventory>::provide(new Inventory());

	load(*this, gameStateRef.activityManager);
	INVENTORYSERIALIZER::load(*this, Locator<ReferenceManager<InventoryItem>>::ref());
	Locator<Inventory>::get()->load(*this);

	this->gameStateRef.load(*this);

	gameStateRef.activityManager.completeReferences();
	Locator<ReferenceManager<InventoryItem>>::ref().completeReferences();

	for (auto& [h, ref] : gameStateRef.activityManager.data) {
		assert(h == ref.get()->getHandle());
	}

	for (auto& [h, ref] : Locator<ReferenceManager<InventoryItem>>::ref().data) {
		assert(h == ref.get()->selfHandle);
	}

	return true;
}

GameState& Loader::getGameStateRef() {
	return this->gameStateRef;
}

Loader::Loader(std::string file, GameState& gameState) : gameStateRef(gameState) {
	Locator<PathManager>::get()->openSave(in, file);
}

Loader::~Loader() {
	in.close();
}
