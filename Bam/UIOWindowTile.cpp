#include "common.h"

#include "UIOWindowTile.h"
#include "RenderInfo.h"

void UIOWindowTile::nextRow() {
	rows.push_back(std::vector<UIOBase*>());
}

void UIOWindowTile::add(UniqueReference<UIOBase, UIOBase> c) {
	rows.back().push_back(c.get());
	addElement(std::move(c));
}

UIOWindowTile::UIOWindowTile(Handle self) {
	selfHandle = self;
	nextRow();
}

ScreenRectangle UIOWindowTile::updateSize(ScreenRectangle newScreenRectangle) {
	screenRectangle = newScreenRectangle;

	//float spacing = 0.05f;
	glm::vec2 screenSize = screenRectangle.getBottomRight() - screenRectangle.getTopLeft();

	float y = screenRectangle.getTopLeft().y;
	float dy = screenSize.y / static_cast<float>(rows.size());

	for (auto& row : rows) {
		if (row.empty()) {
			continue;
		}
		float x = screenRectangle.getTopLeft().x;
		float dx = screenSize.x / static_cast<float>(row.size());
		glm::vec2 size = { dx, dy };
		for (auto element : row) {

			ScreenRectangle newRectangle;
			newRectangle.screenPixels = newScreenRectangle.screenPixels;
			newRectangle.set(
				glm::vec2(x, y),
				glm::vec2(glm::vec2(x + dx, y + dy))
			);
			element->updateSize(newRectangle);
			x += dx;
		}
		y += dy;
	}

	return newScreenRectangle;
}

int32_t UIOWindowTile::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {
	depth = UIOBase::addRenderInfo(gameState, renderInfo, depth);
	//renderInfo.uiRenderInfo.addRectangle(
	//	screenRectangle.top,
	//	screenRectangle.bot,
	//	{ 1,1,1,0.5 },
	//	depth 
	//);
	return depth + 1;
}
