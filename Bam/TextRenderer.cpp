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

void TextRenderer::render(TextRenderInfo& textRenderInfo, Fonts& fonts, GLuint target, CameraInfo& cameraInfo) {
	for (auto& info : textRenderInfo.windowTextRenderInfos) {
		glm::ivec2 bot = (info.screenRectangle.bot / 2.0f + 0.5f) * glm::vec2(info.screenRectangle.getPixelSize());
		glm::ivec2 size = info.screenRectangle.size() / 2.0f * glm::vec2(info.screenRectangle.getPixelSize());
		Locator<BlitRenderer>::ref().render(
			info.uvs,
			info.pos,
			target,
			{ bot.x, bot.y, size.x, size.y },
			fonts.fontAtlas.ID,
			false
		);
	}
}

TextRenderer::TextRenderer() {
}

TextRenderer::~TextRenderer() {
}
