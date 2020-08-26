#pragma once

template<class>
class UIOConstructer;

class UIOList;
class UIOActivityInterface;

namespace Constructer 
{
	UIOConstructer<UIOList> constructActivityInteractor(UIOActivityInterface*& ptr);
}
