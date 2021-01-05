#pragma once

class Saver;
class Loader;
class InventoryItem;

template<class>
class ReferenceManager;

namespace INVENTORYSERIALIZER
{
	void save(Saver& saver, ReferenceManager<InventoryItem>& manager);
	void load(Loader& loader, ReferenceManager<InventoryItem>& manager);
}
