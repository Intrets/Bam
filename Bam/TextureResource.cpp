#include "common.h"
#include "TextureResource.h"

#include "ResourceManagerTexture.h"

TextureStore* TextureResource::get() {
	return Locator<ResourceManagerTexture>::get()->get(index);
}

TextureResource::TextureResource(std::string name) {
	index = Locator<ResourceManagerTexture>::get()->getIndex(name);
}
