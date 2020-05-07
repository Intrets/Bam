#pragma once

#include "LogicSequencer.h"
#include "ReferenceManager.h"
#include "Activity.h"

class ActivitySelector : public LogicSequencer
{
public:
	std::vector<std::unique_ptr<ManagedReference<Activity, Activity>>> history;
	ManagedReference<Activity, Activity> target;

	ActivitySelector();
	~ActivitySelector() = default;

protected:
	virtual void appendRenderInfoInternal(GameState& gameState, RenderInfo& renderInfo) override;
};

