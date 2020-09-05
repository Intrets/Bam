#pragma once

typedef int32_t Handle;
class Activity;

class ActivityIgnoringGroup
{
private:
	std::vector<Handle> const& sortedHandles;

public:
	bool contains(Handle h) const;

	ActivityIgnoringGroup(std::vector<Handle> const& handles);
	ActivityIgnoringGroup() = delete;
	~ActivityIgnoringGroup();
};
