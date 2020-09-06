#include "common.h"

#include "Grabber.h"
#include "GameState.h"
#include "BlockIDTextures.h"
#include "StaticWorldRenderInfo.h"
#include "RenderInfo.h"
#include "Saver.h"
#include "Loader.h"

glm::ivec2 Grabber::getGrabbedOffset() const {
	switch (this->dir) {
		case ACTIVITY::DIR::UP:
			return { 0,1 };
			break;
		case ACTIVITY::DIR::DOWN:
			return { 0,-1 };
			break;
		case ACTIVITY::DIR::RIGHT:
			return { 1,0 };
			break;
		case ACTIVITY::DIR::LEFT:
			return { -1,0 };
			break;
		default:
			return { 0,0 };
			break;
	}
}

glm::ivec2 Grabber::getGrabbedPos() const {
	return this->origin + this->getGrabbedOffset();
}

Grabber::Grabber() {
	auto t = Locator<BlockIDTextures>::get();
	textures[ACTIVITY::DIR::DOWN] = t->getBlockTextureID("grabber_down.dds");
	textures[ACTIVITY::DIR::UP] = t->getBlockTextureID("grabber_up.dds");
	textures[ACTIVITY::DIR::LEFT] = t->getBlockTextureID("grabber_left.dds");
	textures[ACTIVITY::DIR::RIGHT] = t->getBlockTextureID("grabber_right.dds");

	textures_a[ACTIVITY::DIR::DOWN] = t->getBlockTextureID("grabber_down_a.dds");
	textures_a[ACTIVITY::DIR::UP] = t->getBlockTextureID("grabber_up_a.dds");
	textures_a[ACTIVITY::DIR::LEFT] = t->getBlockTextureID("grabber_left_a.dds");
	textures_a[ACTIVITY::DIR::RIGHT] = t->getBlockTextureID("grabber_right_a.dds");
}

Grabber::Grabber(Handle self, glm::ivec2 pos) : Activity(self, pos) {
	auto t = Locator<BlockIDTextures>::get();
	textures[ACTIVITY::DIR::DOWN] = t->getBlockTextureID("grabber_down.dds");
	textures[ACTIVITY::DIR::UP] = t->getBlockTextureID("grabber_up.dds");
	textures[ACTIVITY::DIR::LEFT] = t->getBlockTextureID("grabber_left.dds");
	textures[ACTIVITY::DIR::RIGHT] = t->getBlockTextureID("grabber_right.dds");

	textures_a[ACTIVITY::DIR::DOWN] = t->getBlockTextureID("grabber_down_a.dds");
	textures_a[ACTIVITY::DIR::UP] = t->getBlockTextureID("grabber_up_a.dds");
	textures_a[ACTIVITY::DIR::LEFT] = t->getBlockTextureID("grabber_left_a.dds");
	textures_a[ACTIVITY::DIR::RIGHT] = t->getBlockTextureID("grabber_right_a.dds");
}

void Grabber::rotateForcedLocal(glm::ivec2 center, ACTIVITY::ROT rotation) {
	this->dir = ACTIVITY::ROTATE(rotation, this->dir);
	auto d = this->getOrigin() - center;
	switch (rotation) {
		case ACTIVITY::ROT::CLOCKWISE:
			d = glm::ivec2(d.y, -d.x - 1);
			break;
		case ACTIVITY::ROT::COUNTERCLOCKWISE:
			d = glm::ivec2(-d.y - 1, d.x);
			break;
		default:
			break;
	}
	this->origin = center + d;
}

bool Grabber::canActivityLocal(GameState& gameState, int32_t type) {
	if (type == GRABBER::STATE::RELEASED) {
		return true;
	}
	else {
		return gameState.staticWorld.getBlockRef(this->getGrabbedPos())->isBlock();
	}
}

void Grabber::applyActivityLocalForced(GameState& gameState, int32_t type, int32_t pace) {
	if (type == GRABBER::STATE::RELEASED) {
		this->activityType = GRABBER::STATE::RELEASED;
		gameState.staticWorld.setBlock(this->block, this->getGrabbedPos());
	}
	else {
		this->activityType = GRABBER::STATE::GRABBED;
		auto b = gameState.staticWorld.getBlockRef(this->getGrabbedPos());
		this->block = *b;
		b->setM(this->getHandle());
		b->setID(0);
	}
}

