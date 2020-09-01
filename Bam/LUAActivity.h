#pragma once

#include "Grouper.h"
#include "ActivityLuaTest.h"

class LuaActivity : public SingleGrouper
{
private:
	bool interrupt;

public:
	ActivityLuaTest lua{ *this };

	void start(GameState& gameState);
	void stop();

	virtual glm::ivec2 getOrigin() override;

	LuaActivity() = default;
	LuaActivity(Handle self);
	virtual ~LuaActivity() = default;

	virtual bool moveableIdleLocal() override;

	// Placement
	virtual void rotateForcedLocal(glm::ivec2 center, Activity::ROT rotation) override;

	// Activity
	virtual bool canActivityLocal(GameState& gameState, int32_t type) override;
	virtual void applyActivityLocalForced(GameState& gameState, int32_t type, int32_t pace) override;

	// Moveable
	virtual bool canMoveLocal(GameState& gameState, Activity::DIR dir, ActivityIgnoringGroup& ignore) override;

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
	// Already implemented in SingleGrouper

	// Serial
	virtual Activity::TYPE getType() override;
	virtual void save(Saver& saver) override;
	virtual bool load(Loader& loader) override;

	// Render
	virtual void appendStaticRenderInfo(GameState const& gameState, StaticWorldRenderInfo& staticWorldRenderInfo) override;
	virtual void appendSelectionInfo(GameState const& gameState, RenderInfo& renderInfo, glm::vec4 color) override;
};

