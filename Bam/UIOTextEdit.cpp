#include "common.h"

#include "UIOTextEdit.h"
#include "RenderInfo.h"
#include "TextRenderer.h"
#include "GameState.h"
#include "StringHelpers.h"

UIOTextEdit::UIOTextEdit(Handle self) {
	selfHandle = self;
	lines.push_back("11234567890");
	lines.push_back("21234567890");
	lines.push_back("31234567890");
	lines.push_back("41234567890");
	lines.push_back("51234567890");
	lines.push_back("61234567890");
	lines.push_back("71234567890");

	addBind({ CONTROLS::ACTION0, CONTROLSTATE::CONTROLSTATE_PRESSED },
			[&](GameState& gameState, ControlState& controlState, UIOBase* self_) -> bool {
		focus = self_->contains(gameState.getPlayerCursorScreenSpace());
		return false;
	});

	addBind({ CONTROLS::TEST_UP, CONTROLSTATE::CONTROLSTATE_PRESSED | CONTROLSTATE::CONTROLSTATE_REPEAT },
			[&](GameState& gameState, ControlState& controlState, UIOBase* self_) -> bool {
		this->moveCursor({ 0,-1 });
		return false;
	});
	addBind({ CONTROLS::TEST_DOWN, CONTROLSTATE::CONTROLSTATE_PRESSED | CONTROLSTATE::CONTROLSTATE_REPEAT },
			[&](GameState& gameState, ControlState& controlState, UIOBase* self_) -> bool {
		this->moveCursor({ 0,1 });
		return false;
	});
	addBind({ CONTROLS::TEST_LEFT, CONTROLSTATE::CONTROLSTATE_PRESSED | CONTROLSTATE::CONTROLSTATE_REPEAT },
			[&](GameState& gameState, ControlState& controlState, UIOBase* self_) -> bool {
		this->moveCursor({ -1,0 });
		return false;
	});
	addBind({ CONTROLS::TEST_RIGHT, CONTROLSTATE::CONTROLSTATE_PRESSED | CONTROLSTATE::CONTROLSTATE_REPEAT },
			[&](GameState& gameState, ControlState& controlState, UIOBase* self_) -> bool {
		this->moveCursor({ 1,0 });
		return false;
	});

	addBind({ CONTROLS::CHAR_BUFFER_CHANGED, CONTROLSTATE::CONTROLSTATE_PRESSED },
			[&](GameState& gameState, ControlState& controlState, UIOBase* self_) -> bool {
		if (focus) {
			insertText(controlState.getCharBuffer());
		}
		return false;
	});

	addBind({ CONTROLS::BACKSPACE, CONTROLSTATE::CONTROLSTATE_PRESSED | CONTROLSTATE::CONTROLSTATE_REPEAT },
			[&](GameState& gameState, ControlState& controlState, UIOBase* self_) -> bool {
		if (focus) {
			backspaceChar();
		}
		return false;
	});

	addBind({ CONTROLS::DELETE, CONTROLSTATE::CONTROLSTATE_PRESSED | CONTROLSTATE::CONTROLSTATE_REPEAT },
			[&](GameState& gameState, ControlState& controlState, UIOBase* self_) -> bool {
		if (focus) {
			deleteChar();
		}
		return false;
	});

	addBind({ CONTROLS::TAB, CONTROLSTATE::CONTROLSTATE_PRESSED | CONTROLSTATE::CONTROLSTATE_REPEAT },
			[&](GameState& gameState, ControlState& controlState, UIOBase* self_) -> bool {
		if (focus) {
			insertText("    ");
		}
		return false;
	});
}

ScreenRectangle UIOTextEdit::updateSize(ScreenRectangle newScreenRectangle) {
	screenRectangle = newScreenRectangle;
	return screenRectangle;
}

