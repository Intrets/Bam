#pragma once

#include "Activity.h"
#include "Block.h"

class Platform : public Activity
{
public:
	glm::ivec2 size;

	std::vector<std::vector<Block>> blocks;
	std::array<std::vector<glm::ivec2>, 4> blockedDirections;

	void calculateBlockedDirections();

	Platform(Handle self, GameState& gameState, glm::ivec2 size, glm::ivec2 pos, bool leaveTraces = true);
	Platform() = default;
	~Platform() = default;

	virtual void fillTraces(GameState& gameState) override;
	virtual bool removeTraces(GameState& gameState) override;

	// Inherited via Moveable
	virtual void appendSelectionInfo(GameState& gameState, RenderInfo& renderInfo) override;
	virtual void appendStaticRenderInfo(GameState & gameState, StaticWorldRenderInfo & staticWorldRenderInfo) override;
	virtual bool canMove(GameState & gameState, MOVEABLE::DIR dir, ActivityIgnoringGroup& ignore) override;
	virtual void removeMoveableTraces(GameState & gameState) override;
	virtual void leaveMoveableTraces(GameState & gameState) override;

	// Inherited via Activity
	virtual bool canActivity(GameState & gameState, int type, Activity * ignore) override;
	virtual void doActivityInternal(GameState & gameState, int type, int pace) override;

	// Inherited via Activity
	virtual void removeActivityTraces(GameState & gameState) override;
	virtual void leaveActivityTraces(GameState & gameState) override;

	// Inherited via Activity
	virtual void save(Saver & saver) override;
	virtual bool load(Loader& loader) override;

	virtual void fillModifyingMap(ModifyerBase& modifyer) override;
	virtual void modifyMember(GameState& gameState, std::string& name, std::vector<std::string>& value);

	virtual std::stringstream& getMembers(std::stringstream& out);

	virtual ACTIVITY::TYPE getType() override { return ACTIVITY::PLATFORM; }
};

