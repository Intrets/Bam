#pragma once

#include <sstream>
#include <optional>

#include "Coordinator.h"

class GameState;
struct Client;
class Loader;
class Saver;

namespace COORDINATOR
{
	class Coordinator;
}

class MetaOperation;
namespace METAOPERATION
{
	std::unique_ptr<MetaOperation> loadOperation(Loader& loader);
}

class MetaOperation
{
public:
	COORDINATOR::MESSAGE::TYPE type;

	virtual void run(GameState& gameState, COORDINATOR::Coordinator& coordinator, std::optional<Client*> maybeClient) = 0;
	void load(Loader& loader);
	void save(Saver& saver);

	virtual void loadDerived(Loader& loader) = 0;
	virtual void saveDerived(Saver& saver) = 0;

	MetaOperation() = default;
	virtual ~MetaOperation() = default;
};

class GameLoad : public MetaOperation
{
public:
	std::stringstream saveBuffer;

	virtual void run(GameState& gameState, COORDINATOR::Coordinator& coordinator, std::optional<Client*> maybeClient) override;

	virtual void loadDerived(Loader& loader) override;
	virtual void saveDerived(Saver& saver) override;

	GameLoad();

	virtual ~GameLoad() = default;
};
