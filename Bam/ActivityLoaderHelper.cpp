#include "common.h"
#include "ActivityLoaderHelper.h"
#include "Activity.h"

#include "Piston.h"
#include "Anchor.h"
#include "Platform.h"
#include "Grabber.h"
#include "RailCrane.h"
#include "LuaActivity.h"

#include "Saver.h"
#include "Loader.h"

#define LOAD(TYPE) TYPE* p = new TYPE();\
					p->load(loader);\
					manager.storeReference(p->getHandle(), p);

bool load(Loader& loader, ReferenceManager<Activity>& manager) {
	size_t size;
	loader.retrieve(size);
	for (size_t i = 0; i < size; i++) {
		ACTIVITY::TYPE type;
		loader.retrieve(type);
		switch (type) {
			case ACTIVITY::TYPE::ANCHOR:
				{
					LOAD(Anchor);
					break;
				}
			case ACTIVITY::TYPE::PLATFORM:
				{
					LOAD(Platform);
					break;
				}
			case ACTIVITY::TYPE::PISTON:
				{
					LOAD(Piston);
					break;
				}
			case ACTIVITY::TYPE::RAILCRANE:
				{
					LOAD(RailCrane);
					break;
				}
			case ACTIVITY::TYPE::LUA:
				{
					LOAD(LuaActivity);
					break;
				}
			case ACTIVITY::TYPE::GRABBER:
				{
					LOAD(Grabber);
					break;
				}
			default:
				assert(false);
				break;
		}
	}
	return true;
}
