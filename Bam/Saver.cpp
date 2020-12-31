#include "common.h"

#include "Saver.h"

#include "ReferenceManager.h"
#include "StaticWorldChunk.h"
#include "GameState.h"
#include "ActivitySaverHelper.h"
#include "InventoryItem.h"
#include "InventorySerializerHelper.h"
#include "Inventory.h"
#include "Activity.h"

bool Saver::storeString(std::string s) {
	size_t ss = s.size();
	store<size_t>(ss);
	this->out.write(&s[0], s.size());
	return false;
}

bool Saver::storeActivityPointer(Activity* ptr) {
	if (ptr == nullptr) {
		this->store<int32_t>(0);
	}
	else {
		this->store(ptr->selfHandle);
	}
	return true;
}

bool Saver::saveGame() {
	{
		save(*this, gameStateRef.activityManager);
	}

	{
		auto manager = Locator<ReferenceManager<InventoryItem>>::get();
		INVENTORYSERIALIZER::save(*this, *manager);
	}

	{
		Locator<Inventory>::ref().save(*this);
	}

	this->gameStateRef.save(*this);
	return true;
}

Saver::Saver(std::string file, GameState& gameState) : gameStateRef(gameState) {
	Locator<PathManager>::get()->openSave(this->out, file);
}

Saver::~Saver() {
	this->out.close();
}

