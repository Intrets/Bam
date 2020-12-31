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

void ACTIVITYCOPIER::copyActivity(ReferenceManager<Activity>& manager, Activity* source, Activity* target, HandleMap& handleMap) {
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

	manager.addIncomplete(handleMap[source->getHandle()], &target->parentRef);
	target->selfHandle = handleMap[source->selfHandle];
	target->activityRotation = source->activityRotation;

	target->label = source->label;

	target->baseBlock = source->baseBlock;
}

void ACTIVITYCOPIER::copySingleGrouper(ReferenceManager<Activity>& manager, SingleGrouper* source, SingleGrouper* target, HandleMap& handleMap) {
	manager.addIncomplete(handleMap[source->child.getHandle()], &target->child);

	ACTIVITYCOPIER::copyGrouperBase(manager, source, target, handleMap);
}

void ACTIVITYCOPIER::copyGrouperBase(ReferenceManager<Activity>& manager, GrouperBase* source, GrouperBase* target, HandleMap& handleMap) {
	ACTIVITYCOPIER::copyActivity(manager, source, target, handleMap);
}

void ACTIVITYCOPIER::copySingleBlockActivity(ReferenceManager<Activity>& manager, SingleBlockActivity* source, SingleBlockActivity* target, HandleMap& handleMap) {
	ACTIVITYCOPIER::copyActivity(manager, source, target, handleMap);
}

Activity* ACTIVITYCOPIER::copyPiston(ReferenceManager<Activity>& manager, Piston* source, HandleMap& handleMap) {
	Piston* target = new Piston();

	target->length = source->length;
	target->headBlock = source->headBlock;
	target->shaftBlock = source->shaftBlock;

	ACTIVITYCOPIER::copySingleGrouper(manager, source, target, handleMap);

	return target;
}

Activity* ACTIVITYCOPIER::copyAnchor(ReferenceManager<Activity>& manager, Anchor* source, HandleMap& handleMap) {
	Anchor* target = new Anchor();

	target->children.resize(source->children.size());

	for (size_t i = 0; i < source->children.size(); i++) {
		manager.addIncomplete(handleMap[source->children[i].getHandle()], &target->children[i]);
	}

	ACTIVITYCOPIER::copyGrouperBase(manager, source, target, handleMap);

	return target;
}

Activity* ACTIVITYCOPIER::copyRailCrane(ReferenceManager<Activity>& manager, RailCrane* source, HandleMap& handleMap) {
	RailCrane* target = new RailCrane();

	target->anchorDirection = source->anchorDirection;

	target->headBlock = source->headBlock;
	target->shaftBlock = source->shaftBlock;

	target->anchorIndexPos = source->anchorIndexPos;

	ACTIVITYCOPIER::copySingleGrouper(manager, source, target, handleMap);

	return target;
}

Activity* ACTIVITYCOPIER::copyLuaActivity(ReferenceManager<Activity>& manager, LuaActivity* source, HandleMap& handleMap) {
	LuaActivity* target = new LuaActivity();

	target->interrupt = false;
	target->gameStateRef = source->gameStateRef;

	target->setScript(source->script, *target->gameStateRef);
	target->watchedVars = source->watchedVars;

	ACTIVITYCOPIER::copySingleBlockActivity(manager, source, target, handleMap);

	return target;
}

Activity* ACTIVITYCOPIER::copyGrabber(ReferenceManager<Activity>& manager, Grabber* source, HandleMap& handleMap) {
	Grabber* target = new Grabber();

	target->block = source->block;

	ACTIVITYCOPIER::copyActivity(manager, source, target, handleMap);

	return target;
}

Activity* ACTIVITYCOPIER::copyReader(ReferenceManager<Activity>& manager, Reader* source, HandleMap& handleMap) {
	Reader* reader = new Reader();

	ACTIVITYCOPIER::copySingleBlockActivity(manager, source, reader, handleMap);

	return reader;
}

Activity* ACTIVITYCOPIER::copyDetector(ReferenceManager<Activity>& manager, Detector* source, HandleMap& handleMap) {
	Detector* detector = new Detector();

	ACTIVITYCOPIER::copySingleBlockActivity(manager, source, detector, handleMap);

	return detector;
}

Activity* ACTIVITYCOPIER::copyIncinerator(ReferenceManager<Activity>& manager, Incinerator* source, HandleMap& handleMap) {
	Incinerator* incinerator = new Incinerator();

	ACTIVITYCOPIER::copySingleBlockActivity(manager, source, incinerator, handleMap);

	return incinerator;
}

Activity* ACTIVITYCOPIER::copyForwarder(ReferenceManager<Activity>& manager, Forwarder* source, HandleMap& handleMap) {
	Forwarder* forwarder = new Forwarder();

	ACTIVITYCOPIER::copySingleBlockActivity(manager, source, forwarder, handleMap);

	return forwarder;
}

UniqueReference<Activity, Activity> ACTIVITYCOPIER::copy(ReferenceManager<Activity>& manager, WeakReference<Activity, Activity> ref) {
	HandleMap handleMap;

	auto it = manager.freeHandles.begin();

	for (auto member : ref.get()->getRootPtr()->getTreeMembers()) {
		assert(it != manager.freeHandles.end());

		handleMap[member->getHandle()] = *it;
		it++;
	}

	for (auto member : ref.get()->getRootPtr()->getTreeMembers()) {
		Activity* target = nullptr;
		switch (member->getType()) {
			case ACTIVITY::TYPE::PISTON:
				target = ACTIVITYCOPIER::copyPiston(manager, static_cast<Piston*>(member), handleMap);
				break;
			case ACTIVITY::TYPE::ANCHOR:
				target = ACTIVITYCOPIER::copyAnchor(manager, static_cast<Anchor*>(member), handleMap);
				break;
			case ACTIVITY::TYPE::RAILCRANE:
				target = ACTIVITYCOPIER::copyRailCrane(manager, static_cast<RailCrane*>(member), handleMap);
				break;
			case ACTIVITY::TYPE::GRABBER:
				target = ACTIVITYCOPIER::copyGrabber(manager, static_cast<Grabber*>(member), handleMap);
				break;
			case ACTIVITY::TYPE::LUA:
				target = ACTIVITYCOPIER::copyLuaActivity(manager, static_cast<LuaActivity*>(member), handleMap);
				break;
			case ACTIVITY::TYPE::READER:
				target = ACTIVITYCOPIER::copyReader(manager, static_cast<Reader*>(member), handleMap);
				break;
			case ACTIVITY::TYPE::DETECTOR:
				target = ACTIVITYCOPIER::copyDetector(manager, static_cast<Detector*>(member), handleMap);
				break;
			case ACTIVITY::TYPE::INCINERATOR:
				target = ACTIVITYCOPIER::copyIncinerator(manager, static_cast<Incinerator*>(member), handleMap);
				break;
			case ACTIVITY::TYPE::FORWARDER:
				target = ACTIVITYCOPIER::copyForwarder(manager, static_cast<Forwarder*>(member), handleMap);
				break;
			default:
				assert(0);
				break;
		}
		manager.storeReference(target->selfHandle, target);
	}
	return UniqueReference<Activity, Activity>(manager, handleMap[ref.get()->getRootHandle()]);
}
