#pragma once

class Saver;
class Loader;
class InventoryItem;

template<class>
class ReferenceManager;

namespace INVENTORYSERIALIZER
{
	bool save(Saver& saver, ReferenceManager<InventoryItem>& manager);
	bool load(Loader& loader, ReferenceManager<InventoryItem>& manager);
}
