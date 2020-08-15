#pragma once

#include "SingleBlockActivity.h"

class Activator : public SingleBlockActivity
{
public:
	Activator() = default;
	Activator(Handle self, GameState& gameState, glm::ivec2 pos, std::string name, bool leaveTrace = true);

	virtual ~Activator() = default;

	virtual bool moveableIdleLocal() override;
	virtual bool activityIdleLocal() override;

	// Activity
	virtual bool canActivityLocal(GameState& gameState, int32_t type) override;
	virtual void applyActivityLocalForced(GameState& gameState, int32_t type, int32_t pace) override;

	// Traces Activity
	virtual void removeActivityTracesLocal(GameState& gameState);
	virtual void leaveActivityTracesLocal(GameState& gameState);

	// Serial
	virtual Activity::TYPE getType() override;
	virtual void save(Saver& saver) override;
	virtual bool load(Loader& loader) override;
};

