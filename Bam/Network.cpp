#include "common.h"

#include "Network.h"
#include "ReferenceManager.h"
#include "Activity.h"
#include "LUAActivity.h"
#include "GameState.h"

void Operation::run(GameState& gameState) {
	ReadingBuffer readingBuffer = { this->args.size, 0, this->args.danger };

	switch (this->type) {
		case NWT::TYPE::LUAACTIVITY:
			{
				switch (static_cast<NWT::LUAACTIVITY::METHOD>(this->method)) {
					case NWT::LUAACTIVITY::METHOD::SETSCRIPT:
						{
							Handle handle = readingBuffer.nextInt32();
							auto activity = WeakReference<Activity, LuaActivity>(gameState.getActivityManager(), handle);
							activity.get()->setScript("", gameState);
							break;
						}
					default:
						break;
				}
				break;
			}
		default:
			break;
	}
}

int32_t ReadingBuffer::nextInt32() {
	assert(this->size >= this->offset + sizeof(int32_t));
	this->offset += sizeof(int32_t);
	auto ptr = reinterpret_cast<int32_t*>(this->danger);
	danger = ptr + 1;
	return *ptr;
}

std::string ReadingBuffer::nextString() {
	int32_t stringSize = this->nextInt32();
	assert(this->size >= this->offset + stringSize);
	this->offset += stringSize;
	auto ptr = reinterpret_cast<char*>(this->danger);
	danger = ptr + stringSize;

	return std::string(ptr, stringSize);
}
