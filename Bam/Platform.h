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

	virtual void rotateForced(glm::ivec2 center, MOVEABLE::ROT rotation) override;

	// Inherited via Moveable
	virtual void appendSelectionInfo(GameState& gameState, RenderInfo& renderInfo) override;
	virtual void appendStaticRenderInfo(GameState & gameState, StaticWorldRenderInfo & staticWorldRenderInfo) override;
	virtual bool canMoveLocal(GameState & gameState, MOVEABLE::DIR dir, ActivityIgnoringGroup& ignore) override;
	virtual void removeMoveableTracesLocal(GameState & gameState) override;
	virtual void leaveMoveableTracesLocal(GameState & gameState) override;

	// Inherited via Activity
	virtual bool canActivityLocal(GameState & gameState, int type) override;
	//virtual void doActivityInternal(GameState & gameState, int type, int pace) override;

	// Inherited via Activity
	virtual void removeActivityTracesLocal(GameState & gameState) override;
	virtual void leaveActivityTracesLocal(GameState & gameState) override;

	// Inherited via Activity
	virtual void save(Saver & saver) override;
	virtual bool load(Loader& loader) override;

	virtual void fillModifyingMap(ModifyerBase& modifyer) override;
	virtual void modifyMember(GameState& gameState, std::string& name, std::vector<std::string>& value);

	virtual std::stringstream& getMembers(std::stringstream& out);

	virtual ACTIVITY::TYPE getType() override { return ACTIVITY::PLATFORM; }

	// Inherited via Activity
	virtual void getTreeMembers(std::vector<Activity*>& members) override;
	virtual bool canFillTracesLocal(GameState& gameState) override;
	virtual void fillTracesLocalForced(GameState& gameState) override;
	virtual void removeTracesLocalForced(GameState& gameState) override;

	// Inherited via Activity
	virtual void applyActivityLocalForced(GameState& gameState, int type, int pace) override;
};

