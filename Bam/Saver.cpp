#include "common.h"

#include "Saver.h"

#include "ReferenceManager.h"
#include "StaticWorldChunk.h"
#include "GameState.h"
#include "ActivitySerializerHelper.h"
#include "InventoryItem.h"
#include "InventorySerializerHelper.h"
#include "Inventory.h"
#include "Activity.h"

void Saver::storeString(std::string s) {
	size_t ss = s.size();
	this->store<size_t>(ss);
	this->out.write(&s[0], s.size());
}

void Saver::storeActivityPointer(Activity* ptr) {
	if (ptr == nullptr) {
		this->store<int32_t>(0);
	}
	else {
		this->store(ptr->selfHandle);
	}
}

bool Saver::saveGame() {
	ACTIVITYSERIALIZER::save(*this, gameStateRef.getActivityManager());
	INVENTORYSERIALIZER::save(*this, gameStateRef.getInventoryItemManager());

	this->gameStateRef.save(*this);
	return true;
}

Saver::Saver(std::ostream& out_, GameState& gameState) : out(out_), gameStateRef(gameState) {
}

Saver::~Saver() {
}

