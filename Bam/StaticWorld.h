#pragma once

#include "common.h"

#include <unordered_map>

#include "ReferenceManager.h"
#include "Activity.h"

typedef int Handle;

class StaticWorldChunk;
struct RenderInfo;
class Saver;
class Loader;

class StaticWorld
{
public:
	std::pair<int, WeakReference<Activity, Activity>> getBlock(glm::ivec2 pos);

	void appendStaticRenderInfo(RenderInfo& renderInfo);
	void leaveTrace(glm::ivec2 pos, Handle m);

	void removeTrace(glm::ivec2 pos);
	void removeTrace(glm::ivec2 pos, Handle m);

	bool load(Loader& loader);
	bool save(Saver& saver);

	//TODO: !
	StaticWorldChunk* getChunkByIndex(int i, int j);
	StaticWorldChunk* getChunkByCoords(glm::vec2 pos);
private:
	struct hash
	{
		std::size_t operator()(glm::ivec2 const& s) const noexcept {
			return s.x + (1 << 16) * s.y;
		}
	};

	std::unordered_map<glm::ivec2, std::unique_ptr<StaticWorldChunk>, hash> world;
};

