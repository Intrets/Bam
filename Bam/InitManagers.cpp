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

void initManagers(GLFWwindow* window) {
	Locator<ReferenceManager<UIOBase>>::provide(new ReferenceManager<UIOBase>());

	Locator<PathManager>::provide(new PathManager());
	Locator<OptionManager>::provide(new OptionManager());
	Locator<DebugRenderer>::provide(new DebugRenderer());

	Locator<PassThroughRenderer>::provide(new PassThroughRenderer());
	Locator<BlockIDTextures>::provide(new BlockIDTextures());
	Locator<DebugRenderInfo>::provide(new DebugRenderInfo());
	Locator<ReferenceManager<Activity>>::provide(new ReferenceManager<Activity>());
	Locator<ReferenceManager<InventoryItem>>::provide(new ReferenceManager<InventoryItem>());

	Locator<BlitRenderer>::provide(new BlitRenderer());
	Locator<BlitRendererArrayTexture>::provide(new BlitRendererArrayTexture());

	Locator<Fonts>::provide(new Fonts());
	Locator<Timer>::provide(new Timer());

	Locator<Log>::provide(new Log());

	loadBlocks();
	loadShapes();

	Locator<Inventory>::provide(new Inventory());
}
