#pragma once

#include <any>
#include "ControlState.h"

class GameState;
class ControlState;
struct RenderInfo;

namespace UIO
{
	typedef enum
	{
		STOP,
		CONTINUE,
	} CONTINUATION;
}

struct ScreenRectangle
{
	glm::ivec2 screenPixels;
	//glm::vec2 screenSize;
	//glm::vec2 screenPos;
	glm::vec2 bot;
	glm::vec2 top;

	glm::ivec2 getPixelSize();
	glm::ivec2 getPixelPos();
	bool contains(glm::vec2 p);
	void scaleAnchorTopLeft(glm::vec2 s);

	void set(glm::vec2 p1, glm::vec2 p2);
	glm::vec2 topLeft();
	glm::vec2 bottomRight();
};

struct ContinuationInformation
{
	UIO::CONTINUATION cont;
	std::any payload;
};

class UIOController;
class UIOController
{
protected:
	void addElement(std::unique_ptr<UIOController> element);

public:
	ScreenRectangle screenRectangle;

	std::vector<std::unique_ptr<UIOController>> elements;

	virtual void addRenderInfo(RenderInfo& renderInfo);
	virtual ScreenRectangle updateSizeUp(ScreenRectangle newScreenRectangle) = 0;

	virtual void clickTest(glm::vec2 pos, glm::vec2 change) = 0;

	UIOController();
	virtual ~UIOController();
private:
	using CallBack = std::function<ContinuationInformation(GameState&, ControlState&, UIOController*)>;
	std::vector<std::pair<BindControl, CallBack>> binds;


	//ContinuationInformation run(ControlState& controlState, GameState& gameState, UIOController& returnController);
};

class UIOWindowTest : public UIOController
{
public:
	virtual void addRenderInfo(RenderInfo& renderInfo) override;
	virtual ScreenRectangle updateSizeUp(ScreenRectangle newScreenRectangle) override;

	UIOWindowTest();
	virtual ~UIOWindowTest();

	// Inherited via UIOController
	virtual void clickTest(glm::vec2 pos, glm::vec2 change) override;
};

class UIOWindowTileTest : public UIOController
{
private:
	std::vector<std::vector<UIOController*>> rows;

public:
	virtual void addRenderInfo(RenderInfo& renderInfo) override;
	virtual ScreenRectangle updateSizeUp(ScreenRectangle newScreenRectangle) override;

	void nextRow();
	void add(std::unique_ptr<UIOController> c);

	UIOWindowTileTest();
	virtual ~UIOWindowTileTest();

	// Inherited via UIOController
	virtual void clickTest(glm::vec2 pos, glm::vec2 change) override;
};



class UIOWindowResize : public UIOController
{
public:
	bool scaling = false;
	glm::vec2 scaled;
	UIOController* main;
	UIOController* bottomScale;
	UIOController* sideScale;

	virtual ScreenRectangle updateSizeUp(ScreenRectangle newScreenRectangle) override;
	virtual void addRenderInfo(RenderInfo& renderInfo) override;

	UIOWindowResize(std::unique_ptr<UIOController> c);
	virtual ~UIOWindowResize();
	virtual void clickTest(glm::vec2 pos, glm::vec2 change) override;
};
