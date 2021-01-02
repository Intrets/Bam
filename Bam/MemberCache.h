#pragma once

class Activity;

typedef int32_t Handle;

class MemberCache
{
private:
	Activity& self;

	bool validMembers = false;
	int32_t membersTick = 0;
	std::vector<Activity*> members;

	bool validSortedMembers = false;
	std::vector<Activity*> sortedMembers;

	bool validRoot = false;
	int32_t rootTick = 0;
	Activity* root;

public:
	void invalidateAll();
	void invalidateMembers();
	void invalidateRoot();

	bool isValidAll() const;
	bool isValidMembers() const;
	bool isValidRoot() const;

	int32_t getMembersTick() const;
	int32_t getRootTick() const;

	std::vector<Activity*> const& getMembers();
	std::vector<Activity*> const& getSortedMembers();
	Activity* getRoot();

	MemberCache() = delete;
	MemberCache(Activity& self_);
	~MemberCache() = default;

	NOCOPYMOVE(MemberCache);
};

