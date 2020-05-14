#include "common.h"
#include "Piston.h"

#include "Loader.h"
#include "Saver.h"
#include "GameState.h"
#include "StaticWorldRenderInfo.h"
#include "Modifyables.h"
#include "ActivityIgnoringGroup.h"
#include "BlockIDTextures.h"
#include "RenderInfo.h"

Piston::Piston(Handle self, GameState& gameState, glm::ivec2 pos, MOVEABLE::DIR dir, bool leaveTraces) :
	Grouper(self, pos),
	length(0),
	headDir(dir) {
	state = PISTON::STATIONARY;

	if (leaveTraces) {
		fillTracesLocalForced(gameState);
	}

	auto t = Locator<BlockIDTextures>::getService();
	headTex = t->getBlockTextureID("grabber.dds");
	ropeTex = t->getBlockTextureID("rope.dds");
	cogTex = t->getBlockTextureID("cog.dds");
}

Piston::Piston() {
}

Piston::~Piston() {
}

void Piston::fillModifyingMap(ModifyerBase& modifier) {
	Activity::fillModifyingMap(modifier);
	modifier.modifyables["headdirection"] = std::make_unique<ModifyableDIR<Piston>>(&Piston::headDir);
	modifier.modifyables["child"] = std::make_unique<ModifyableFuncWeakRef<Piston, Anchor>>(&Piston::addChild, &Piston::child);
}

void Piston::modifyMember(GameState& gameState, std::string& name, std::vector<std::string>& value) {
	auto& modifyer = Locator<Modifyer<Piston>>::getService()->modifyables;
	if (modifyer.count(name) != 0) {
		modifyer[name]->modify(this, gameState, value);
	}
}

std::stringstream& Piston::getMembers(std::stringstream& out) {
	out << "^ Piston members: ^\n";
	for (auto& member : Locator<Modifyer<Piston>>::getService()->modifyables) {
		out << member.first << ": ";
		member.second->toStream(this, out) << "\n";
	}
	out << "v Piston members: v\n";
	return out;
}

ACTIVITY::TYPE Piston::getType() {
	return ACTIVITY::PISTON;
}

bool Piston::canActivityLocal(GameState& gameState, int type) {
	glm::ivec2 headDirection = MOVEABLE::DIRECTION[headDir];
	switch (type) {
		case PISTON::EXTEND:
			{
				if (child.isNotNull()) {
					return child.get()->canMoveUp(gameState, headDir);
				}
				else {
					auto next = origin + (length + 2) * headDirection;
					return !gameState.staticWorld.getActivity(next).has_value();
				}
			}
			break;
		case PISTON::RETRACT:
			{
				if (length == 0) {
					return false;
				}
				if (child.isNull()) {
					return true;
				}
				std::vector<Activity*> extraIgnore{ this };
				return child.get()->canMoveUp(gameState, MOVEABLE::REV_DIR[headDir], extraIgnore);
			}
			break;
		default:
			return false;
			break;
	}
}

bool Piston::canMoveLocal(GameState& gameState, MOVEABLE::DIR dir, ActivityIgnoringGroup& ignore) {
	glm::ivec2 headDirection = MOVEABLE::DIRECTION[headDir];
	glm::ivec2 moveDirection = MOVEABLE::DIRECTION[dir];

	int d = idot(moveDirection, headDirection);
	auto ori = origin + moveDirection;
	bool blocked = false;
	if (d == 0) {
		for (int i = 0; i <= length + 1; i++) {
			auto p = ori + i * headDirection;
			if (gameState.staticWorld.isOccupied(p, ignore)) {
				blocked = true;
				break;
			}
		}
	}
	else if (d == 1) {
		auto p = ori + (length + 1) * headDirection;
		if (gameState.staticWorld.isOccupied(p, ignore)) {
			blocked = true;
		}
	}
	else {
		auto p = ori;
		if (gameState.staticWorld.isOccupied(p, ignore)) {
			blocked = true;
		}
	}
	return !blocked;
}

void Piston::appendSelectionInfo(GameState& gameState, RenderInfo& renderInfo, glm::vec4 color) {
	glm::vec2 headDirection = MOVEABLE::DIRECTION[headDir];
	glm::vec2 moveDirection = MOVEABLE::DIRECTION[movementDirection];
	int tick = gameState.tick;
	glm::vec2 ori = glm::vec2(origin);
	if (moving) {
		float scale = static_cast<float>(tick - movingTickStart) / movingPace;
		ori += scale * moveDirection;
	}
	glm::vec2 grabberPos = ori;
	if (active) {
		float scale = static_cast<float>(tick - activityTickStart) / activityPace;
		grabberPos += (scale - 1) * glm::vec2(direction);
	}

	glm::vec2 base = ori;
	glm::vec2 head = grabberPos + headDirection * (1.0f + length) + glm::vec2(1.0f);
	renderInfo.selectionRenderInfo.addBox(base, head, color);
}

void Piston::appendStaticRenderInfo(GameState& gameState, StaticWorldRenderInfo& staticWorldRenderInfo) {
	glm::vec2 headDirection = MOVEABLE::DIRECTION[headDir];
	glm::vec2 moveDirection = MOVEABLE::DIRECTION[movementDirection];
	int tick = gameState.tick;
	glm::vec2 ori = glm::vec2(origin);
	if (moving) {
		float scale = static_cast<float>(tick - movingTickStart) / movingPace;
		ori += scale * moveDirection;
	}
	glm::vec2 grabberPos = ori;
	if (active) {
		float scale = static_cast<float>(tick - activityTickStart) / activityPace;
		grabberPos += (scale - 1) * glm::vec2(direction);
	}
	staticWorldRenderInfo.addBlockWithShadow(ori, cogTex);
	for (int i = 0; i <= length; i++) {
		auto p = ori + static_cast<float>(i) * headDirection;
		staticWorldRenderInfo.addBlockWithoutShadow(p, ropeTex);
	}
	auto p = grabberPos + static_cast<float>(length + 1) * headDirection;
	staticWorldRenderInfo.addBlockWithShadow(p, headTex);
}

