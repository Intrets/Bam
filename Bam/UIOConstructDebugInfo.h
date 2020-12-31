#pragma once

class UIOBase;
class UIOList;

template<class, class>
class WeakReference;

namespace UIO2
{
	WeakReference<UIOBase, UIOList> constructDebugInfo();
}
