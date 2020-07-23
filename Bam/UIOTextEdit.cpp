#include "common.h"

#include "UIOTextEdit.h"
#include "RenderInfo.h"
#include "TextRenderer.h"
#include "GameState.h"
#include "StringHelpers.h"
#include "State.h"
#include "UIOCallBackParams.h"

UIOTextEdit::UIOTextEdit(Handle self) {
	this->selfHandle = self;
	this->lines.push_back("11234567890");
	this->lines.push_back("21234567890");
	this->lines.push_back("31234567890");
	this->lines.push_back("41234567890");
	this->lines.push_back("51234567890");
	this->lines.push_back("61234567890");
	this->lines.push_back("71234567890");

	addGlobalBind({ CONTROLS::ACTION0, CONTROLSTATE::CONTROLSTATE_PRESSED },
				  [&](UIOCallBackParams& params, UIOBase* self_) -> CallBackBindResult {
		this->focus = this->contains(params.uiState.getCursorPositionScreen());
		if (this->focus) {
			return BIND_RESULT::CONTINUE | BIND_RESULT::FOCUS | BIND_RESULT::CONSUME;
		}
		else {
			return BIND_RESULT::CONTINUE;
		}
	});

	addFocussedBind({ CONTROLS::TEST_UP, CONTROLSTATE::CONTROLSTATE_PRESSED | CONTROLSTATE::CONTROLSTATE_REPEAT },
					[&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		if (!this->focus) {
			return BIND_RESULT::CONTINUE;
		}
		this->moveCursor({ 0,-1 });
		return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
	});
	addFocussedBind({ CONTROLS::TEST_DOWN, CONTROLSTATE::CONTROLSTATE_PRESSED | CONTROLSTATE::CONTROLSTATE_REPEAT },
					[&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		if (!this->focus) {
			return BIND_RESULT::CONTINUE;
		}
		this->moveCursor({ 0,1 });
		return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
	});
	addFocussedBind({ CONTROLS::TEST_LEFT, CONTROLSTATE::CONTROLSTATE_PRESSED | CONTROLSTATE::CONTROLSTATE_REPEAT },
					[&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		if (!this->focus) {
			return BIND_RESULT::CONTINUE;
		}
		this->moveCursor({ -1,0 });
		return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
	});
	addFocussedBind({ CONTROLS::TEST_RIGHT, CONTROLSTATE::CONTROLSTATE_PRESSED | CONTROLSTATE::CONTROLSTATE_REPEAT },
					[&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		if (!this->focus) {
			return BIND_RESULT::CONTINUE;
		}
		this->moveCursor({ 1,0 });
		return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
	});

	addFocussedBind({ CONTROLS::CHAR_BUFFER_CHANGED, CONTROLSTATE::CONTROLSTATE_PRESSED },
					[&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		if (!this->focus) {
			return BIND_RESULT::CONTINUE;
		}
		insertText(state.controlState.getCharBuffer());
		return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
	});

	addFocussedBind({ CONTROLS::BACKSPACE, CONTROLSTATE::CONTROLSTATE_PRESSED | CONTROLSTATE::CONTROLSTATE_REPEAT },
					[&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		if (!this->focus) {
			return BIND_RESULT::CONTINUE;
		}
		backspaceChar();
		return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
	});

	addFocussedBind({ CONTROLS::DELETE, CONTROLSTATE::CONTROLSTATE_PRESSED | CONTROLSTATE::CONTROLSTATE_REPEAT },
					[&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		if (!this->focus) {
			return BIND_RESULT::CONTINUE;
		}
		deleteChar();
		return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
	});

	addFocussedBind({ CONTROLS::TAB, CONTROLSTATE::CONTROLSTATE_PRESSED | CONTROLSTATE::CONTROLSTATE_REPEAT },
					[&](UIOCallBackParams& state, UIOBase* self_) -> CallBackBindResult {
		if (!this->focus) {
			return BIND_RESULT::CONTINUE;
		}
		insertText("    ");
		return BIND_RESULT::CONTINUE | BIND_RESULT::CONSUME;
	});
}

ScreenRectangle UIOTextEdit::updateSize(ScreenRectangle newScreenRectangle) {
	if (!this->screenRectangle.equals(newScreenRectangle)) {
		cachedText = std::nullopt;
	}
	this->screenRectangle = newScreenRectangle;
	return this->screenRectangle;
}

