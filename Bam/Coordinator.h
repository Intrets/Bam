#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <mutex>

#include "PlayerActions.h"

class GameLoad;
class Loader;
class Saver;

namespace NETWORK
{
	struct Message;
}

namespace COORDINATOR
{
	namespace MESSAGE
	{
		enum class TYPE
		{
			PLAYER_ACTIONS = 123,
			GAME_LOAD,
		};
	}

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

inline std::ostream& operator<<(std::ostream& out, COORDINATOR::MESSAGE::TYPE const& type) {
	out << static_cast<int32_t>(type);
	return out;
}

inline std::istream& operator>>(std::istream& in, COORDINATOR::MESSAGE::TYPE& type) {
	int32_t t;
	in >> t;
	type = static_cast<COORDINATOR::MESSAGE::TYPE>(t);
	return in;
}

