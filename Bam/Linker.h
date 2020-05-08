#pragma once

#include "ReferenceManager.h"
#include "Activity.h"
#include "Piston.h"

class GameState;

namespace Linker
{
	std::string linkPiston(GameState& gameState, WeakReference<Activity, Piston> r1, WeakReference<Activity, Activity> r2);
	std::string link(GameState& gameState, WeakReference<Activity, Activity> r1, WeakReference<Activity, Activity> r2);
	std::string linkAnchors(GameState& gameState, WeakReference<Activity, Anchor> a1, WeakReference<Activity, Anchor> a2);
}
