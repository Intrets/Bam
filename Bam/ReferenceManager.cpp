#include "common.h"
#include "ReferenceManager.h"

void ManagedReferenceBase::validate() {
	this->valid = true;
}

void ManagedReferenceBase::invalidate() {
	this->valid = false;
}

bool ManagedReferenceBase::isValid() const {
	return this->valid;
}

Handle ManagedReferenceBase::getHandle() const {
	return this->handle;
}
