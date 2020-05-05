#pragma once

#include <string>
#include <vector>

class ModifyerBase;
class GameState;

class ModifyingInterface
{
public:
	virtual void modifyMember(GameState& gameState, std::string& name, std::vector<std::string>& value) = 0;
	virtual std::pair<bool, std::string> getMember(std::string& name) { return std::make_pair(false, ""); };
	
	std::string getMembers();
	virtual std::stringstream& getMembers(std::stringstream& out) = 0;

	std::string getSimpleInfo();
	virtual std::ostream& getSimpleInfo(std::ostream& out) = 0;

	virtual void fillModifyingMap(ModifyerBase& modifyer) {};
};
