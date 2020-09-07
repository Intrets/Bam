#include "common.h"

#include "Linker.h"
#include "Anchor.h"

bool Linker::mergeAnchors(GameState& gameState, WeakReference<Activity, Anchor> r1, WeakReference<Activity, Anchor> r2) {
	if (r2.handle == r1.handle) {
		return false;
	}

	for (auto& member : r1.get()->getRootPtr()->getTreeMembers()) {
		member->memberCache.invalidateMembers();
	}

	auto toDelete = r2;

	for (auto& member : r2.get()->getTreeMembers()) {
		member->memberCache.invalidateRoot();
	}

	for (auto& r : r2.get()->getChildren()) {
		r1.get()->addChild(std::move(r));
	}

	Locator<ReferenceManager<Activity>>::get()->deleteReference(&toDelete);
	return true;
}

bool Linker::linkSingleGrouper(GameState& gameState, WeakReference<Activity, SingleGrouper> r1, WeakReference<Activity, Activity> r2) {
	auto& refMan = Locator<ReferenceManager<Activity>>::ref();

	if (r1.isNull() || r2.isNull()) {
		return false;
	}

	r2 = r2.get()->getRootRef();

	if (r1.get()->hasChild()) {
		auto& child = r1.get()->getChild();
		if (child.get()->getType() != ACTIVITY::TYPE::ANCHOR) {
			auto childAnchor = refMan.makeUniqueRef<Anchor>();
			childAnchor.get()->fillTracesLocalForced(gameState);
			childAnchor.get()->addChild(r1.get()->popChild());

			auto childAnchorHandle = childAnchor.handle;

			r1.get()->addChild(std::move(childAnchor));
			r1.handle = childAnchorHandle;
		}
		else {
			r1 = child;
		}
		return linkAnchor(gameState, r1, r2);
	}
	else {
		for (auto& member : r1.get()->getRootPtr()->getTreeMembers()) {
			member->memberCache.invalidateMembers();
		}

		r1.get()->addChild(UniqueReference<Activity, Activity>(r2.handle));

		for (auto& member : r2.get()->getTreeMembers()) {
			member->memberCache.invalidateRoot();
		}

		return true;
	}
}

bool Linker::linkAnchor(GameState& gameState, WeakReference<Activity, Anchor> r1, WeakReference<Activity, Activity> r2) {
	if (r1.isNull() || r2.isNull()) {
		return false;
	}

	r2 = r2.get()->getRootRef();

	auto p1 = r1.get();
	auto p2 = r2.get();

	if (p2->getType() == ACTIVITY::TYPE::ANCHOR) {
		return mergeAnchors(gameState, r1, r2);
	}
	else {
		for (auto& member : r1.get()->getRootPtr()->getTreeMembers()) {
			member->memberCache.invalidateMembers();
		}

		p1->addChild(UniqueReference<Activity, Activity>(r2.handle));

		for (auto& member : r2.get()->getTreeMembers()) {
			member->memberCache.invalidateRoot();
		}
		return true;
	}
}


bool Linker::linkNonGrouper(GameState& gameState, WeakReference<Activity, Activity> r1, WeakReference<Activity, Activity> r2) {
#ifdef BAM_DEBUG
	auto type = r1.get()->getType();
	assert(
		type != ACTIVITY::TYPE::ANCHOR &&
		type != ACTIVITY::TYPE::RAILCRANE &&
		type != ACTIVITY::TYPE::PISTON);
#endif
	r2 = r2.get()->getRootRef();

	auto parent = r1.get()->parentRef;
	if (parent.isNotNull()) {
		if (parent.get()->getType() == ACTIVITY::TYPE::ANCHOR) {
			return linkAnchor(gameState, parent, r2);
		}
		else {
			return linkSingleGrouper(gameState, parent, r2);
		}
	}
	else {
		auto& refMan = Locator<ReferenceManager<Activity>>::ref();

		auto r1Anchor = refMan.makeRef<Anchor>();
		r1Anchor.get()->fillTracesLocalForced(gameState);
		r1Anchor.get()->addChild(UniqueReference<Activity, Activity>(r1.handle));

		return linkAnchor(gameState, r1Anchor, r2);
	}
}

bool Linker::link(GameState& gameState, WeakReference<Activity, Activity> r1, WeakReference<Activity, Activity> r2) {
	if (r1.isNull() || r2.isNull()) {
		return false;
	}
	auto p1 = r1.get();
	auto p2 = r2.get();

	if (p2->parentRef.isNotNull()) {
		return link(gameState, r1, p2->getRootRef());
	}

	switch (p1->getType()) {
		case ACTIVITY::TYPE::ANCHOR:
			return linkAnchor(gameState, r1, r2);
			break;
		case ACTIVITY::TYPE::PISTON:
		case ACTIVITY::TYPE::RAILCRANE:
			return linkSingleGrouper(gameState, r1, r2);
			break;
		default:
			return linkNonGrouper(gameState, r1, r2);
			break;
	}

	return false;
}


