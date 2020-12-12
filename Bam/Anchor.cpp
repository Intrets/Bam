#include "common.h"

#include "Anchor.h"
#include "GameState.h"
#include <sstream>
#include "ActivityIgnoringGroup.h"
#include "Saver.h"
#include "Loader.h"
#include "RenderInfo.h"
#include <algorithm>

std::vector<UniqueReference<Activity, Activity>>& Anchor::getChildren() {
	return children;
}

bool Anchor::addChild(UniqueReference<Activity, Activity> ref) {
	// TODO: check if not creating loops?
	if (ref == this) {
		return false;
	}
	ref.get()->parentRef = this;
	this->children.push_back(std::move(ref));
	return true;
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

void Anchor::preMoveableStopLocal(GameState& gameState) {
}

void Anchor::postMoveableStartLocal(GameState& gameState) {
}

bool Anchor::canActivityLocal(GameState& gameState, int32_t useType) {
	return false;
}

void Anchor::save(Saver& saver) {
	Activity::save(saver);
	size_t s = this->children.size();
	saver.store<size_t>(s);
	for (auto& child : this->children) {
		saver.store(child);
	}
}

bool Anchor::load(Loader& loader) {
	Activity::load(loader);
	size_t count;
	loader.retrieve<size_t>(count);
	for (int32_t i = 0; i < count; i++) {
		int32_t handle;
		loader.retrieve(handle);
		this->children.emplace_back(handle);
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

UniqueReference<Activity, Activity> Anchor::popChild() {
	auto res = std::move(this->children.back());
	this->children.resize(this->children.size() - 1);
	return std::move(res);
}

std::optional<UniqueReference<Activity, Activity>> Anchor::removeChild(WeakReference<Activity, Activity> ref) {
	auto newEnd = std::find_if(this->children.begin(), this->children.end(), [ref](UniqueReference<Activity, Activity> const& t) -> bool
	{
		return t == ref;
	});

	if (newEnd == this->children.end()) {
		return std::nullopt;
	}
	else {
		auto res = std::move(*newEnd);
		this->children.erase(newEnd);
		return std::move(res);
	}
}

void Anchor::impl_getTreeMembersDepths(std::vector<std::pair<int32_t, Activity*>>& members, int32_t depth) {
	members.push_back({ depth, this });
	for (auto& child : this->children) {
		child.get()->impl_getTreeMembersDepths(members, depth + 1);
	}
}
