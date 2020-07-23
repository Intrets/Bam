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
		this->fillTracesLocalForced(gameState);
	}

	auto t = Locator<BlockIDTextures>::get();
	this->headTex = t->getBlockTextureID("grabber.dds");
	this->ropeTex = t->getBlockTextureID("rope.dds");
	this->cogTex = t->getBlockTextureID("cog.dds");
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
	auto& modifyer = Locator<Modifyer<Piston>>::get()->modifyables;
	if (modifyer.count(name) != 0) {
		modifyer[name]->modify(this, gameState, value);
	}
}

std::stringstream& Piston::getMembers(std::stringstream& out) {
	out << "^ Piston members: ^\n";
	for (auto& member : Locator<Modifyer<Piston>>::get()->modifyables) {
		out << member.first << ": ";
		member.second->toStream(this, out) << "\n";
	}
	out << "v Piston members: v\n";
	return out;
}

ACTIVITY::TYPE Piston::getType() {
	return ACTIVITY::PISTON;
}

bool Piston::canActivityLocal(GameState& gameState, int32_t type) {
	glm::ivec2 headDirection = MOVEABLE::DIRECTION[this->headDir];
	switch (type) {
		case PISTON::EXTEND:
			{
				if (this->child.isNotNull()) {
					return this->child.get()->canMoveUp(gameState, this->headDir);
				}
				else {
					auto next = this->origin + (this->length + 2) * headDirection;
					return !gameState.staticWorld.getActivity(next).has_value();
				}
			}
			break;
		case PISTON::RETRACT:
			{
				if (this->length == 0) {
					return false;
				}
				if (this->child.isNull()) {
					return true;
				}
				std::vector<Activity*> extraIgnore{ this };
				return this->child.get()->canMoveUp(gameState, MOVEABLE::REV_DIR[this->headDir], extraIgnore);
			}
			break;
		default:
			return false;
			break;
	}
}