void Piston::removeActivityTracesLocal(GameState& gameState) {
}

void Piston::leaveActivityTracesLocal(GameState& gameState) {
}

void Piston::removeMoveableTracesLocal(GameState& gameState) {
	glm::ivec2 headDirection = MOVEABLE::DIRECTION[headDir];
	int d = idot(getDirection(movementDirection), headDirection);
	auto ori = origin - getDirection(movementDirection);
	if (d == 0) {
		for (int i = 0; i <= length + 1; i++) {
			auto p = ori + i * headDirection;
			gameState.staticWorld.removeTraceFilter(p, selfHandle);
		}
	}
	else if (d == -1) {
		auto p = ori + (length + 1) * headDirection;
		gameState.staticWorld.removeTraceFilter(p, selfHandle);
	}
	else {
		auto p = ori;
		gameState.staticWorld.removeTraceFilter(p, selfHandle);
	}
}

void Piston::leaveMoveableTracesLocal(GameState& gameState) {
	glm::ivec2 headDirection = MOVEABLE::DIRECTION[headDir];
	int d = idot(getDirection(movementDirection), headDirection);
	auto ori = origin + getDirection(movementDirection);
	if (d == 0) {
		for (int i = 0; i <= length + 1; i++) {
			auto p = ori + i * headDirection;
			gameState.staticWorld.leaveTrace(p, selfHandle);
		}
	}
	else if (d == 1) {
		auto p = ori + (length + 1) * headDirection;
		gameState.staticWorld.leaveTrace(p, selfHandle);
	}
	else {
		auto p = ori;
		gameState.staticWorld.leaveTrace(p, selfHandle);
	}
}

void Piston::save(Saver& saver) {
	Activity::save(saver);
	saver.store<glm::ivec2>(direction);
	saver.store<MOVEABLE::DIR>(headDir);
	saver.store<PISTON::DIR>(state);
	saver.store<int>(child.handle);
	saver.store<int>(length);
}

bool Piston::load(Loader& loader) {
	Activity::load(loader);
	loader.retrieve<glm::ivec2>(direction);
	loader.retrieve<MOVEABLE::DIR>(headDir);
	loader.retrieve<PISTON::DIR>(state);
	int handle;
	loader.retrieve<int>(handle);
	child.handle = handle;
	loader.retrieve<int>(length);

	auto s = Locator<BlockIDTextures>::getService();
	headTex = s->getBlockTextureID("grabber.dds");
	ropeTex = s->getBlockTextureID("rope.dds");
	cogTex = s->getBlockTextureID("cog.dds");

	return true;
}

void Piston::rotateForcedLocal(glm::ivec2 center, MOVEABLE::ROT rotation) {
	auto d = origin - center;
	switch (rotation) {
		case MOVEABLE::ROT::CLOCKWISE:
			d = glm::ivec2(d.y, -d.x - 1);
			headDir = static_cast<MOVEABLE::DIR>(static_cast<int>(headDir + 1) % 4);
			break;
		case MOVEABLE::ROT::COUNTERCLOCKWISE:
			headDir = static_cast<MOVEABLE::DIR>(static_cast<int>(headDir + 3) % 4);
			d = glm::ivec2(-d.y - 1, d.x);
			break;
		default:
			break;
	}
	origin = center + d;
}

void Piston::fillTracesLocalForced(GameState& gameState) {
	glm::ivec2 pos = origin;
	for (int i = 0; i < length + 2; i++) {
		gameState.staticWorld.leaveTrace(pos, selfHandle);
		pos += MOVEABLE::DIRECTION[headDir];
	}
}

void Piston::removeTracesLocalForced(GameState& gameState) {
	glm::ivec2 pos = origin;
	for (int i = 0; i < length + 2; i++) {
		gameState.staticWorld.removeTraceForced(pos);
		pos += MOVEABLE::DIRECTION[headDir];
	}
}

void Piston::applyActivityLocalForced(GameState& gameState, int type, int pace) {
	Activity::applyActivityLocalForced(gameState, type, pace);
	glm::ivec2 headDirection = MOVEABLE::DIRECTION[headDir];
	switch (type) {
		case PISTON::EXTEND:
			{
				auto next = origin + (length + 2) * headDirection;
				gameState.staticWorld.leaveTrace(next, selfHandle);
				if (child.isNotNull()) {
					child.get()->applyMoveUpForced(gameState, headDir, pace);
				}
				direction = headDirection;
				length++;
			}
			break;
		case PISTON::RETRACT:
			{
				auto headpos = origin + (length + 1) * headDirection;
				gameState.staticWorld.removeTraceFilter(headpos, selfHandle);
				if (child.isNotNull()) {
					child.get()->applyMoveUpForced(gameState, MOVEABLE::REV_DIR[headDir], pace);
				}
				direction = -headDirection;
				length--;
			}
			break;
		default:
			break;
	}
}

bool Piston::canFillTracesLocal(GameState& gameState) {
	glm::ivec2 pos = origin;
	for (int i = 0; i < length + 2; i++) {
		if (gameState.staticWorld.isOccupied(pos)) {
			return false;
		}
		pos += MOVEABLE::DIRECTION[headDir];
	}
	return true;
}

