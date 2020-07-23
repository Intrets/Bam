#include "common.h"

#include "Anchor.h"
#include "GameState.h"
#include "Modifyables.h"
#include <sstream>
#include "ActivityIgnoringGroup.h"
#include "Saver.h"
#include "Loader.h"
#include "RenderInfo.h"
#include <algorithm>

void Anchor::addChild(Handle h) {
	if (h == this->selfHandle) {
		return;
	}
	this->children.push_back(WeakReference<Activity, Activity>(h));
}

bool Anchor::removeChild(Handle h) {
	auto newEnd = std::remove_if(
		this->children.begin(), this->children.end(),
		[=](WeakReference<Activity, Activity> t) -> bool {
		return t.handle == h;
	}
	);
	this->children.erase(newEnd, this->children.end());
	return this->children.empty();
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

bool Anchor::canActivityLocal(GameState& gameState, int32_t useType) {
	return false;
}

void Anchor::removeActivityTracesLocal(GameState& gameState) {
}

void Anchor::leaveActivityTracesLocal(GameState& gameState) {
}

void Anchor::save(Saver& saver) {
	Activity::save(saver);
	size_t s = this->children.size();
	saver.store<size_t>(s);
	for (auto& child : this->children) {
		saver.store(child.handle);
	}
}

bool Anchor::load(Loader& loader) {
	Activity::load(loader);
	size_t count;
	loader.retrieve<size_t>(count);
	for (int32_t i = 0; i < count; i++) {
		int32_t handle;
		loader.retrieve(handle);
		this->children.push_back(WeakReference<Activity, Activity>(handle));
	}
	return true;
}

void Anchor::fillModifyingMap(ModifyerBase& modifyer) {
	Activity::fillModifyingMap(modifyer);
	modifyer.modifyables["children"] = std::make_unique<ModifyableAddChild>(&Anchor::addChild);

}

void Anchor::modifyMember(GameState& gameState, std::string& name, std::vector<std::string>& value) {
	auto& modifyer = Locator<Modifyer<Anchor>>::get()->modifyables;
	if (modifyer.count(name) != 0) {
		modifyer[name]->modify(this, gameState, value);
	}
}

std::stringstream& Anchor::getMembers(std::stringstream& out) {
	out << "^ Anchor members: ^\n";
	for (auto& member : Locator<Modifyer<Anchor>>::get()->modifyables) {
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
	for (auto& child : this->children) {
		child.get()->getTreeMembers(members);
	}
}

bool Anchor::canFillTracesLocal(GameState& gameState) {
	return true;
}

void Anchor::fillTracesLocalForced(GameState& gameState) {
}

void Anchor::removeTracesLocalForced(GameState& gameState) {
}

void Anchor::applyActivityLocalForced(GameState& gameState, int32_t type, int32_t pace) {
	Activity::applyActivityLocalForced(gameState, type, pace);
}

void Anchor::appendSelectionInfo(GameState& gameState, RenderInfo& renderInfo, glm::vec4 color) {
	for (auto& child : this->children) {
		child.get()->appendSelectionInfo(gameState, renderInfo, { 0.5,0.5,0.5,0.5 });
	}
}
