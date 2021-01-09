#include "common.h"

#include "TextRenderer.h"

#include "CameraInfo.h"
#include "TextureLoader.h"
#include "ShaderLoader.h"
#include "GLEnableWrapper.h"
#include "CameraInfo.h"
#include "TextRenderInfo.h"
#include "BlitRenderer.h"
#include "Fonts.h"
#include "Colors.h"
#include "Locator.h"

#include <iostream>

void TextRenderer::render(TextRenderInfo const& textRenderInfo, Fonts const& fonts, GLuint target, CameraInfo const& cameraInfo) {
	for (auto& info : textRenderInfo.windowTextRenderInfos) {
		glm::ivec2 size = glm::floor(glm::vec2(info.screenRectangle.getPixelSize()) * info.screenRectangle.getAbsSize() / 2.0f);

		glm::vec2 topLeft = glm::floor((info.screenRectangle.getTopLeft() / 2.0f + 0.5f) * glm::vec2(info.screenRectangle.getPixelSize()));

		Locator<BlitRenderer>::ref().render(
			info.uvs,
			info.pos,
			target,
			{ topLeft.x, topLeft.y - size.y, size.x, size.y },
			fonts.fontAtlas.ID,
			info.depth,
			false,
			info.offset,
			COLORS::UI::TEXT
		);
	}
}

TextRenderer::TextRenderer() {
}

TextRenderer::~TextRenderer() {
}
