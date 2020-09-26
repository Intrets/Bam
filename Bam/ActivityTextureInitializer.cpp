#include "common.h"

#include "ActivityTextureInitializer.h"
#include "Grabber.h"
#include "BlockIDTextures.h"
#include "Piston.h"
#include "RailCrane.h"

void ActivityTextureInitializer::run() {
	auto man = Locator<BlockIDTextures>::get();
	Grabber::textureActive = man->getBlockTextureID("grabber_right_a.dds");
	Grabber::textureInactive = man->getBlockTextureID("grabber_right.dds");
}
