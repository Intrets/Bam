#pragma once
#include "Activity.h"
class GameState;
class Activity;
class Loader;
class Saver;

class ActivityPaceHandler
{
public:
	int size;
	int i = 0;

	std::vector<std::vector<WeakReference<Activity, Activity>>> activities;

	void finish(GameState& gameState);

	void cycle();
	void add(WeakReference<Activity, Activity> m, int duration);

	void save(Saver& saver);
	void load(Loader& loader);

	ActivityPaceHandler(int max);
	ActivityPaceHandler();
	~ActivityPaceHandler() = default;

	deleteDefaults(ActivityPaceHandler);
};

class MovementPaceHandler
{
public:
	int size;
	int i = 0;

	std::vector<std::vector<WeakReference<Activity, Activity>>> activities;

	void finish(GameState& gameState);

	void save(Saver& saver);
	void load(Loader& loader);

	void cycle();
	void add(WeakReference<Activity, Activity> m, int duration);

	MovementPaceHandler(int max);
	MovementPaceHandler();
	~MovementPaceHandler() = default;

	deleteDefaults(MovementPaceHandler);
};