#pragma once

typedef int32_t Handle;
class Activity;

class ActivityIgnoringGroup
{
private:
	std::vector<Activity*> const& sortedHandles;

public:
	bool contains(Activity* h) const;

	ActivityIgnoringGroup(std::vector<Activity*> const& handles);
	ActivityIgnoringGroup() = delete;
	~ActivityIgnoringGroup();
};
