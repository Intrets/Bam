#include "common.h"
#include "ActivityLoaderHelper.h"
#include "Activity.h"

//#include "Breaker.h"
//#include "Piston.h"
//#include "Loader.h"
#include "Anchor.h"
//#include "Mover.h"
#include "Platform.h"
//#include "SinglePlatform.h"
//#include "Grabber.h"

#define LOAD(TYPE) TYPE* p = new TYPE();\
					p->load(loader);\
					manager.storeReference(p->getHandle(), p);

bool load(Loader& loader, ReferenceManager<Activity>& manager) {
	int size;
	loader.retrieve(size);
	for (int i = 0; i < size; i++) {
		ACTIVITY::TYPE type;
		loader.retrieve(type);
		switch (type) {
			case ACTIVITY::ANCHOR:
				{
					LOAD(Anchor);
					break;
				}
			//case ACTIVITY::MOVER:
			//	{
			//		LOAD(Mover);
			//		break;
			//	}
			case ACTIVITY::PLATFORM:
				{
					LOAD(Platform);
					break;
				}
			//case ACTIVITY::PISTON:
			//	{
			//		LOAD(Piston);
			//		break;
			//	}
			//case ACTIVITY::SINGLEPLATFORM:
			//	{
			//		LOAD(SinglePlatform);
			//		break;
			//	}
			//case ACTIVITY::BREAKER:
			//	{
			//		LOAD(Breaker);
			//		break;
			//	}
			//case ACTIVITY::GRABBER:
			//	{
			//		LOAD(Grabber);
			//		break;
			//	}
		}
	}
	return true;
}
