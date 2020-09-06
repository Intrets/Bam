#include "common.h"

#include "Anchor.h"
#include "GameState.h"
#include <sstream>
#include "ActivityIgnoringGroup.h"
#include "Saver.h"
#include "Loader.h"
#include "RenderInfo.h"
#include <algorithm>

std::vector<WeakReference<Activity, Activity>>& Anchor::getChildren() {
	return children;
}

bool Anchor::addChild(WeakReference<Activity, Activity> ref) {
	// TODO: check if not creating loops?
	if (ref.handle == this->selfHandle) {
		return false;
	}
	this->children.push_back(ref);
	ref.get()->parentRef.handle = this->selfHandle;
	return true;
}

bool Anchor::removeChild(WeakReference<Activity, Activity> ref) {
	auto newEnd = std::remove_if(
		this->children.begin(), this->children.end(),
		[h = ref.handle](WeakReference<Activity, Activity> t) -> bool
	{
		return t.handle == h;
	});
	this->children.erase(newEnd, this->children.end());
	return this->children.empty();
}

bool Anchor::hasChild() const {
	return !this->children.empty();
}

Anchor::Anchor() {
}

Anchor::Anchor(Handle self) : GrouperBase(self, glm::ivec2(0, 0)) {
}

Anchor::~Anchor() {
}

void Anchor::rotateForcedLocal(glm::ivec2 center, ACTIVITY::ROT rotation) {
}

void Anchor::appendStaticRenderInfo(GameState const& gameState, StaticWorldRenderInfo& staticWorldRenderInfo) {
}

bool Anchor::canMoveLocal(GameState& gameState, ACTIVITY::DIR dir, ActivityIgnoringGroup& ignore) {
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

ACTIVITY::TYPE Anchor::getType() {
	return ACTIVITY::TYPE::ANCHOR;
}

void Anchor::impl_getTreeMembers(std::vector<Activity*>& members) {
	members.push_back(this);
	for (auto& child : this->children) {
		child.get()->impl_getTreeMembers(members);
	}
}

bool Anchor::canFillTracesLocal(GameState& gameState) {
	return true;
}

void Anchor::applyActivityLocalForced(GameState& gameState, int32_t type, int32_t pace) {
	this->Activity::applyActivityLocalForced(gameState, type, pace);
}

void Anchor::appendSelectionInfo(GameState const& gameState, RenderInfo& renderInfo, glm::vec4 color) {
	for (auto& child : this->children) {
		child.get()->appendSelectionInfo(gameState, renderInfo, color);
	}
}

void Anchor::getTreeMembersDepths(std::vector<std::pair<int32_t, Activity*>>& members, int32_t depth) {
	members.push_back({ depth, this });
	for (auto& child : this->children) {
		child.get()->getTreeMembersDepths(members, depth + 1);
	}
}
