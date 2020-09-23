#pragma once

#include "SingleBlockActivity.h"
#include "LuaStore.h"
#include "ActivityMaterial.h"

class Forwarder : public SingleBlockActivity
{
private:
	LUASTORE::Args args;

	bool receiveMessage(GameState& gameState, LUASTORE::Args& args_);
	bool canReceiveMessage(GameState& gameState);

	friend class LuaActivity;
public:
	Forwarder() = default;
	Forwarder(Handle self, glm::ivec2 pos);
	virtual ~Forwarder() = default;

	// Placement
	// Already Implemented in SingleBlockActivity

	// Activity
	virtual bool canActivityLocal(GameState& gameState, int32_t type) override;
	virtual void applyActivityLocalForced(GameState& gameState, int32_t type, int32_t pace) override;
	virtual void postActivityStopLocal(GameState& gameState) override;

	// Moveable
	// Already Implemented in SingleBlockActivity

	// Traces Placement
	// Already Implemented in SingleBlockActivity

	// Traces Moveable
	// Already Implemented in SingleBlockActivity

	// Tree Information
	// Already Implemented in SingleBlockActivity

	// Serial
	virtual ACTIVITY::TYPE getType() override;

	// Render
	// Already Implemented in SingleBlockActivity

	virtual void save(Saver& saver) override;
	virtual bool load(Loader& loader) override;
};

