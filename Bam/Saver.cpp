#include "common.h"
#include "Saver.h"
#include "ReferenceManager.h"
#include "StaticWorldChunk.h"
#include "GameState.h"
//#include "ScriptManager.h"
#include "ActivitySaverHelper.h"

bool Saver::storeString(std::string s) {
	int ss = s.size();
	store<int>(ss);
	out.write(&s[0], s.size());
	return false;
}

bool Saver::saveGame(GameState& gameState) {
	//Locator<ScriptManager>::getService()->save(*this);

	auto manager = Locator<ReferenceManager<Activity>>::getService();
	save(*this, *manager);

	gameState.save(*this);
	return true;
}

void Saver::close() {
	out.close();
}

Saver::Saver(std::string file) : out(file, std::ofstream::binary) {
}

Saver::Saver() {
}

Saver::~Saver() {
	close();
}
