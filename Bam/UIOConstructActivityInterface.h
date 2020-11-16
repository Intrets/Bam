#pragma once

template<class>
class UIOConstructer;

class UIOList;
class UIOActivityInterface;

namespace CONSTRUCTER
{
	UIOConstructer<UIOList> constructActivityInteractor(UIOActivityInterface*& ptr);
}