void Grabber::removeTracesLocalForced(GameState& gameState) {
	switch (this->activityType) {
		case GRABBER::STATE::GRABBED:
			gameState.staticWorld.removeTraceForced(this->getOrigin());
			gameState.staticWorld.removeTraceForced(this->getGrabbedPos());
			break;
		case GRABBER::STATE::RELEASED:
			gameState.staticWorld.removeTraceForced(this->getOrigin());
			break;
		default:
			break;
	}
}

void Grabber::fillTracesLocalForced(GameState& gameState) {
	switch (this->activityType) {
		case GRABBER::STATE::GRABBED:
			gameState.staticWorld.leaveTrace(this->getOrigin(), this->getHandle());
			gameState.staticWorld.leaveTrace(this->getGrabbedPos(), this->getHandle());
			break;
		case GRABBER::STATE::RELEASED:
			gameState.staticWorld.leaveTrace(this->getOrigin(), this->getHandle());
			break;
		default:
			break;
	}
}

void Grabber::removeActivityTracesLocal(GameState& gameState) {
}

void Grabber::leaveActivityTracesLocal(GameState& gameState) {
}

ACTIVITY::TYPE Grabber::getType() {
	return ACTIVITY::TYPE::GRABBER;
}

void Grabber::save(Saver& saver) {
	this->Activity::save(saver);
	this->block.save(saver);
	saver.store(this->dir);
}

bool Grabber::load(Loader& loader) {
	this->Activity::load(loader);
	this->block.load(loader);
	loader.retrieve(this->dir);
	return true;
}

void Grabber::appendStaticRenderInfo(GameState const& gameState, StaticWorldRenderInfo& staticWorldRenderInfo) {
	glm::vec2 ori = this->origin;
	if (this->moving) {
		float scale = static_cast<float>(gameState.tick - this->movingTickStart) / this->movingPace;
		ori += scale * glm::vec2(ACTIVITY::GETDIRECTION(this->movementDirection));
	}
	if (this->activityType == GRABBER::STATE::GRABBED) {
		staticWorldRenderInfo.addBlockWithShadow(this->getMovingOrigin(gameState), this->textures_a[dir]);
		staticWorldRenderInfo.addBlockWithShadow(this->getMovingOrigin(gameState) + glm::vec2(this->getGrabbedOffset()), this->block.getID());
	}
	else {
		staticWorldRenderInfo.addBlockWithShadow(this->getMovingOrigin(gameState), this->textures[dir]);
	}
}

bool Grabber::canMoveLocal(GameState& gameState, ACTIVITY::DIR d, ActivityIgnoringGroup& ignore) {
	auto moveDir = ACTIVITY::GETDIRECTION(d);

	switch (this->activityType) {
		case GRABBER::STATE::GRABBED:
			{
				auto grabDir = ACTIVITY::GETDIRECTION(this->dir);

				auto s = idot(moveDir, grabDir);

				switch (s) {
					case -1:
						return !gameState.staticWorld.isOccupied(this->getOrigin() + moveDir, ignore);
						break;
					case 0:
						return !gameState.staticWorld.isOccupied(this->getOrigin() + moveDir, ignore) && !gameState.staticWorld.isOccupied(this->getGrabbedPos() + moveDir, ignore);
						break;
					case 1:
						return !gameState.staticWorld.isOccupied(this->getGrabbedPos() + moveDir, ignore);
						break;
					default:
						break;
				}
				break;
			}
		case GRABBER::STATE::RELEASED:
			{
				return !gameState.staticWorld.isOccupied(this->getOrigin() + moveDir, ignore);
				break;
			}
		default:
			break;
	}
	return false;
}

