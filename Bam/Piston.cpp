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
		fillTraces(gameState);
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

void Piston::doMove(GameState& gameState, MOVEABLE::DIR dir, int pace) {
	Activity::doMove(gameState, dir, pace);
	if (child) {
		child.get()->doMove(gameState, dir, pace);
	}
}

void Piston::stopMovement(GameState& gameState) {
	Activity::stopMovement(gameState);
	if (child) {
		child.get()->stopMovement(gameState);
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

void Piston::doActivityInternal(GameState& gameState, int useType, int pace) {
	activityPace = pace;
	active = true;
	activityTickStart = gameState.tick;
	glm::ivec2 headDirection = MOVEABLE::DIRECTION[headDir];
	switch (useType) {
		case PISTON::EXTEND:
			{
				auto next = origin + (length + 2) * headDirection;
				gameState.staticWorld.leaveTrace(next, selfHandle);
				if (child) {
					child.get()->applyCurrentMove(gameState, headDir, pace);
				}
				direction = headDirection;
				length++;
			}
			break;
		case PISTON::RETRACT:
			{
				auto headpos = origin + (length + 1) * headDirection;
				gameState.staticWorld.removeTrace(headpos, selfHandle);
				if (child) {
					child.get()->applyCurrentMove(gameState, MOVEABLE::REV_DIR[headDir], pace);
				}
				direction = -headDirection;
				length--;
			}
			break;
		default:
			break;
	}
}

ACTIVITY::TYPE Piston::getType() {
	return ACTIVITY::PISTON;
}

bool Piston::canActivity(GameState& gameState, int type, Activity* ignore_) {
	glm::ivec2 headDirection = MOVEABLE::DIRECTION[headDir];
	if (active) return false;
	switch (type) {
		case PISTON::EXTEND:
			{
				if (child) {
					ActivityIgnoringGroup ignore;
					getGroup(ignore);
					child.get()->getGroup(ignore);
					ignore.prepare();
					return child.get()->canMove(gameState, headDir, ignore);
				}
				else {
					auto next = origin + (length + 2) * headDirection;
					auto r = gameState.staticWorld.getBlock(next);
					return r.first == 0;
				}
			}
			break;
		case PISTON::RETRACT:
			{
				if (length == 0) {
					return false;
				}
				if (!child) {
					return true;
				}
				ActivityIgnoringGroup ignore;
				getGroup(ignore);
				child.get()->getGroup(ignore);
				ignore.prepare();
				return child.get()->canMove(gameState, MOVEABLE::REV_DIR[headDir], ignore);
			}
			break;
		default:
			return false;
			break;
	}
}

bool Piston::canMove(GameState& gameState, MOVEABLE::DIR dir, ActivityIgnoringGroup& ignore) {
	glm::ivec2 headDirection = MOVEABLE::DIRECTION[headDir];
	glm::ivec2 moveDirection = MOVEABLE::DIRECTION[dir];
	if (moving) return false;
	if (child && !child.get()->canMove(gameState, dir, ignore)) {
		return false;
	}
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

void Piston::appendSelectionInfo(GameState& gameState, RenderInfo& renderInfo) {
	if (child) {
		child.get()->appendSelectionInfo(gameState, renderInfo);
	}
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
	renderInfo.selectionRenderInfo.addBox(base, head);
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
	staticWorldRenderInfo.offsets.push_back(ori);
	staticWorldRenderInfo.offsetsShadow.push_back(ori);
	staticWorldRenderInfo.textureIDs.push_back(cogTex);
	for (int i = 0; i <= length; i++) {
		auto p = ori + static_cast<float>(i) * headDirection;
		staticWorldRenderInfo.offsets.push_back(p);
		staticWorldRenderInfo.textureIDs.push_back(ropeTex);
	}
	auto p = grabberPos + static_cast<float>(length + 1) * headDirection;
	staticWorldRenderInfo.offsets.push_back(p);
	staticWorldRenderInfo.offsetsShadow.push_back(p);
	staticWorldRenderInfo.textureIDs.push_back(headTex);
}

bool Piston::fillTraces(GameState& gameState) {
	glm::ivec2 pos = origin;
	for (int i = 0; i < length + 2; i++) {
		if (gameState.staticWorld.isOccupied(pos)) {
			return false;
		}
		pos += MOVEABLE::DIRECTION[headDir];
	}
	pos = origin;
	for (int i = 0; i < length + 2; i++) {
		gameState.staticWorld.leaveTrace(pos, selfHandle);
		pos += MOVEABLE::DIRECTION[headDir];
	}
	return true;
}

bool Piston::removeTraces(GameState& gameState) {
	if (moving || active) {
		return false;
	}
	glm::ivec2 pos = origin;
	for (int i = 0; i < length + 2; i++) {
		gameState.staticWorld.removeTrace(pos);
		pos += MOVEABLE::DIRECTION[headDir];
	}
	return true;
}

void Piston::removeActivityTraces(GameState& gameState) {
}

void Piston::leaveActivityTraces(GameState& gameState) {
}

void Piston::removeMoveableTraces(GameState& gameState) {
	glm::ivec2 headDirection = MOVEABLE::DIRECTION[headDir];
	int d = idot(getDirection(movementDirection), headDirection);
	auto ori = origin - getDirection(movementDirection);
	if (d == 0) {
		for (int i = 0; i <= length + 1; i++) {
			auto p = ori + i * headDirection;
			gameState.staticWorld.removeTrace(p, selfHandle);
		}
	}
	else if (d == -1) {
		auto p = ori + (length + 1) * headDirection;
		gameState.staticWorld.removeTrace(p, selfHandle);
	}
	else {
		auto p = ori;
		gameState.staticWorld.removeTrace(p, selfHandle);
	}
}

void Piston::leaveMoveableTraces(GameState& gameState) {
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

void Piston::getGroup(ActivityIgnoringGroup& ignore) {
	ignore.add(selfHandle);
	if (child) {
		child.get()->getGroup(ignore);
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

bool Piston::addChild(WeakReference<Activity, Activity> ref) {
	if (child) {
		return false;
	}
	else {
		child = ref;
	}
	return true;
}
