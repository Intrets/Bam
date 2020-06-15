#include "common.h"
#include "ReferenceManager.h"

void ManagedReferenceBase::validate() {
	valid = true;
}

void ManagedReferenceBase::invalidate() {
	valid = false;
}

bool ManagedReferenceBase::isValid() const {
	return valid;
}
