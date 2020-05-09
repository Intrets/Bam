#pragma once

#include "Activity.h"
#include "Block.h"

class Platform : public Activity
{
private:
	glm::ivec2 size;

	std::vector<std::vector<Block>> blocks;
	std::array<std::vector<glm::ivec2>, 4> blockedDirections;

	void calculateBlockedDirections();

public:
	Platform(Handle self, GameState& gameState, glm::ivec2 size, glm::ivec2 pos, bool leaveTraces = true);
	Platform() = default;
	~Platform() = default;

	// Placement
	virtual void rotateForcedLocal(glm::ivec2 center, MOVEABLE::ROT rotation) override;

	// Activity
	virtual bool canActivityLocal(GameState & gameState, int type) override;
	virtual void applyActivityLocalForced(GameState& gameState, int type, int pace) override;

	// Moveable
	virtual bool canMoveLocal(GameState & gameState, MOVEABLE::DIR dir, ActivityIgnoringGroup& ignore) override;

	// Traces Placement
	virtual bool canFillTracesLocal(GameState& gameState) override;
	virtual void fillTracesLocalForced(GameState& gameState) override;
	virtual void removeTracesLocalForced(GameState& gameState) override;

	// Traces Activity
	virtual void removeActivityTracesLocal(GameState & gameState) override;
	virtual void leaveActivityTracesLocal(GameState & gameState) override;

	// Traces Moveable
	virtual void removeMoveableTracesLocal(GameState & gameState) override;
	virtual void leaveMoveableTracesLocal(GameState & gameState) override;

	// Tree Information
	virtual void getTreeMembers(std::vector<Activity*>& members) override;

	// Serial
	virtual void save(Saver & saver) override;
	virtual bool load(Loader& loader) override;

	// Render
	virtual void appendSelectionInfo(GameState& gameState, RenderInfo& renderInfo) override;
	virtual void appendStaticRenderInfo(GameState & gameState, StaticWorldRenderInfo & staticWorldRenderInfo) override;

	// Modifying
	virtual void fillModifyingMap(ModifyerBase& modifyer) override;
	virtual void modifyMember(GameState& gameState, std::string& name, std::vector<std::string>& value);
	virtual std::stringstream& getMembers(std::stringstream& out);
	virtual ACTIVITY::TYPE getType() override { return ACTIVITY::PLATFORM; }
};

