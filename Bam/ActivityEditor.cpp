#include "common.h"

#include "ActivityEditor.h"
#include "RenderInfo.h"
#include <sstream>
#include <iostream>
#include "StringHelpers.h"

void ActivityEditor::appendRenderInfoInternal(GameState& gameState, RenderInfo& renderInfo) {
	ActivitySelector::appendRenderInfoInternal(gameState, renderInfo);
}
