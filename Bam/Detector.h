#pragma once

#include "SingleBlockActivity.h"

class Detector : public SingleBlockActivity
{
public:
	Detector() = default;
	Detector(Handle self, glm::ivec2 pos);
	virtual ~Detector() = default;

	// Placement
	// Already Implemented in SingleBlockActivity

	// Activity
	virtual bool canActivityLocal(GameState& gameState, int32_t type) override;
	virtual void applyActivityLocalForced(GameState& gameState, int32_t type, int32_t pace) override;

	// Moveable
	// Already Implemented in SingleBlockActivity

	// Traces Placement
	// Already Implemented in SingleBlockActivity

	// Traces Activity
	virtual void removeActivityTracesLocal(GameState& gameState) override;
	virtual void leaveActivityTracesLocal(GameState& gameState) override;

	// Traces Moveable
	// Already Implemented in SingleBlockActivity

	// Tree Information
	// Already Implemented in SingleBlockActivity

	// Serial
	virtual ACTIVITY::TYPE getType() override;

	// Render
	// Already Implemented in SingleBlockActivity
};

