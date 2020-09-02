#pragma once

#include "ActivityLuaTest.h"
#include "SingleBlockActivity.h"

class LuaActivity : public SingleBlockActivity
{
private:
	bool interrupt;

public:
	ActivityLuaTest lua{ *this };

	void start(GameState& gameState);
	void stop();

	LuaActivity() = default;
	LuaActivity(Handle self, GameState& gameState, glm::ivec2 pos);
	virtual ~LuaActivity() = default;

	virtual bool moveableIdleLocal() override;

	// Placement
	// Already implemented in SingleBlockActivity

	// Activity
	virtual bool canActivityLocal(GameState& gameState, int32_t type) override;
	virtual void applyActivityLocalForced(GameState& gameState, int32_t type, int32_t pace) override;

	// Moveable
	// Already implemented in SingleBlockActivity

	// Traces Placement
	// Already implemented in SingleBlockActivity

	// Traces Activity
	virtual void removeActivityTracesLocal(GameState& gameState) override;
	virtual void leaveActivityTracesLocal(GameState& gameState) override;

	// Traces Moveable
	// Already implemented in SingleBlockActivity

	// Tree Information
	// Already implemented in SingleBlockActivity

	// Serial
	virtual Activity::TYPE getType() override;
	virtual void save(Saver& saver) override;
	virtual bool load(Loader& loader) override;
};

