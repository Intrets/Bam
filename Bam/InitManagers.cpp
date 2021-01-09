#include "common.h"

#include "InitManagers.h"

#include "DebugRenderInfo.h"
#include "Option.h"
#include "DebugRenderer.h"
#include "BlockIDTextures.h"
#include "Activity.h"
#include "UIOBase.h"
#include "Log.h"
#include "PassThroughRenderer.h"
#include "Fonts.h"
#include "BlitRenderer.h"
#include "Timer.h"
#include "Block.h"
#include "BlitRendererArrayTexture.h"
#include "Inventory.h"
#include "InventoryItem.h"
#include "Locator.h"
#include "PathManager.h"

void initManagers(GLFWwindow* window, PROGRAM::TYPE type) {
	Locator<PathManager>::provide(new PathManager());
	Locator<OptionManager>::provide(new OptionManager());

	switch (type) {
		case PROGRAM::TYPE::OFFLINE:
		case PROGRAM::TYPE::CLIENT:
		case PROGRAM::TYPE::SERVER:
			Locator<DebugRenderer>::provide(new DebugRenderer());
			Locator<PassThroughRenderer>::provide(new PassThroughRenderer());
			Locator<BlockIDTextures>::provide(new BlockIDTextures());
			Locator<DebugRenderInfo>::provide(new DebugRenderInfo());

			Locator<BlitRenderer>::provide(new BlitRenderer());
			Locator<BlitRendererArrayTexture>::provide(new BlitRendererArrayTexture());

			Locator<Fonts>::provide(new Fonts());

			loadBlocks(true);
			loadShapes(true);
			break;
		case PROGRAM::TYPE::HEADLESS_SERVER:
			loadBlocks(false);
			loadShapes(false);
			break;
		default:
			break;
	}

	Locator<Timer>::provide(new Timer());
	Locator<Log>::provide(new Log());
}
