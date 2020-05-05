#include "common.h"
#include "ReferenceManager.h"

void _ManagedReferenceBase::validate() {
	valid = true;
}

void _ManagedReferenceBase::invalidate() {
	valid = false;
}

bool _ManagedReferenceBase::isValid() {
	return valid;
}
