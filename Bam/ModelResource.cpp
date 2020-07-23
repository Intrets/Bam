#include "common.h"
#include "ModelResource.h"
#include "ResourceManagerModel.h"

ModelStore* ModelResource::get() {
	return Locator<ResourceManagerModel>::get()->get(this->index);
}

ModelResource::ModelResource(std::string name) {
	this->index = Locator<ResourceManagerModel>::get()->getIndex(name);
}
