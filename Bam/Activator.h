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
	virtual bool canActivityLocal(GameState& gameState, int type) override;
	virtual void applyActivityLocalForced(GameState& gameState, int type, int pace) override;

	// Traces Activity
	virtual void removeActivityTracesLocal(GameState& gameState);
	virtual void leaveActivityTracesLocal(GameState& gameState);

	// Serial
	virtual ACTIVITY::TYPE getType() override;
	virtual void save(Saver& saver) override;
	virtual bool load(Loader& loader) override;

	// Modifying 
	virtual void modifyMember(GameState& gameState, std::string& name, std::vector<std::string>& value) override;
	virtual std::stringstream& getMembers(std::stringstream& out) override;
};

