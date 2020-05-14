#pragma once

typedef int32_t Handle;
class Activity;

class ActivityIgnoringGroup
{
private:
	std::vector<Handle> sortedHandles;

public:
	void add(Handle h);
	void prepare();

	bool contains(Handle h);

	ActivityIgnoringGroup(std::vector<Activity*>& members);
	ActivityIgnoringGroup(std::vector<Activity*>& members, std::vector<Activity*>& members2);
	ActivityIgnoringGroup();
	~ActivityIgnoringGroup();
};
