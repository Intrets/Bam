#include "common.h"

#include "Linker.h"
#include "Anchor.h"
//#include "Piston.h"

// merge second group into first group
std::string Linker::linkAnchors(GameState& gameState, WeakReference<Activity, Anchor> r1, WeakReference<Activity, Anchor> r2) {
	if (r2.handle == r1.handle) {
		return "Same Groups";
	}
	//auto t1 = r1.get();
	//auto t2 = r2.get();
	auto toDelete = r2;
	for (auto& r : r2.get()->children) {
		r1.get()->children.push_back(r);
		r.get()->parentRef = r1;
	}
	Locator<ReferenceManager<Activity>>::getService()->deleteReference(&toDelete);
	return "Successfully linked";
}


std::string Linker::linkPiston(GameState & gameState, WeakReference<Activity, Piston> r1, WeakReference<Activity, Activity> r2) {
	if (!r1 || !r2) {
		return "Invalid references";
	}
	auto a11 = r1.get();
	auto a2 = r2.get();
	WeakReference<Activity, Anchor> target;
	if (a2->getType() == ACTIVITY::ANCHOR) {
		target = r2;
	}
	else if (a2->parentRef) {
		target = a2->parentRef;
	}
	else {
		target = Locator<ReferenceManager<Activity>>::getService()->makeRef<Anchor>();
		target.get()->addChild(a2->selfHandle);
		a2->parentRef = target;
	}

	WeakReference<Activity, Anchor> head;
	if (a11->child) {
		head = a11->child;
	}
	else {
		head = Locator<ReferenceManager<Activity>>::getService()->makeRef<Anchor>();
		head.get()->parentRef = r1;
		a11->addChild(head);
	}
	return link(gameState, head, target);
}

// from r1 to r2, or grouped together with Anchor
std::string Linker::link(GameState& gameState, WeakReference<Activity, Activity> r1, WeakReference<Activity, Activity> r2) {
	if (!r1 || !r2) {
		return "Invalid references";
	}
	auto a1 = r1.get();
	auto a2 = r2.get();
	// TODO: stop cycles from breaking everything
	WeakReference<Activity, Anchor> g1;
	WeakReference<Activity, Anchor> g2;
	if (a1->getType() == ACTIVITY::ANCHOR) {
		g1 = r1;
	}
	else if (!a1->parentRef) {
		g1 = Locator<ReferenceManager<Activity>>::getService()->makeRef<Anchor>();
		g1.get()->addChild(a1->selfHandle);
		a1->parentRef = g1;
	}
	else {
		g1 = a1->parentRef;
	}
	if (a2->getType() == ACTIVITY::ANCHOR) {
		g2 = r2;
	}
	else if (!a2->parentRef) {
		g2 = Locator<ReferenceManager<Activity>>::getService()->makeRef<Anchor>();
		g2.get()->addChild(a2->selfHandle);
		a2->parentRef = g2;
	}
	else {
		g2 = a2->parentRef;
	}

	if (g1.get()->parentRef && g2.get()->parentRef) {
		return "two groups already with parents";
	}
	if (g1.get()->parentRef) {
		linkAnchors(gameState, g1, g2);
	}
	else {
		linkAnchors(gameState, g2, g1);
	}

	return "Successfully linked";
}