int32_t UIOTextEdit::addRenderInfo(RenderInfo& renderInfo, int32_t depth) {
	auto& cameraInfo = renderInfo.cameraInfo;
	int fontSize = 16;
	auto& textRenderer = *renderInfo.textRenderInfo.textRendererRef;

	fontWidth = textRenderer.fontWidth;

	glm::vec2 fontSize2 = { 2 * fontSize / textRenderer.fontWidth,  2 * fontSize };
	glm::vec2 fontScale = fontSize2 / glm::vec2(cameraInfo.x, cameraInfo.y);

	// TODO: seperate font class/manager
	viewSize = screenRectangle.size() / fontScale;
	viewHorizontal[1] = viewHorizontal[0] + viewSize.x;
	viewVertical[1] = viewVertical[0] + viewSize.y;


	glm::vec2 start = screenRectangle.topLeft();

	int32_t istart = glm::max(0, viewVertical[0]);
	int32_t iend = glm::min(static_cast<int32_t>(lines.size()), viewVertical[1]);

	auto pos = start;
	for (int i = istart; i < iend; i++) {
		pos.x = start.x;
		pos.y -= fontScale.y;
		auto& line = lines[i];

		int32_t jstart = glm::max(0, viewHorizontal[0]);
		int32_t jend = glm::min(static_cast<int32_t>(line.size()), viewHorizontal[1]);

		for (int j = jstart; j < jend; j++) {
			//auto pos = 
			renderInfo.textRenderInfo.addChar(pos, line[j], fontScale);
			pos.x += fontScale.x;
		}
	}

	auto relativeCursorPos = cursor - glm::ivec2(viewHorizontal[0], viewVertical[0]);
	relativeCursorPos.y *= -1;
	relativeCursorPos.y -= 1;
	renderInfo.uiRenderInfo.addRectangle(start + glm::vec2(relativeCursorPos) * fontScale, start + glm::vec2(1 + relativeCursorPos) * fontScale, glm::vec4(0.5f, 0, 0, 0.5f));

	return depth;
}

void UIOTextEdit::moveCursor(glm::ivec2 p) {
	cursor += p;
	cursor = glm::max(cursor, glm::ivec2(0));

	cursor.y = glm::min(cursor.y, static_cast<int32_t>(lines.size() - 1));
	cursor.x = glm::min(cursor.x, static_cast<int32_t>(lines[cursor.y].size()));

	if (cursor.x >= viewHorizontal[1]) {
		int32_t d = cursor.x - viewHorizontal[1] + 1;
		viewHorizontal += d;
	}
	else if (cursor.x < viewHorizontal[0]) {
		int32_t d = cursor.x - viewHorizontal[0];
		viewHorizontal += d;
	}

	if (cursor.y >= viewVertical[1]) {
		int32_t d = cursor.y - viewVertical[1] + 1;
		viewVertical += d;
	}
	else if (cursor.y < viewVertical[0]) {
		int32_t d = cursor.y - viewVertical[0];
		viewVertical += d;
	}
}

void UIOTextEdit::insertText(std::string text) {
	for (auto c : text) {
		if (c == '\n') {
			auto first = std::string(lines[cursor.y].begin(), lines[cursor.y].begin() + cursor.x);
			auto second = std::string(lines[cursor.y].begin() + cursor.x, lines[cursor.y].end());

			lines[cursor.y] = first;
			cursor.x = 0;
			cursor.y++;
			lines.insert(lines.begin() + cursor.y, second);
		}
		else {
			lines[cursor.y].insert(cursor.x, std::string(1, c));
			cursor.x++;
		}
	}
	moveCursor(glm::ivec2(0));
}

void UIOTextEdit::backspaceChar() {
	if ( cursor == glm::ivec2(0)) {
		return;
	}
	if (cursor.x == 0) {
		cursor.x = static_cast<int32_t>(lines[cursor.y - 1].size());
		lines[cursor.y - 1] += lines[cursor.y];
		lines.erase(lines.begin() + cursor.y, lines.begin() + cursor.y + 1);
		cursor.y--;
	}
	else {
		lines[cursor.y].erase(cursor.x - 1, 1);
		cursor.x = glm::max(cursor.x - 1, 0);
	}
	moveCursor(glm::ivec2(0));
}

void UIOTextEdit::deleteChar() {
	auto test = glm::ivec2(lines[cursor.y].size(), lines.size() - 1);
	if (cursor == test){ 
		return;
	}
	if (cursor.x == lines[cursor.y].size()) {
		lines[cursor.y] += lines[cursor.y + 1];
		lines.erase(lines.begin() + cursor.y + 1, lines.begin() + cursor.y + 2);
	}
	else {
		lines[cursor.y].erase(cursor.x, 1);
		cursor.x = glm::max(cursor.x, 0);
	}
	moveCursor(glm::ivec2(0));
}
