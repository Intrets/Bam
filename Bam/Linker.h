#pragma once

#include "ReferenceManager.h"
#include "Activity.h"
#include "Piston.h"

class GameState;

namespace Linker
{
	/// <summary>
	/// (SingleGrouper) -> (Activity)
	/// </summary>
	/// <param name="gameState">- GameState</param>
	/// <param name="r1">- Links r2 into this</param>
	/// <param name="r2">- Gets linked into r1</param>
	/// <returns></returns>
	bool linkSingleGrouper(GameState& gameState, WeakReference<Activity, SingleGrouper> r1, WeakReference<Activity, Activity> r2);

	/// <summary>
	/// (Anchor) -> (Activity)
	/// </summary>
	/// <param name="gameState">- GameState</param>
	/// <param name="r1">- Links r2 into this</param>
	/// <param name="r2">- Gets linked into r1</param>
	/// <returns></returns>
	bool linkAnchor(GameState& gameState, WeakReference<Activity, Anchor> r1, WeakReference<Activity, Activity> r2);

	/// <summary>
	/// (r1) -> (Activity) r1 Grouper but not SingleGrouper or Anchor
	/// </summary>
	/// <param name="gameState">- GameState</param>
	/// <param name="r1">- Links r2 into this</param>
	/// <param name="r2">- Gets linked into r1</param>
	/// <returns></returns>
	bool linkNonGrouper(GameState& gameState, WeakReference<Activity, Activity> r1, WeakReference<Activity, Activity> r2);

	/// <summary>
	/// (Activity) -> (Activity) General purpose Link
	/// </summary>
	/// <param name="gameState">- GameState</param>
	/// <param name="r1"></param>
	/// <param name="r2"></param>
	/// <returns>True if link is succesful</returns>
	bool link(GameState& gameState, WeakReference<Activity, Activity> r1, WeakReference<Activity, Activity> r2);

	/// <summary>
	/// (Anchor) -> (Anchor)
	/// Merges a2 into a1
	/// </summary>
	/// <param name="gameState">- GameState</param>
	/// <param name="a1">- Links a2 into this</param>
	/// <param name="a2">- Gets linked into a1</param>
	/// <returns>True if link is succesful</returns>
	bool mergeAnchors(GameState& gameState, WeakReference<Activity, Anchor> a1, WeakReference<Activity, Anchor> a2);
}
