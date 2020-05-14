#pragma once

#include "Grouper.h"
#include "Activity.h"

namespace PISTON
{
	typedef enum
	{
		STATIONARY,
		RETRACT,
		EXTEND,
	} DIR;
}

class Piston : public Grouper
{
public:
	int cogTex;
	int ropeTex;
	int headTex;
	int length;

	glm::ivec2 direction;
	MOVEABLE::DIR headDir;

	PISTON::DIR state = PISTON::STATIONARY;


	Piston(Handle self, GameState& gameState, glm::ivec2 pos, MOVEABLE::DIR dir, bool leaveTraces = true);
	Piston();
	~Piston();

	// Placement
	virtual void rotateForcedLocal(glm::ivec2 center, MOVEABLE::ROT rotation) override;

	// Activity
	virtual bool canActivityLocal(GameState& gameState, int type) override;
	virtual void applyActivityLocalForced(GameState& gameState, int type, int pace) override;

	// Moveable
	virtual bool canMoveLocal(GameState& gameState, MOVEABLE::DIR dir, ActivityIgnoringGroup& ignore) override;

	// Traces Placement
	virtual bool canFillTracesLocal(GameState& gameState) override;
	virtual void fillTracesLocalForced(GameState& gameState) override;
	virtual void removeTracesLocalForced(GameState& gameState) override;

	// Traces Activity
	virtual void removeActivityTracesLocal(GameState& gameState) override;
	virtual void leaveActivityTracesLocal(GameState& gameState) override;
	
	// Traces Moveable
	virtual void removeMoveableTracesLocal(GameState& gameState) override;
	virtual void leaveMoveableTracesLocal(GameState& gameState) override;

	// Serial
	virtual ACTIVITY::TYPE getType() override;
	virtual void save(Saver& saver) override;
	virtual bool load(Loader& loader) override;

	// Render
	virtual void appendSelectionInfo(GameState& gameState, RenderInfo& renderInfo, glm::vec4 color) override;
	virtual void appendStaticRenderInfo(GameState& gameState, StaticWorldRenderInfo& staticWorldRenderInfo) override;

	// Modifying
	virtual void fillModifyingMap(ModifyerBase& modifier) override;
	virtual void modifyMember(GameState& gameState, std::string& name, std::vector<std::string>& value) override;
	virtual std::stringstream& getMembers(std::stringstream& out) override;
};

