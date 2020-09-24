#include "common.h"

#include "Saver.h"

#include "ReferenceManager.h"
#include "StaticWorldChunk.h"
#include "GameState.h"
#include "ActivitySaverHelper.h"

bool Saver::storeString(std::string s) {
	size_t ss = s.size();
	store<size_t>(ss);
	this->out.write(&s[0], s.size());
	return false;
}

bool Saver::saveGame() {
	auto manager = Locator<ReferenceManager<Activity>>::get();
	save(*this, *manager);

	this->gameStateRef.save(*this);
	return true;
}

Saver::Saver(std::string file, GameState& gameState) : gameStateRef(gameState) {
	Locator<PathManager>::get()->openSave(this->out, file);
}

Saver::~Saver() {
	this->out.close();
}

