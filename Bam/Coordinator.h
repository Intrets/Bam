#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <mutex>

#include "NetworkAction.h"

class GameLoad;
class Loader;
class Saver;

namespace NETWORK
{
	class Message;
}

namespace COORDINATOR
{
	class Coordinator
	{
	public:
		std::mutex mutex;

		int32_t currentTick = 0;
		int32_t tickBufferSize = 10;

		std::optional<std::unique_ptr<GameLoad>> maybeLoadGame;

		std::string gameStateUuid;
		std::unordered_map<int32_t, PlayerActions> tickBuffer;

		void pushTick(int32_t tick, PlayerActions&& actions);

		void pushMessage(NETWORK::Message&& message);

		bool ready(int32_t tick);

		[[nodiscard]]
		PlayerActions pullTick();

		void reset(int32_t tick);
	};
}