int32_t UIOTextEdit::addRenderInfo(GameState& gameState, RenderInfo& renderInfo, int32_t depth) {

	if (!this->cachedText.has_value()) {
		WindowTextRenderInfo textInfo(this->screenRectangle, Fonts::Font::ROBOTO_12, true);

		for (auto& line : this->lines) {
			textInfo.addString(line);
			textInfo.newLine();
		}

		this->cachedText = textInfo;
	}

	this->cachedText.value().setDepth(depth++);
	renderInfo.textRenderInfo.windowTextRenderInfos.push_back(this->cachedText.value());

	//TODO: new text rendering

	//auto& cameraInfo = renderInfo.cameraInfo;
	//int fontSize = 16;

	//auto& textRenderer = *renderInfo.textRenderInfo.textRendererRef;

	//fontWidth = textRenderer.fontWidth;

	//glm::vec2 fontSize2 = { 2 * fontSize / textRenderer.fontWidth,  2 * fontSize };
	//glm::vec2 fontScale = fontSize2 / glm::vec2(cameraInfo.x, cameraInfo.y);

	//// TODO: seperate font class/manager
	//viewSize = screenRectangle.size() / fontScale;
	//viewHorizontal[1] = viewHorizontal[0] + viewSize.x;
	//viewVertical[1] = viewVertical[0] + viewSize.y;

	//glm::vec2 start = screenRectangle.getTopLeft();

	//int32_t istart = glm::max(0, viewVertical[0]);
	//int32_t iend = glm::min(static_cast<int32_t>(this->lines.size()), viewVertical[1]);

	//auto pos = start;
	//for (int i = istart; i < iend; i++) {
	//	pos.x = start.x;
	//	pos.y -= fontScale.y;
	//	auto& line = this->lines[i];

	//	int32_t jstart = glm::max(0, viewHorizontal[0]);
	//	int32_t jend = glm::min(static_cast<int32_t>(line.size()), viewHorizontal[1]);

	//	for (int j = jstart; j < jend; j++) {
	//		//auto pos = 
	//		renderInfo.textRenderInfo.addChar(pos, line[j], fontScale, depth);
	//		pos.x += fontScale.x;
	//	}
	//}

	//auto relativeCursorPos = cursor - glm::ivec2(viewHorizontal[0], viewVertical[0]);
	//relativeCursorPos.y *= -1;
	//relativeCursorPos.y -= 1;
	//renderInfo.uiRenderInfo.addRectangle(start + glm::vec2(relativeCursorPos) * fontScale, start + glm::vec2(1 + relativeCursorPos) * fontScale, glm::vec4(0.5f, 0, 0, 0.5f), depth);
	renderInfo.uiRenderInfo.addRectangle(screenRectangle.bot, screenRectangle.top, { 0.9f, 0.9f, 0.9f, 1.0f }, depth++);

	return depth;
}

void UIOTextEdit::moveCursor(glm::ivec2 p) {
	this->cursor += p;
	this->cursor = glm::max(this->cursor, glm::ivec2(0));

	this->cursor.y = glm::min(this->cursor.y, static_cast<int32_t>(this->lines.size() - 1));
	this->cursor.x = glm::min(this->cursor.x, static_cast<int32_t>(this->lines[this->cursor.y].size()));

	if (this->cursor.x >= this->viewHorizontal[1]) {
		int32_t d = this->cursor.x - this->viewHorizontal[1] + 1;
		this->viewHorizontal += d;
	}
	else if (this->cursor.x < this->viewHorizontal[0]) {
		int32_t d = this->cursor.x - this->viewHorizontal[0];
		this->viewHorizontal += d;
	}

	if (this->cursor.y >= this->viewVertical[1]) {
		int32_t d = this->cursor.y - this->viewVertical[1] + 1;
		this->viewVertical += d;
	}
	else if (this->cursor.y < this->viewVertical[0]) {
		int32_t d = this->cursor.y - this->viewVertical[0];
		this->viewVertical += d;
	}
}

void UIOTextEdit::insertText(std::string text) {
	for (auto c : text) {
		if (c == '\n') {
			auto first = std::string(this->lines[this->cursor.y].begin(), this->lines[this->cursor.y].begin() + this->cursor.x);
			auto second = std::string(this->lines[this->cursor.y].begin() + this->cursor.x, this->lines[this->cursor.y].end());

			this->lines[this->cursor.y] = first;
			this->cursor.x = 0;
			this->cursor.y++;
			this->lines.insert(this->lines.begin() + this->cursor.y, second);
		}
		else {
			this->lines[this->cursor.y].insert(this->cursor.x, std::string(1, c));
			this->cursor.x++;
		}
	}
	this->cachedText = std::nullopt;
	moveCursor(glm::ivec2(0));
}

void UIOTextEdit::backspaceChar() {
	if (this->cursor == glm::ivec2(0)) {
		return;
	}
	if (this->cursor.x == 0) {
		this->cursor.x = static_cast<int32_t>(this->lines[this->cursor.y - 1].size());
		this->lines[this->cursor.y - 1] += this->lines[this->cursor.y];
		this->lines.erase(this->lines.begin() + this->cursor.y, this->lines.begin() + this->cursor.y + 1);
		this->cursor.y--;
	}
	else {
		this->lines[this->cursor.y].erase(this->cursor.x - 1, 1);
		this->cursor.x = glm::max(this->cursor.x - 1, 0);
	}
	this->cachedText = std::nullopt;
	moveCursor(glm::ivec2(0));
}

void UIOTextEdit::deleteChar() {
	auto test = glm::ivec2(this->lines[this->cursor.y].size(), this->lines.size() - 1);
	if (this->cursor == test) {
		return;
	}
	if (this->cursor.x == this->lines[this->cursor.y].size()) {
		this->lines[this->cursor.y] += this->lines[this->cursor.y + 1];
		this->lines.erase(this->lines.begin() + this->cursor.y + 1, this->lines.begin() + this->cursor.y + 2);
	}
	else {
		this->lines[this->cursor.y].erase(this->cursor.x, 1);
		this->cursor.x = glm::max(this->cursor.x, 0);
	}
	this->cachedText = std::nullopt;
	moveCursor(glm::ivec2(0));
}
