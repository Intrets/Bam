#pragma once

#include "Activity.h"

class GameState;

class SingleBlockActivity : public Activity
{
public:
	int tex;
	std::string texName;

	SingleBlockActivity() = default;
	SingleBlockActivity(Handle self, GameState& gameState, glm::ivec2 pos, std::string name, bool leaveTrace = true);

	virtual ~SingleBlockActivity() = default;

	// Placement 
	virtual void rotateForcedLocal(glm::ivec2 center, MOVEABLE::ROT rotation) override {};

	// Moveable
	virtual bool canMoveLocal(GameState& gameState, MOVEABLE::DIR dir, ActivityIgnoringGroup& ignore) override;

	// Traces Placement
	virtual bool canFillTracesLocal(GameState& gameState) override;
	virtual void fillTracesLocalForced(GameState& gameState) override;
	virtual void removeTracesLocalForced(GameState& gameState) override;

	// Traces Moveable
	virtual void removeMoveableTracesLocal(GameState& gameState) override;
	virtual void leaveMoveableTracesLocal(GameState& gameState) override;

	// Tree Informations
	virtual void getTreeMembers(std::vector<Activity*>& members) override;

	// Render
	virtual void appendSelectionInfo(GameState& gameState, RenderInfo& renderInfo, glm::vec4 color);
	virtual void appendStaticRenderInfo(GameState& gameState, StaticWorldRenderInfo& staticWorldRenderInfo) override;

	virtual void fillModifyingMap(ModifyerBase& modifyer) override;

	virtual void save(Saver& saver);
	virtual bool load(Loader& loader);
};

