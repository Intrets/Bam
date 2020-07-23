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

bool Saver::saveGame(GameState& gameState) {
	auto manager = Locator<ReferenceManager<Activity>>::get();
	save(*this, *manager);

	gameState.save(*this);
	return true;
}

void Saver::close() {
	this->out.close();
}

Saver::Saver(std::string file) {
	Locator<PathManager>::get()->openSave(this->out, file);
}

Saver::Saver() {
}

Saver::~Saver() {
	close();
}

