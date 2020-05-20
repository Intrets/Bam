#include "common.h"

#include "UIOController.h"
#include "RenderInfo.h"
#include <iostream>

void UIOController::addElement(std::unique_ptr<UIOController> element) {
	elements.push_back(std::move(element));
}

void UIOController::addRenderInfo(RenderInfo& renderInfo) {
	for (auto& element : elements) {
		element->addRenderInfo(renderInfo);
	}
}

UIOController::UIOController() {
}

UIOController::~UIOController() {
}

UIOWindowTest::UIOWindowTest() {
}

UIOWindowTest::~UIOWindowTest() {
}

void UIOWindowTest::clickTest(glm::vec2 pos, glm::vec2 change) {
	if (screenRectangle.contains(pos)) {
		std::cout << "clicked\n";
	}
}

ScreenRectangle UIOWindowTest::updateSizeUp(ScreenRectangle newScreenRectangle) {
	screenRectangle = newScreenRectangle;
	return screenRectangle;
}

void UIOWindowTest::addRenderInfo(RenderInfo& renderInfo) {
	auto& ui = renderInfo.uiRenderInfo;
	//auto& text = renderInfo.textRenderInfo;

	ui.addRectangle(
		screenRectangle.bot,
		screenRectangle.top,
		{ 1,1,1,0.5 }
	);
}

glm::ivec2 ScreenRectangle::getPixelSize() {
	// TODO
	assert(false);
	return glm::ivec2(0);
}

glm::ivec2 ScreenRectangle::getPixelPos() {
	// TODO
	assert(false);
	return glm::ivec2(0);
}

bool ScreenRectangle::contains(glm::vec2 p) {
	return (bot.x < p.x) && (p.x < top.x) && (bot.y < p.y) && (p.y < top.y);
}

void ScreenRectangle::scaleAnchorTopLeft(glm::vec2 s) {
	glm::vec2 v1 = topLeft();
	glm::vec2 v2 = bottomRight();
	//s.y *= -1.0f;
	glm::vec2 diag = (v2 - v1) * s;

	set(v1, v1 + diag);
}

void ScreenRectangle::set(glm::vec2 p1, glm::vec2 p2) {
	bot = glm::min(p1, p2);
	top = glm::max(p1, p2);
}

glm::vec2 ScreenRectangle::topLeft() {
	return glm::vec2(bot.x, top.y);
}

glm::vec2 ScreenRectangle::bottomRight() {
	return glm::vec2(top.x, bot.y);
}

void UIOWindowTileTest::addRenderInfo(RenderInfo& renderInfo) {
	UIOController::addRenderInfo(renderInfo);
	renderInfo.uiRenderInfo.addRectangle(
		screenRectangle.top,
		screenRectangle.bot,
		{ 1,1,1,0.5 }
	);
}

ScreenRectangle UIOWindowTileTest::updateSizeUp(ScreenRectangle newScreenRectangle) {
	screenRectangle = newScreenRectangle;

	//float spacing = 0.05f;
	glm::vec2 screenSize = screenRectangle.bottomRight() - screenRectangle.topLeft();

	float y = screenRectangle.topLeft().y;
	float dy = screenSize.y / static_cast<float>(rows.size());

	for (auto& row : rows) {
		if (row.empty()) {
			continue;
		}
		float x = screenRectangle.topLeft().x;
		float dx = screenSize.x / static_cast<float>(row.size());
		glm::vec2 size = { dx, dy };
		for (auto element : row) {

			ScreenRectangle newRectangle;
			newRectangle.set(
				glm::vec2(x, y),
				glm::vec2(glm::vec2( x + dx * 0.9f, y + dy * 0.9f))
			);
			element->updateSizeUp(newRectangle);
			x += dx;
		}
		y += dy;
	}

	return newScreenRectangle;
}

void UIOWindowTileTest::nextRow() {
	rows.push_back(std::vector<UIOController*>());
}

void UIOWindowTileTest::add(std::unique_ptr<UIOController> c) {
	rows.back().push_back(c.get());
	addElement(std::move(c));
}

UIOWindowTileTest::UIOWindowTileTest() {
}

UIOWindowTileTest::~UIOWindowTileTest() {
}

void UIOWindowTileTest::clickTest(glm::vec2 pos, glm::vec2 change) {
	if (screenRectangle.contains(pos)) {
		std::cout << "clicked\n";
	}
	for (auto& element : elements) {
		element->clickTest(pos, change);
	}
}

ScreenRectangle UIOWindowResize::updateSizeUp(ScreenRectangle newScreenRectangle) {
	screenRectangle = newScreenRectangle;
	newScreenRectangle.scaleAnchorTopLeft(scaled);
	main->updateSizeUp(newScreenRectangle);
	return screenRectangle;
}

void UIOWindowResize::addRenderInfo(RenderInfo& renderInfo) {
	main->addRenderInfo(renderInfo);

	renderInfo.uiRenderInfo.addRectangle({ -1,1 }, { -0.75f,0.75f }, { 1,0,0,1 });
	renderInfo.uiRenderInfo.addRectangle({ 1,1 }, { 0.75f,0.75f }, { 0,1,0,1 });
	renderInfo.uiRenderInfo.addRectangle({ 1,-1 }, { 0.75f,-0.75f }, { 0,0,1,1 });
	renderInfo.uiRenderInfo.addRectangle({ -1,-1 }, { -0.75f,-0.75f }, { 0,1,1,1 });
}

UIOWindowResize::UIOWindowResize(std::unique_ptr<UIOController> c) {
	scaled = glm::vec2(1.0f);
	main = c.get();
	UIOController::addElement(std::move(c));
}

UIOWindowResize::~UIOWindowResize() {
}

void UIOWindowResize::clickTest(glm::vec2 pos, glm::vec2 change_) {
	auto sub = screenRectangle;
	sub.scaleAnchorTopLeft(scaled);
	if (!sub.contains(pos)) {
		scaling = false;
	}
	else {
		glm::vec2 change = sub.bottomRight() - pos;
		//change = change_;
		//change.y *= -1.0f;
		glm::vec2 diag = sub.topLeft() - sub.bottomRight();
		glm::vec2 scale = (diag - change) / diag;
		scaled /= scale;
		//glm::vec2 x = { change.x, change.y };
		//scaled *= (sub.screenSize + x/4.0f) / sub.screenSize;
		//scaled = glm::max(glm::vec2(0.1f), glm::min(glm::vec2(1.0f), scaled));
	}
	main->clickTest(pos, change_);
}
