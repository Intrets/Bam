#include "common.h"

#include "InventorySerializerHelper.h"
#include "Saver.h"
#include "Loader.h"
#include "ReferenceManager.h"
#include "InventoryItem.h"

#define LOAD(TYPE) TYPE* p = new TYPE();\
					p->load(loader);\
					manager.storeReference(p->selfHandle, p);

void INVENTORYSERIALIZER::save(Saver& saver, ReferenceManager<InventoryItem>& manager) {
	saver.store(manager.data.size());
	for (auto& p : manager.data) {
		saver.store(p.second.get()->getType());
		p.second->save(saver);
	}
}

void INVENTORYSERIALIZER::load(Loader& loader, ReferenceManager<InventoryItem>& manager) {
	size_t size;
	loader.retrieve(size);
	for (size_t i = 0; i < size; i++) {
		INVENTORYITEM::TYPE type;
		loader.retrieve(type);
		switch (type) {
			case INVENTORYITEM::TYPE::BLOCK:
				{
					LOAD(InventoryBlock);
					break;
				}
			case INVENTORYITEM::TYPE::ACTIVITY:
				{
					LOAD(InventoryActivity);
					break;
				}
			default:
				assert(0);
				break;
		}
	}
}
