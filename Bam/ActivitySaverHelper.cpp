#include "common.h"

#include "ActivitySaverHelper.h"

#include "Saver.h"
#include "ReferenceManager.h"
#include "Activity.h"

void save(Saver& saver, ReferenceManager<Activity>& manager) {
	saver.store(manager.data.size());
	for (auto& p : manager.data) {
		p.second->save(saver);
	}
}
