#include "common.h"

#include "ActivityCopier.h"
#include "ReferenceManager.h"
#include "Piston.h"
#include "Anchor.h"
#include "RailCrane.h"
#include "LuaActivity.h"
#include "SingleBlockActivity.h"
#include "Grabber.h"
#include "Reader.h"
#include "Detector.h"
#include "Incinerator.h"
#include "Forwarder.h"

void ACTIVITYCOPIER::copyActivity(Activity* source, Activity* target, HandleMap& handleMap) {
	target->activityPace = 10;
	target->activityTickStart = 0;
	target->activityType = 0;
	target->active = false;

	target->movingPace = 20;
	target->movingTickStart = 0;
	target->movementDirection = ACTIVITY::DIR::STATIONARY;
	target->moving = false;

	target->inWorld = false;
	target->origin = source->origin;

	target->parentRef.handle = handleMap[source->parentRef.handle];
	target->selfHandle = handleMap[source->selfHandle];
	target->activityRotation = source->activityRotation;

	target->label = source->label;

	target->baseBlock = source->baseBlock;
}

void ACTIVITYCOPIER::copySingleGrouper(SingleGrouper* source, SingleGrouper* target, HandleMap& handleMap) {
	target->child.handle = handleMap[source->child.handle];

	ACTIVITYCOPIER::copyGrouperBase(source, target, handleMap);
}

void ACTIVITYCOPIER::copyGrouperBase(GrouperBase* source, GrouperBase* target, HandleMap& handleMap) {
	ACTIVITYCOPIER::copyActivity(source, target, handleMap);
}

void ACTIVITYCOPIER::copySingleBlockActivity(SingleBlockActivity* source, SingleBlockActivity* target, HandleMap& handleMap) {
	target->tex = source->tex;
	target->texName = source->texName;

	target->material = source->material;

	ACTIVITYCOPIER::copyActivity(source, target, handleMap);
}

Activity* ACTIVITYCOPIER::copyPiston(Piston* source, HandleMap& handleMap) {
	Piston* target = new Piston();

	target->length = source->length;
	target->headBlock = source->headBlock;
	target->shaftBlock = source->shaftBlock;

	ACTIVITYCOPIER::copySingleGrouper(source, target, handleMap);

	return target;
}

Activity* ACTIVITYCOPIER::copyAnchor(Anchor* source, HandleMap& handleMap) {
	Anchor* target = new Anchor();

	for (auto& child : source->children) {
		target->children.emplace_back(handleMap[child.handle]);
	}

	ACTIVITYCOPIER::copyGrouperBase(source, target, handleMap);

	return target;
}

Activity* ACTIVITYCOPIER::copyRailCrane(RailCrane* source, HandleMap& handleMap) {
	RailCrane* target = new RailCrane();

	target->anchorDirection = source->anchorDirection;

	target->headBlock = source->headBlock;
	target->shaftBlock = source->shaftBlock;

	target->anchorIndexPos = source->anchorIndexPos;

	ACTIVITYCOPIER::copySingleGrouper(source, target, handleMap);

	return target;
}

Activity* ACTIVITYCOPIER::copyLuaActivity(LuaActivity* source, HandleMap& handleMap) {
	LuaActivity* target = new LuaActivity();

	target->interrupt = false;
	target->gameStateRef = source->gameStateRef;

	target->setScript(source->script, *target->gameStateRef);
	target->watchedVars = source->watchedVars;

	ACTIVITYCOPIER::copySingleBlockActivity(source, target, handleMap);

	return target;
}

Activity* ACTIVITYCOPIER::copyGrabber(Grabber* source, HandleMap& handleMap) {
	Grabber* target = new Grabber();

	target->block = source->block;
	target->material = source->material;

	ACTIVITYCOPIER::copyActivity(source, target, handleMap);

	return target;
}

Activity* ACTIVITYCOPIER::copyReader(Reader* source, HandleMap& handleMap) {
	Reader* reader = new Reader();

	ACTIVITYCOPIER::copySingleBlockActivity(source, reader, handleMap);

	return reader;
}

Activity* ACTIVITYCOPIER::copyDetector(Detector* source, HandleMap& handleMap) {
	Detector* detector = new Detector();

	ACTIVITYCOPIER::copySingleBlockActivity(source, detector, handleMap);

	return detector;
}

Activity* ACTIVITYCOPIER::copyIncinerator(Incinerator* source, HandleMap& handleMap) {
	Incinerator* incinerator = new Incinerator();

	ACTIVITYCOPIER::copySingleBlockActivity(source, incinerator, handleMap);

	return incinerator;
}

Activity* ACTIVITYCOPIER::copyForwarder(Forwarder* source, HandleMap& handleMap) {
	Forwarder* forwarder = new Forwarder();

	ACTIVITYCOPIER::copySingleBlockActivity(source, forwarder, handleMap);

	return forwarder;
}

UniqueReference<Activity, Activity> ACTIVITYCOPIER::copy(WeakReference<Activity, Activity> ref) {
	HandleMap handleMap;

	auto const& freeHandles = Locator<ReferenceManager<Activity>>::ref().freeHandles;
	auto it = freeHandles.begin();

	for (auto member : ref.get()->getRootPtr()->getTreeMembers()) {
		assert(it != freeHandles.end());

		handleMap[member->getHandle()] = *it;
		it++;
	}

	for (auto member : ref.get()->getRootPtr()->getTreeMembers()) {
		Activity* target = nullptr;
		switch (member->getType()) {
			case ACTIVITY::TYPE::PISTON:
				target = ACTIVITYCOPIER::copyPiston(static_cast<Piston*>(member), handleMap);
				break;
			case ACTIVITY::TYPE::ANCHOR:
				target = ACTIVITYCOPIER::copyAnchor(static_cast<Anchor*>(member), handleMap);
				break;
			case ACTIVITY::TYPE::RAILCRANE:
				target = ACTIVITYCOPIER::copyRailCrane(static_cast<RailCrane*>(member), handleMap);
				break;
			case ACTIVITY::TYPE::GRABBER:
				target = ACTIVITYCOPIER::copyGrabber(static_cast<Grabber*>(member), handleMap);
				break;
			case ACTIVITY::TYPE::LUA:
				target = ACTIVITYCOPIER::copyLuaActivity(static_cast<LuaActivity*>(member), handleMap);
				break;
			case ACTIVITY::TYPE::READER:
				target = ACTIVITYCOPIER::copyReader(static_cast<Reader*>(member), handleMap);
				break;
			case ACTIVITY::TYPE::DETECTOR:
				target = ACTIVITYCOPIER::copyDetector(static_cast<Detector*>(member), handleMap);
				break;
			case ACTIVITY::TYPE::INCINERATOR:
				target = ACTIVITYCOPIER::copyIncinerator(static_cast<Incinerator*>(member), handleMap);
				break;
			case ACTIVITY::TYPE::FORWARDER:
				target = ACTIVITYCOPIER::copyForwarder(static_cast<Forwarder*>(member), handleMap);
				break;
			default:
				assert(0);
				break;
		}
		Locator<ReferenceManager<Activity>>::ref().storeReference(target->selfHandle, target);
	}
	return UniqueReference<Activity, Activity>(handleMap[ref.get()->getRootHandle()]);
}
