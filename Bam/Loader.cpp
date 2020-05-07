#include "common.h"
#include "GameState.h"
//#include "Breaker.h"
//#include "Piston.h"
#include "Loader.h"
#include "Anchor.h"
//#include "Mover.h"
#include "Platform.h"
#include "ReferenceManager.h"
#include "StaticWorldChunk.h"
//#include "SinglePlatform.h"
//#include "Grabber.h"
//#include "ScriptManager.h"
#include "ActivityLoaderHelper.h"


#define LOAD(TYPE) TYPE* p = new TYPE();\
					p->load(*this);\
					manager->storeReference(p->selfHandle, p);

bool Loader::retrieveString(std::string& str) {
	int s;
	retrieve<int>(s);
	str.resize(s);
	in.read(&str[0], s);
	return false;
}

bool Loader::loadGame(GameState& gameState) {
	//auto s = new ScriptManager();
	//s->load(*this);
	//Locator<ScriptManager>::provide(s);

	auto manager = new ReferenceManager<Activity>(1024);
	load(*this, *manager);
	Locator<ReferenceManager<Activity>>::provide(manager);

	gameState.load(*this);
	return true;
}

Loader::Loader(std::string file) : in(file, std::ifstream::binary) {

}

Loader::Loader() {
}


Loader::~Loader() {
	in.close();
}