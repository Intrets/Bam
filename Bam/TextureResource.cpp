#include "common.h"
#include "TextureResource.h"

#include "ResourceManagerTexture.h"

TextureStore* TextureResource::get() {
	return Locator<ResourceManagerTexture>::get()->get(this->index);
}

TextureResource::TextureResource(std::string name) {
	this->index = Locator<ResourceManagerTexture>::get()->getIndex(name);
}
