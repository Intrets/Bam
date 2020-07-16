#pragma once

#include "LogicSequencer.h"
#include "ReferenceManager.h"
#include "Activity.h"

enum class SELECTION_TYPE
{
	NOTHING,
	HOVERING,
	SELECTED,
};

class ActivitySelector 
{
public:
	SELECTION_TYPE type = SELECTION_TYPE::NOTHING;

	std::vector<ManagedReference<Activity, Activity>> history;

	ManagedReference<Activity, Activity> target;

	void selectTarget(GameState& gameState);
	void expandTarget();
	void shrinkTarget();
	void spawnHover(GameState& gameState, glm::ivec2 pos, ACTIVITY::TYPE activityType);
	void rotateHover(MOVEABLE::ROT rot);

	ActivitySelector();
	~ActivitySelector() = default;

	int32_t addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth);
};

