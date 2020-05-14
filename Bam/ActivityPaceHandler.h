#pragma once
#include "Activity.h"
class GameState;
class Activity;
class Loader;
class Saver;

class ActivityPaceHandler
{
public:
	int32_t size;
	int32_t i = 0;

	std::vector<std::vector<WeakReference<Activity, Activity>>> activities;

	void finish(GameState& gameState);

	void cycle();
	void add(WeakReference<Activity, Activity> m, int32_t duration);

	void save(Saver& saver);
	void load(Loader& loader);

	ActivityPaceHandler(int32_t max);
	ActivityPaceHandler();
	~ActivityPaceHandler() = default;

	deleteDefaults(ActivityPaceHandler);
};

class MovementPaceHandler
{
public:
	int32_t size;
	int32_t i = 0;

	std::vector<std::vector<WeakReference<Activity, Activity>>> activities;

	void finish(GameState& gameState);

	void save(Saver& saver);
	void load(Loader& loader);

	void cycle();
	void add(WeakReference<Activity, Activity> m, int32_t duration);

	MovementPaceHandler(int32_t max);
	MovementPaceHandler();
	~MovementPaceHandler() = default;

	deleteDefaults(MovementPaceHandler);
};