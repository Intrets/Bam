#pragma once

#include "Block.h"

class GameState;
struct RenderInfo;
class UIState;
class Inventory;

struct ShapeInfo
{
	SHAPE::TYPE type;
	std::vector<ACTIVITY::DIR> dirs;

	bool validDirection(ACTIVITY::DIR dir) const;
	ACTIVITY::DIR toDir() const;
};

class ActivityGhost
{
public:
	glm::ivec2 origin;
	glm::ivec2 size;

	std::vector<std::vector<std::optional<ShapeInfo>>> shapes;

	bool validate(GameState& gameState);

	void addRenderInfo(GameState& gameState, RenderInfo& renderInfo, UIState& uiState, Inventory& inventory);

	ActivityGhost();
	virtual ~ActivityGhost() = default;
};