bool Grabber::canFillTracesLocal(GameState& gameState) {
	switch (this->activityType) {
		case GRABBER::STATE::GRABBED:
			return !gameState.staticWorld.isOccupied(this->getOrigin()) && !gameState.staticWorld.isOccupied(this->getGrabbedPos());
			break;
		case GRABBER::STATE::RELEASED:
			return !gameState.staticWorld.isOccupied(this->getOrigin());
			break;
		default:
			return false;
			break;
	}
}

void Grabber::removeMoveableTracesLocal(GameState& gameState) {
	switch (this->activityType) {
		case GRABBER::STATE::GRABBED:
			{
				auto moveDir = ACTIVITY::GETDIRECTION(ACTIVITY::FLIP(this->movementDirection));
				auto grabDir = ACTIVITY::GETDIRECTION(this->dir);

				auto d = idot(moveDir, grabDir);

				switch (d) {
					case -1:
						gameState.staticWorld.removeTraceFilter(this->getOrigin() + moveDir, this->getHandle());
						break;
					case 0:
						gameState.staticWorld.removeTraceFilter(this->getOrigin() + moveDir, this->getHandle());
						gameState.staticWorld.removeTraceFilter(this->getGrabbedPos() + moveDir, this->getHandle());
						break;
					case 1:
						gameState.staticWorld.removeTraceFilter(this->getGrabbedPos() + moveDir, this->getHandle());
						break;
					default:
						break;
				}
				break;
			}
		case GRABBER::STATE::RELEASED:
			{
				gameState.staticWorld.removeTraceFilter(this->getOrigin() + ACTIVITY::GETDIRECTION(ACTIVITY::FLIP(this->movementDirection)), this->getHandle());
				break;
			}
		default:
			break;
	}
}

void Grabber::leaveMoveableTracesLocal(GameState& gameState) {
	switch (this->activityType) {
		case GRABBER::STATE::GRABBED:
			{
				auto moveDir = ACTIVITY::GETDIRECTION(this->movementDirection);
				auto grabDir = ACTIVITY::GETDIRECTION(this->dir);

				auto d = idot(moveDir, grabDir);

				switch (d) {
					case -1:
						gameState.staticWorld.leaveTrace(this->getOrigin() + moveDir, this->getHandle());
						break;
					case 0:
						gameState.staticWorld.leaveTrace(this->getOrigin() + moveDir, this->getHandle());
						gameState.staticWorld.leaveTrace(this->getGrabbedPos() + moveDir, this->getHandle());
						break;
					case 1:
						gameState.staticWorld.leaveTrace(this->getGrabbedPos() + moveDir, this->getHandle());
						break;
					default:
						break;
				}
				break;
			}
		case GRABBER::STATE::RELEASED:
			{
				gameState.staticWorld.leaveTrace(this->getOrigin() + ACTIVITY::GETDIRECTION(this->movementDirection), this->getHandle());
				break;
			}
		default:
			break;
	}
}

void Grabber::impl_getTreeMembers(std::vector<Activity*>& members) {
	members.push_back(this);
}

void Grabber::impl_getTreeMembersDepths(std::vector<std::pair<int32_t, Activity*>>& members, int32_t depth) {
	members.push_back({ depth, this });
}

void Grabber::appendSelectionInfo(GameState const& gameState, RenderInfo& renderInfo, glm::vec4 color) {
	switch (this->activityType) {
		case GRABBER::STATE::GRABBED:
			{
				auto p1 = this->getMovingOrigin(gameState);
				auto p2 = p1 + glm::vec2(this->getGrabbedOffset());

				switch (this->dir) {
					case ACTIVITY::DIR::UP:
					case ACTIVITY::DIR::RIGHT:
						renderInfo.selectionRenderInfo.addBox(p1, p2 + 1.0f, color);
						break;
					case ACTIVITY::DIR::LEFT:
					case ACTIVITY::DIR::DOWN:
						renderInfo.selectionRenderInfo.addBox(p1 + 1.0f, p2, color);
						break;
					default:
						break;
				}
				break;
			}
		case GRABBER::STATE::RELEASED:
			{
				auto p = this->getMovingOrigin(gameState);
				renderInfo.selectionRenderInfo.addBox(p, p + 1.0f, color);
				break;
			}
		default:
			break;
	}
}
