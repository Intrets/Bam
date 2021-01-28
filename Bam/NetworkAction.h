#pragma once

#include "Enums.h"
#include "GameAction.h"

class Loader;
class Saver;
struct Client;
class GameState;

namespace COORDINATOR
{
	class Coordinator;
}

class NetworkAction
{
public:
	NETWORKACTION::TYPE type;

	void load(Loader& loader);
	void save(Saver& saver);

	virtual void loadDerived(Loader& loader) = 0;
	virtual void saveDerived(Saver& saver) = 0;

	NetworkAction() = default;
	virtual ~NetworkAction() = default;
};

class GameLoad : public NetworkAction
{
public:
	std::stringstream saveBuffer;

	void run(GameState& gameState, COORDINATOR::Coordinator& coordinator, std::optional<Client*> maybeClient);

	virtual void loadDerived(Loader& loader) override;
	virtual void saveDerived(Saver& saver) override;

	GameLoad();
	virtual ~GameLoad() = default;
};

class PlayerActions : public NetworkAction
{
public:
	std::string uuid;

	int32_t tick;
	std::vector<std::unique_ptr<GameAction>> operations;

	virtual void loadDerived(Loader& loader) override;
	virtual void saveDerived(Saver& saver) override;

	PlayerActions();
};
