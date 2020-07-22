#include "common.h"

#include "InitManagers.h"

#include "DebugRenderInfo.h"
#include "ResourceManagerModel.h"
#include "ResourceManagerTexture.h"
#include "Option.h"
#include "DebugRenderer.h"
#include "BlockIDTextures.h"
#include "Activity.h"
#include "UIOBase.h"
#include "Log.h"
#include "PassThroughRenderer.h"
#include "Fonts.h"
#include "BlitRenderer.h"

void initManagers(GLFWwindow* window) {
	Locator<ReferenceManager<UIOBase>>::provide(new ReferenceManager<UIOBase>());

	Locator<PathManager>::provide(new PathManager());
	Locator<ResourceManagerModel>::provide(new ResourceManagerModel());
	Locator<ResourceManagerTexture>::provide(new ResourceManagerTexture());
	Locator<OptionManager>::provide(new OptionManager());
	Locator<DebugRenderer>::provide(new DebugRenderer());

	Locator<PassThroughRenderer>::provide(new PassThroughRenderer());
	Locator<BlockIDTextures>::provide(new BlockIDTextures());
	Locator<DebugRenderInfo>::provide(new DebugRenderInfo());
	Locator<ReferenceManager<Activity>>::provide(new ReferenceManager<Activity>());
	Locator<BlitRenderer>::provide(new BlitRenderer());
	Locator<Fonts>::provide(new Fonts());

	Locator<Log>::provide(new Log());
}
