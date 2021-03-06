#pragma once

#include <unordered_map>
#include <map>
#include <optional>

#include "Activity.h"
#include "ReferenceManager.h"

typedef int32_t Handle;

class StaticWorldChunk;
struct RenderInfo;
class Saver;
class Loader;
class ShapedBlock;
class WorldBlock;
class ActivityIgnoringGroup;

class StaticWorld
{
public:
	std::optional<WeakReference<Activity, Activity>> getActivity(glm::ivec2 pos);
	std::optional<WeakReference<Activity, Activity>> getActivity(glm::vec2 pos);

	bool isOccupied(glm::ivec2 pos, ActivityIgnoringGroup const& ignore);
	bool isOccupied(glm::ivec2 pos);

	void appendStaticRenderInfo(RenderInfo& renderInfo);
	void leaveTrace(glm::ivec2 pos, Activity* m);

	void removeTraceForced(glm::ivec2 pos);
	void removeTraceFilter(glm::ivec2 pos, Activity* m);

	bool load(Loader& loader);
	bool save(Saver& saver);

	//TODO: !
	StaticWorldChunk& getChunkByIndex(int32_t i, int32_t j);
	StaticWorldChunk& getChunkByCoords(glm::vec2 pos);

	WorldBlock& getBlockRef(glm::ivec2 pos);
	WorldBlock& getBlockRef(glm::vec2 pos);

	WorldBlock const& getBlockConstRef(glm::ivec2 pos);
	WorldBlock const& getBlockConstRef(glm::vec2 pos);

	void setBlockForce(ShapedBlock block, glm::ivec2 pos);
	void setBlockForce(ShapedBlock block, glm::vec2 pos);

	bool setBlock(glm::ivec2 pos, ShapedBlock block);
	bool setBlock(glm::vec2 pos, ShapedBlock block);

private:
	struct hash
	{
		std::size_t operator()(glm::ivec2 const& s) const noexcept {
			return static_cast<size_t>(s.x) + (1ULL << 16) * static_cast<size_t>(s.y);
		}
	};

	struct cmp
	{
		bool operator()(const glm::ivec2& a, const glm::ivec2& b) const {
			return (a[0] < b[0]) || (a[0] == b[0]) && (a[1] < b[1]);
		}
	};

	//std::unordered_map<glm::ivec2, std::unique_ptr<StaticWorldChunk>, hash> world;
	std::map<glm::ivec2, std::unique_ptr<StaticWorldChunk>, cmp> world;
};

