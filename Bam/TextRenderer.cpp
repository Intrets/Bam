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

void TextRenderer::render(TextRenderInfo const& textRenderInfo, Fonts const& fonts, GLuint target, CameraInfo const& cameraInfo) {
	for (auto& info : textRenderInfo.windowTextRenderInfos) {
		glm::ivec2 bot = glm::round((info.screenRectangle.getBottomLeft() / 2.0f + 0.5f) * glm::vec2(info.screenRectangle.getPixelSize()));
		glm::ivec2 size = glm::round(info.screenRectangle.getAbsSize() / 2.0f * glm::vec2(info.screenRectangle.getPixelSize()));
		Locator<BlitRenderer>::ref().render(
			info.uvs,
			info.pos,
			target,
			{ bot.x, bot.y, size.x, size.y },
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
