#include "common.h"
#include "TextureResource.h"

#include "ResourceManagerTexture.h"

TextureStore* TextureResource::get() {
	return Locator<ResourceManagerTexture>::getService()->get(index);
}

TextureResource::TextureResource(std::string name) {
	index = Locator<ResourceManagerTexture>::getService()->getIndex(name);
}
