#pragma once

typedef int Handle;

class ActivityIgnoringGroup
{
private:
	std::vector<Handle> sortedHandles;

public:
	void add(Handle h);
	void prepare();

	bool contains(Handle h);

	ActivityIgnoringGroup();
	~ActivityIgnoringGroup();
};