bool Piston::canMoveLocal(GameState& gameState, MOVEABLE::DIR dir, ActivityIgnoringGroup& ignore) {
	glm::ivec2 headDirection = MOVEABLE::DIRECTION[this->headDir];
	glm::ivec2 moveDirection = MOVEABLE::DIRECTION[dir];

	int32_t d = idot(moveDirection, headDirection);
	auto ori = this->origin + moveDirection;
	bool blocked = false;
	if (d == 0) {
		for (int32_t i = 0; i <= this->length + 1; i++) {
			auto p = ori + i * headDirection;
			if (gameState.staticWorld.isOccupied(p, ignore)) {
				blocked = true;
				break;
			}
		}
	}
	else if (d == 1) {
		auto p = ori + (this->length + 1) * headDirection;
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
	glm::vec2 headDirection = MOVEABLE::DIRECTION[this->headDir];
	glm::vec2 moveDirection = MOVEABLE::DIRECTION[this->movementDirection];
	int32_t tick = gameState.tick;
	glm::vec2 ori = glm::vec2(this->origin);
	if (this->moving) {
		float scale = static_cast<float>(tick - this->movingTickStart) / this->movingPace;
		ori += scale * moveDirection;
	}
	glm::vec2 grabberPos = ori;
	if (this->active) {
		float scale = static_cast<float>(tick - this->activityTickStart) / this->activityPace;
		grabberPos += (scale - 1) * glm::vec2(this->direction);
	}

	glm::vec2 base = ori;
	glm::vec2 head = grabberPos + headDirection * (1.0f + this->length) + glm::vec2(1.0f);
	renderInfo.selectionRenderInfo.addBox(base, head, color);
}

void Piston::appendStaticRenderInfo(GameState& gameState, StaticWorldRenderInfo& staticWorldRenderInfo) {
	glm::vec2 headDirection = MOVEABLE::DIRECTION[this->headDir];
	glm::vec2 moveDirection = MOVEABLE::DIRECTION[this->movementDirection];
	int32_t tick = gameState.tick;
	glm::vec2 ori = glm::vec2(this->origin);
	if (this->moving) {
		float scale = static_cast<float>(tick - this->movingTickStart) / this->movingPace;
		ori += scale * moveDirection;
	}
	glm::vec2 grabberPos = ori;
	if (this->active) {
		float scale = static_cast<float>(tick - this->activityTickStart) / this->activityPace;
		grabberPos += (scale - 1) * glm::vec2(this->direction);
	}
	staticWorldRenderInfo.addBlockWithShadow(ori, this->cogTex);
	for (int32_t i = 0; i <= this->length; i++) {
		auto p = ori + static_cast<float>(i) * headDirection;
		staticWorldRenderInfo.addBlockWithoutShadow(p, this->ropeTex);
	}
	auto p = grabberPos + static_cast<float>(this->length + 1) * headDirection;
	staticWorldRenderInfo.addBlockWithShadow(p, this->headTex);
}

void Piston::removeActivityTracesLocal(GameState& gameState) {
}

void Piston::leaveActivityTracesLocal(GameState& gameState) {
}

void Piston::removeMoveableTracesLocal(GameState& gameState) {
	glm::ivec2 headDirection = MOVEABLE::DIRECTION[this->headDir];
	int32_t d = idot(getDirection(this->movementDirection), headDirection);
	auto ori = this->origin - getDirection(this->movementDirection);
	if (d == 0) {
		for (int32_t i = 0; i <= this->length + 1; i++) {
			auto p = ori + i * headDirection;
			gameState.staticWorld.removeTraceFilter(p, this->selfHandle);
		}
	}
	else if (d == -1) {
		auto p = ori + (this->length + 1) * headDirection;
		gameState.staticWorld.removeTraceFilter(p, this->selfHandle);
	}
	else {
		auto p = ori;
		gameState.staticWorld.removeTraceFilter(p, this->selfHandle);
	}
}

void Piston::leaveMoveableTracesLocal(GameState& gameState) {
	glm::ivec2 headDirection = MOVEABLE::DIRECTION[this->headDir];
	int32_t d = idot(getDirection(this->movementDirection), headDirection);
	auto ori = this->origin + getDirection(this->movementDirection);
	if (d == 0) {
		for (int32_t i = 0; i <= this->length + 1; i++) {
			auto p = ori + i * headDirection;
			gameState.staticWorld.leaveTrace(p, this->selfHandle);
		}
	}
	else if (d == 1) {
		auto p = ori + (this->length + 1) * headDirection;
		gameState.staticWorld.leaveTrace(p, this->selfHandle);
	}
	else {
		auto p = ori;
		gameState.staticWorld.leaveTrace(p, this->selfHandle);
	}
}

void Piston::save(Saver& saver) {
	Grouper::save(saver);
	saver.store<glm::ivec2>(this->direction);
	saver.store<MOVEABLE::DIR>(this->headDir);
	saver.store<PISTON::DIR>(this->state);
	saver.store(this->length);
}

bool Piston::load(Loader& loader) {
	Grouper::load(loader);
	loader.retrieve<glm::ivec2>(this->direction);
	loader.retrieve<MOVEABLE::DIR>(this->headDir);
	loader.retrieve<PISTON::DIR>(this->state);
	loader.retrieve(this->length);

	auto s = Locator<BlockIDTextures>::get();
	this->headTex = s->getBlockTextureID("grabber.dds");
	this->ropeTex = s->getBlockTextureID("rope.dds");
	this->cogTex = s->getBlockTextureID("cog.dds");

	return true;
}

void Piston::rotateForcedLocal(glm::ivec2 center, MOVEABLE::ROT rotation) {
	auto d = this->origin - center;
	switch (rotation) {
		case MOVEABLE::ROT::CLOCKWISE:
			d = glm::ivec2(d.y, -d.x - 1);
			this->headDir = static_cast<MOVEABLE::DIR>(static_cast<int32_t>(this->headDir + 1) % 4);
			break;
		case MOVEABLE::ROT::COUNTERCLOCKWISE:
			this->headDir = static_cast<MOVEABLE::DIR>(static_cast<int32_t>(this->headDir + 3) % 4);
			d = glm::ivec2(-d.y - 1, d.x);
			break;
		default:
			break;
	}
	this->origin = center + d;
}

void Piston::fillTracesLocalForced(GameState& gameState) {
	glm::ivec2 pos = this->origin;
	for (int32_t i = 0; i < this->length + 2; i++) {
		gameState.staticWorld.leaveTrace(pos, this->selfHandle);
		pos += MOVEABLE::DIRECTION[this->headDir];
	}
}

void Piston::removeTracesLocalForced(GameState& gameState) {
	glm::ivec2 pos = this->origin;
	for (int32_t i = 0; i < this->length + 2; i++) {
		gameState.staticWorld.removeTraceForced(pos);
		pos += MOVEABLE::DIRECTION[this->headDir];
	}
}

void Piston::applyActivityLocalForced(GameState& gameState, int32_t type, int32_t pace) {
	Activity::applyActivityLocalForced(gameState, type, pace);
	glm::ivec2 headDirection = MOVEABLE::DIRECTION[this->headDir];
	switch (type) {
		case PISTON::EXTEND:
			{
				auto next = this->origin + (this->length + 2) * headDirection;
				gameState.staticWorld.leaveTrace(next, this->selfHandle);
				if (this->child.isNotNull()) {
					this->child.get()->applyMoveUpForced(gameState, this->headDir, pace);
				}
				this->direction = headDirection;
				this->length++;
			}
			break;
		case PISTON::RETRACT:
			{
				auto headpos = this->origin + (this->length + 1) * headDirection;
				gameState.staticWorld.removeTraceFilter(headpos, this->selfHandle);
				if (this->child.isNotNull()) {
					this->child.get()->applyMoveUpForced(gameState, MOVEABLE::REV_DIR[this->headDir], pace);
				}
				this->direction = -headDirection;
				this->length--;
			}
			break;
		default:
			break;
	}
}

bool Piston::canFillTracesLocal(GameState& gameState) {
	glm::ivec2 pos = this->origin;
	for (int32_t i = 0; i < this->length + 2; i++) {
		if (gameState.staticWorld.isOccupied(pos)) {
			return false;
		}
		pos += MOVEABLE::DIRECTION[this->headDir];
	}
	return true;
}

