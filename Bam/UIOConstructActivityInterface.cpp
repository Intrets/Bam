#include "common.h"

#include "UIOConstructActivityInterface.h"

#include "UIOList.h"
#include "UIOConstructer.h"

UIOConstructer<UIOList> ActivityInterfaceConstructer::constructActivityInteractor() {
	 auto outerList = UIOConstructer<UIOList>::makeConstructer(UIOList::DIR::DOWN);


	return std::move(outerList);
}
