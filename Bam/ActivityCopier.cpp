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

	target->label = source->label;
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

	ACTIVITYCOPIER::copyActivity(source, target, handleMap);
}

Activity* ACTIVITYCOPIER::copyPiston(Piston* source, HandleMap& handleMap) {
	Piston* target = new Piston();

	target->cogTex = source->cogTex;
	target->ropeTex = source->ropeTex;
	target->headTex = source->headTex;
	target->length = source->length;

	target->direction = source->direction;
	target->headDir = source->headDir;

	target->state = source->state;

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

	target->supportTex = source->supportTex;
	target->shaftTex = source->shaftTex;
	target->anchorTex = source->anchorTex;

	target->orientation = source->orientation;
	target->anchorDirection = source->anchorDirection;
	target->length = source->length;
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

	target->textures = source->textures;
	target->textures_a = source->textures_a;

	target->block = source->block;
	target->dir = source->dir;

	ACTIVITYCOPIER::copyActivity(source, target, handleMap);

	return target;
}

Activity* ACTIVITYCOPIER::copyReader(Reader* source, HandleMap& handleMap) {
	Reader* reader = new Reader();

	ACTIVITYCOPIER::copySingleBlockActivity(source, reader, handleMap);

	return reader;
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
			default:
				assert(0);
				break;
		}
		Locator<ReferenceManager<Activity>>::ref().storeReference(target->selfHandle, target);
	}
	return UniqueReference<Activity, Activity>(handleMap[ref.get()->getRootHandle()]);
}
