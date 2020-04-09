#include "common.h"
#include "ModelResource.h"
#include "ResourceManagerModel.h"

ModelStore* ModelResource::get() {
	return Locator<ResourceManagerModel>::getService()->get(index);
}

ModelResource::ModelResource(std::string name) {
	index = Locator<ResourceManagerModel>::getService()->getIndex(name);
}
