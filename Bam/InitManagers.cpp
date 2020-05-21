#include "common.h"

#include "InitManagers.h"

#include "InitBinds.h"
#include "BindHandler.h"

#include "DebugRenderInfo.h"
#include "ResourceManagerModel.h"
#include "ResourceManagerTexture.h"
#include "Option.h"
#include "DebugRenderer.h"
#include "BlockIDTextures.h"
#include "Activity.h"
#include "UIOBase.h"

void initManagers() {
	//Locator<CommandHandler>::provide(new CommandHandler());
	//Locator<ReferenceManager<BaseWindow>>::provide(new ReferenceManager<BaseWindow>());
	//Locator<ScriptManager>::provide(new ScriptManager());

	Locator<ReferenceManager<UIOBase>>::provide(new ReferenceManager<UIOBase>());

	BindHandler* bindHandler = new BindHandler();
	initBinds(*bindHandler);
	Locator<BindHandler>::provide(bindHandler);

	Locator<PathManager>::provide(new PathManager());
	Locator<ResourceManagerModel>::provide(new ResourceManagerModel());
	Locator<ResourceManagerTexture>::provide(new ResourceManagerTexture());
	Locator<OptionManager>::provide(new OptionManager());
	Locator<DebugRenderer>::provide(new DebugRenderer());
	Locator<BlockIDTextures>::provide(new BlockIDTextures());
	Locator<DebugRenderInfo>::provide(new DebugRenderInfo());
	Locator<ReferenceManager<Activity>>::provide(new ReferenceManager<Activity>());
}
