#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <mutex>

#include "PlayerActions.h"

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
			PLAYER_ACTIONS,
		};
	}

	class Coordinator
	{
	public:
		std::mutex mutex;

		int32_t currentTick = 0;
		int32_t tickBufferSize = 10;

		std::unordered_map<int32_t, PlayerActions> tickBuffer;

		void pushTick(int32_t tick, PlayerActions&& actions);

		void pushMessage(NETWORK::Message&& message);

		bool ready(int32_t tick);

		[[nodiscard]]
		PlayerActions pullTick();

		void reset(int32_t tick);
	};
}

