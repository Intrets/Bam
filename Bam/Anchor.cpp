#include "common.h"
#include "Anchor.h"
#include "GameState.h"
#include "Modifyables.h"
#include <sstream>
#include "ActivityIgnoringGroup.h"
//#include "Saver.h"
//#include "Loader.h"
#include "RenderInfo.h"

void Anchor::addChild(Handle h) {
	if (h == selfHandle) {
		return;
	}
	children.push_back(WeakReference<Activity, Activity>(h));
}

Anchor::Anchor() {
}

Anchor::Anchor(Handle self) : Activity(self, glm::ivec2(0, 0)) {
}

Anchor::Anchor(Handle self, GameState& gameState) : Anchor(self) {
}

Anchor::~Anchor() {
}

void Anchor::forceMoveOrigin(glm::ivec2 d) {
	for (auto& child : children) {
		child.get()->forceMoveOrigin(d);
	}
}

bool Anchor::idle() {
	if (!Activity::idle()) {
		return false;
	}
	for (auto& child : children) {
		if (!child.get()->idle()) {
			return false;
		}
	}
	return true;
}

void Anchor::rotateForced(glm::ivec2 center, MOVEABLE::ROT rotation) {
	for (auto& child : children) {
		child.get()->rotateForced(center, rotation);
	}
}

void Anchor::appendStaticRenderInfo(GameState& gameState, StaticWorldRenderInfo& staticWorldRenderInfo) {
	//TODO: better debug render info handling?
	glm::vec2 center;
	for (auto& child : children) {
		center += child.get()->getOrigin();
		child.get()->appendStaticRenderInfo(gameState, staticWorldRenderInfo);
	}

	center /= children.size();
	Locator<DebugRenderInfo>::getService()->addPoint(center);

	for (auto& child : children) {
		Locator<DebugRenderInfo>::getService()->addLine(center, child.get()->getOrigin());
	}
}

bool Anchor::canMove(GameState& gameState, MOVEABLE::DIR dir, ActivityIgnoringGroup& ignore) {
	if (moving) return false;
	bool blocked = false;
	for (auto& child : children) {
		if (!child.get()->canMove(gameState, dir, ignore)) {
			blocked = true;
			break;
		}
	}
	return !blocked;
}

bool Anchor::fillTraces(GameState& gameState) {
	return true;
}

void Anchor::removeMoveableTraces(GameState& gameState) {
}

void Anchor::leaveMoveableTraces(GameState& gameState) {
}

void Anchor::doMove(GameState& gameState, MOVEABLE::DIR dir, int pace) {
	//gameState.activityPaceHandler.add(this, pace);
	moving = true;
	for (auto& child : children) {
		child.get()->doMove(gameState, dir, pace);
	}
}

void Anchor::stopMovement(GameState& gameState) {
	for (auto& child : children) {
		child.get()->stopMovement(gameState);
	}
	moving = false;
}

bool Anchor::canActivity(GameState& gameState, int useType, Activity* ignore) {
	return false;
}

void Anchor::doActivityInternal(GameState& gameState, int useType, int pace) {
}

void Anchor::removeActivityTraces(GameState& gameState) {
}

void Anchor::leaveActivityTraces(GameState& gameState) {
}

void Anchor::getGroup(ActivityIgnoringGroup& ignore) {
	ignore.add(selfHandle);
	for (auto& child : children) {
		child.get()->getGroup(ignore);
	}
}

void Anchor::save(Saver& saver) {
	Activity::save(saver);
	int s = children.size();
	saver.store<int>(s);
	for (auto& child : children) {
		saver.store<int>(child.handle);
	}
}

bool Anchor::load(Loader& loader) {
	Activity::load(loader);
	int count;
	loader.retrieve<int>(count);
	for (int i = 0; i < count; i++) {
		int handle;
		loader.retrieve<int>(handle);
		children.push_back(WeakReference<Activity, Activity>(handle));
	}
	return true;
}

void Anchor::fillModifyingMap(ModifyerBase& modifyer) {
	Activity::fillModifyingMap(modifyer);
	modifyer.modifyables["children"] = std::make_unique<ModifyableAddChild>(&Anchor::addChild);

}

void Anchor::modifyMember(GameState& gameState, std::string& name, std::vector<std::string>& value) {
	auto& modifyer = Locator<Modifyer<Anchor>>::getService()->modifyables;
	if (modifyer.count(name) != 0) {
		modifyer[name]->modify(this, gameState, value);
	}
}

std::stringstream& Anchor::getMembers(std::stringstream& out) {
	out << "^ Anchor members: ^\n";
	for (auto& member : Locator<Modifyer<Anchor>>::getService()->modifyables) {
		out << member.first << ": ";
		member.second->toStream(this, out) << "\n";
	}
	out << "v Anchor members: v\n";
	return out;
}

ACTIVITY::TYPE Anchor::getType() {
	return ACTIVITY::ANCHOR;
}

void Anchor::removeTracesUp(GameState& gameState) {
	for (auto& child : children) {
		child.get()->removeTracesUp(gameState);
	}
	removeTracesForced(gameState);
}

bool Anchor::removeTracesForced(GameState& gameState) {
	return true;
}

void Anchor::appendSelectionInfo(GameState& gameState, RenderInfo& renderInfo) {
	for (auto& child : children) {
		child.get()->appendSelectionInfo(gameState, renderInfo);
	}
}
