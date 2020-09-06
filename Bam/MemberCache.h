#pragma once

class Activity;

typedef int32_t Handle;

class MemberCache
{
private:
	Activity& self;

	bool validMembers = false;
	std::vector<Activity*> members;

	bool validSortedHandles = false;
	std::vector<Handle> sortedHandles;

	bool validRoot = false;
	Activity* root;

public:
	void invalidateAll();
	void invalidateMembers();
	void invalidateRoot();

	std::vector<Activity*> const& getMembers();
	std::vector<Handle> const& getSortedHandles();
	Activity* getRoot();

	MemberCache() = delete;
	MemberCache(Activity& self_);
	~MemberCache() = default;

	NOCOPYMOVE(MemberCache);
};

