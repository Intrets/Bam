#include "common.h"
#include "InitBinds.h"

#include "BindHandler.h"
#include "GameState.h"
#include "Option.h"

void initBinds(BindHandler& bindHandler) {
	//bindHandler.addBind(
	//	CONTROLS::LEFT, CONTROLSTATE_DOWN,
	//	[](GameState& gameState) -> void {
	//	gameState.playerPos += glm::vec2(-1.0f, 0.0f);
	//}
	//);
	//bindHandler.addBind(
	//	CONTROLS::RIGHT, CONTROLSTATE_DOWN,
	//	[](GameState& gameState) -> void {
	//	gameState.playerPos += glm::vec2(1.0f, 0.0f);
	//}
	//);
	//bindHandler.addBind(
	//	CONTROLS::UP, CONTROLSTATE_DOWN,
	//	[](GameState& gameState) -> void {
	//	gameState.playerPos += glm::vec2(0.0f, 1.0f);
	//}
	//);
	//bindHandler.addBind(
	//	CONTROLS::DOWN, CONTROLSTATE_DOWN,
	//	[](GameState& gameState) -> void {
	//	gameState.playerPos += glm::vec2(0.0f, -1.0f);
	//}
	//);
	//bindHandler.addBind(
	//	CONTROLS::ZOOM_IN, CONTROLSTATE_DOWN,
	//	[](GameState& gameState) -> void {
	//	Option<CL_VIEWPORTSCALE, float> viewportScale;
	//	viewportScale.setVal(viewportScale.getVal() * 1.1f);
	//}
	//);
	//bindHandler.addBind(
	//	CONTROLS::ZOOM_OUT, CONTROLSTATE_DOWN,
	//	[](GameState& gameState) -> void {
	//	Option<CL_VIEWPORTSCALE, float> viewportScale;
	//	viewportScale.setVal(viewportScale.getVal() / 1.1f);
	//}
	//);
	//bindHandler.addBind(
	//	CONTROLS::TEST_SAVE, CONTROLSTATE_PRESSED,
	//	[](GameState& gameState) -> void {
	//	Saver saver("test.save");
	//	saver.saveGame(gameState);
	//}
	//);
	//bindHandler.addBind(
	//	CONTROLS::TEST_LOAD, CONTROLSTATE_PRESSED,
	//	[](GameState& gameState) -> void {
	//	Loader loader("test.save");
	//	loader.loadGame(gameState);
	//}
	//);
	//bindHandler.addBind(
	//	CONTROLS::TOGGLE_DEBUG, CONTROLSTATE_PRESSED,
	//	[](GameState& gameState) -> void {
	//	Option<GR_DEBUG, bool>().setVal(!Option<GR_DEBUG, bool>().getVal());
	//}
	//);
}
