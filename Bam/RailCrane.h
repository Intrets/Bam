#pragma once

#include "Grouper.h"

namespace RAILCRANE
{
	typedef enum
	{
		STATIONARY,
		LEFT,
		RIGHT,
	} DIR;
}

class RailCrane : public Grouper 
{
public:
	int supportTex;
	int shaftTex;
	int anchorTex;

	MOVEABLE::DIR orientation;
	RAILCRANE::DIR anchorDirection;
	int length;
	int anchorIndexPos;

	WeakReference<Activity, Anchor> child;

	RailCrane(Handle self, GameState& gameState, glm::ivec2 pos, bool leavetraces = true);
	RailCrane();
	~RailCrane();

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

	// Tree Information
	virtual void getTreeMembers(std::vector<Activity*>& members) override;
	
	// Serial
	virtual ACTIVITY::TYPE getType() override;
	virtual void save(Saver& saver);
	virtual bool load(Loader& loader);

	// Render
	virtual void appendSelectionInfo(GameState& gameState, RenderInfo& renderInfo, glm::vec4 color) override;
	virtual void appendStaticRenderInfo(GameState& gameState, StaticWorldRenderInfo& staticWorldRenderInfo) override;

	// Modyfing
	virtual void modifyMember(GameState& gameState, std::string& name, std::vector<std::string>& value) override;
	virtual std::stringstream& getMembers(std::stringstream& out) override;

	// Inherited via Grouper
	virtual bool addChild(WeakReference<Activity, Activity> ref) override;
};

