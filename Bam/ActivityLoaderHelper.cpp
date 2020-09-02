#include "common.h"
#include "ActivityLoaderHelper.h"
#include "Activity.h"

//#include "Breaker.h"
#include "Piston.h"
//#include "Loader.h"
#include "Anchor.h"
//#include "Mover.h"
#include "Platform.h"
//#include "SinglePlatform.h"
//#include "Grabber.h"
#include "RailCrane.h"

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
				//case ACTIVITY::TYPE::MOVER:
				//	{
				//		LOAD(Mover);
				//		break;
				//	}
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
				//case Activity::Type::SINGLEPLATFORM:
				//	{
				//		LOAD(SinglePlatform);
				//		break;
				//	}
				//case Activity::Type::BREAKER:
				//	{
				//		LOAD(Breaker);
				//		break;
				//	}
				//case Activity::Type::GRABBER:
				//	{
				//		LOAD(Grabber);
				//		break;
				//	}
			default:
				assert(false);
				break;
		}
	}
	return true;
}
