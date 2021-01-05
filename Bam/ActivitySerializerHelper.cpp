#include "common.h"

#include "ActivitySerializerHelper.h"

#include "ReferenceManager.h"
#include "Activity.h"
#include "Piston.h"
#include "Anchor.h"
#include "Platform.h"
#include "Grabber.h"
#include "RailCrane.h"
#include "LuaActivity.h"
#include "Reader.h"
#include "Detector.h"
#include "Incinerator.h"
#include "Forwarder.h"

#include "Saver.h"
#include "Loader.h"

#define LOAD(TYPE) TYPE* p = new TYPE();\
					p->load(loader);\
					manager.storeReference(p->getHandle(), p);

namespace ACTIVITYSERIALIZER
{
	void load(Loader& loader, ReferenceManager<Activity>& manager) {
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
				case ACTIVITY::TYPE::READER:
					{
						LOAD(Reader);
						break;
					}
				case ACTIVITY::TYPE::DETECTOR:
					{
						LOAD(Detector);
						break;
					}
				case ACTIVITY::TYPE::INCINERATOR:
					{
						LOAD(Incinerator);
						break;
					}
				case ACTIVITY::TYPE::FORWARDER:
					{
						LOAD(Forwarder);
						break;
					}
				default:
					assert(false);
					break;
			}
		}
	}

	void save(Saver& saver, ReferenceManager<Activity>& manager) {
		saver.store(manager.data.size());
		for (auto& p : manager.data) {
			p.second->save(saver);
		}
	}
}

