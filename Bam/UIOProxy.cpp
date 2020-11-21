#include "common.h"

#include "UIOProxy.h"

UIOProxy::UIOProxy(Handle self) {
	this->selfHandle = self;
}

UniqueReference<UIOBase, UIOBase> UIOProxy::getMain() {
	return std::move(this->main);
}
