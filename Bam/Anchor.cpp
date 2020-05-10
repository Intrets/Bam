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

void Anchor::removeChild(Handle h) {
	for (auto it = children.begin(); it != children.end(); it++) {
		if (it->handle == h) {
			children.erase(it);
			return;
		}
	}
}

Anchor::Anchor() {
}

Anchor::Anchor(Handle self) : Activity(self, glm::ivec2(0, 0)) {
}

Anchor::Anchor(Handle self, GameState& gameState) : Anchor(self) {
}

Anchor::~Anchor() {
}

void Anchor::rotateForcedLocal(glm::ivec2 center, MOVEABLE::ROT rotation) {
}

void Anchor::appendStaticRenderInfo(GameState& gameState, StaticWorldRenderInfo& staticWorldRenderInfo) {
}

bool Anchor::canMoveLocal(GameState& gameState, MOVEABLE::DIR dir, ActivityIgnoringGroup& ignore) {
	return true;
}

void Anchor::removeMoveableTracesLocal(GameState& gameState) {
}

void Anchor::leaveMoveableTracesLocal(GameState& gameState) {
}

bool Anchor::canActivityLocal(GameState& gameState, int useType) {
	return false;
}

void Anchor::removeActivityTracesLocal(GameState& gameState) {
}

void Anchor::leaveActivityTracesLocal(GameState& gameState) {
}

void Anchor::save(Saver& saver) {
	Activity::save(saver);
	size_t s = children.size();
	saver.store<size_t>(s);
	for (auto& child : children) {
		saver.store<int>(child.handle);
	}
}

bool Anchor::load(Loader& loader) {
	Activity::load(loader);
	size_t count;
	loader.retrieve<size_t>(count);
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

void Anchor::getTreeMembers(std::vector<Activity*>& members) {
	members.push_back(this);
	for (auto& child : children) {
		members.push_back(child.get());
	}
}

bool Anchor::canFillTracesLocal(GameState& gameState) {
	return true;
}

void Anchor::fillTracesLocalForced(GameState& gameState) {
}

void Anchor::removeTracesLocalForced(GameState& gameState) {
}

void Anchor::applyActivityLocalForced(GameState& gameState, int type, int pace) {
	Activity::applyActivityLocalForced(gameState, type, pace);
}

void Anchor::appendSelectionInfo(GameState& gameState, RenderInfo& renderInfo) {
	for (auto& child : children) {
		child.get()->appendSelectionInfo(gameState, renderInfo);
	}
}
