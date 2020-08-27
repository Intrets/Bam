#pragma once

#include "ReferenceManager.h"
#include "Activity.h"
#include "Piston.h"

class GameState;

namespace Linker
{
	// (SingleGrouper) -> (Activity)
	bool linkSingleGrouper(GameState& gameState, WeakReference<Activity, SingleGrouper> r1, WeakReference<Activity, Activity> r2);

	// (Anchor) -> (Activity)
	bool linkAnchor(GameState& gameState, WeakReference<Activity, Anchor> r1, WeakReference<Activity, Activity> r2);

	// (r1) -> (Activity) r1 has to be no SingleGrouper and no Anchor
	bool linkNonGrouper(GameState& gameState, WeakReference<Activity, Activity> r1, WeakReference<Activity, Activity> r2);

	// (Activity) -> (Activity) General purpose link 
	bool link(GameState& gameState, WeakReference<Activity, Activity> r1, WeakReference<Activity, Activity> r2);

	// (Anchor) -> (Anchor)
	bool mergeAnchors(GameState& gameState, WeakReference<Activity, Anchor> a1, WeakReference<Activity, Anchor> a2);
}
