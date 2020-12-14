#include "common.h"

#include "Linker.h"
#include "Anchor.h"

bool Linker::mergeAnchors(GameState& gameState, WeakReference<Activity, Anchor> r1, WeakReference<Activity, Anchor> r2) {
	if (r1.isNull() || r2.isNull()) {
		return false;
	}

	if (r2 == r1) {
		return true;
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

	Locator<ReferenceManager<Activity>>::get()->deleteReference(toDelete);
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

			WeakReference<Activity, Anchor> ref = childAnchor;

			r1.get()->addChild(std::move(childAnchor));

			if (linkAnchor(gameState, ref, r2)) {
				return true;
			}
			else {
				auto r1child = childAnchor.get()->popChild();

				r1.get()->popChild();
				r1.get()->addChild(std::move(r1child));

				return false;
			}
		}
		else {
			return linkAnchor(gameState, child.as<Anchor>(), r2);
		}
	}
	else {
		for (auto& member : r1.get()->getRootPtr()->getTreeMembers()) {
			member->memberCache.invalidateMembers();
		}

		r1.get()->addChild(UniqueReference<Activity, Activity>(r2));

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

	if (r2.get()->getType() == ACTIVITY::TYPE::ANCHOR) {
		return mergeAnchors(gameState, r1, r2.as<Anchor>());
	}
	else {
		for (auto& member : r1.get()->getRootPtr()->getTreeMembers()) {
			member->memberCache.invalidateMembers();
		}

		r1.get()->addChild(UniqueReference<Activity, Activity>(r2));

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
		switch (parent.get()->getType()) {
			case ACTIVITY::TYPE::ANCHOR:
				{
					return linkAnchor(gameState, parent.as<Anchor>(), r2);
					break;
				}
			case ACTIVITY::TYPE::RAILCRANE:
			case ACTIVITY::TYPE::PISTON:
				{
					return linkSingleGrouper(gameState, parent.as<SingleGrouper>(), r2);
					break;
				}
			default:
				{
					assert(0);
					return false;
					break;
				}
		}
	}
	else {
		auto& refMan = Locator<ReferenceManager<Activity>>::ref();

		r1.get()->memberCache.invalidateAll();

		auto r1Anchor = refMan.makeRef<Anchor>();
		r1Anchor.get()->fillTracesLocalForced(gameState);
		r1Anchor.get()->addChild(UniqueReference<Activity, Activity>(r1));

		if (linkAnchor(gameState, r1Anchor, r2)) {
			return true;
		}
		else {
			auto child = r1Anchor.get()->popChild();
			child.clear();

			assert(!r1Anchor.get()->hasChild());

			return false;
		}
	}
}

bool Linker::link(GameState& gameState, WeakReference<Activity, Activity> r1, WeakReference<Activity, Activity> r2) {
	if (r1.isNull() || r2.isNull()) {
		return false;
	}

	if (r2.get()->parentRef.isNotNull()) {
		return link(gameState, r1, r2.get()->getRootRef());
	}

	switch (r1.get()->getType()) {
		case ACTIVITY::TYPE::ANCHOR:
			return linkAnchor(gameState, r1.as<Anchor>(), r2);
			break;
		case ACTIVITY::TYPE::PISTON:
		case ACTIVITY::TYPE::RAILCRANE:
			return linkSingleGrouper(gameState, r1.as<SingleGrouper>(), r2);
			break;
		default:
			return linkNonGrouper(gameState, r1, r2);
			break;
	}
}


