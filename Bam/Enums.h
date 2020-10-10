#pragma once

namespace INVENTORYITEM
{
	enum class TYPE
	{
		BLOCK,
		ACTIVITY,
	};
}

namespace SHAPE
{
	enum TYPE
	{
		BLOCK,
		PISTON_HEAD,
		PISTON_BODY,
		SHAFT,
		CRANE_BASE_BLOCK,
		CRANE_HEAD_BLOCK,
		LUA,
		GRABBER,
		READER,
		DETECTOR,
		INCINERATOR,
		FORWARDER,
		_MAX,
	};
}

namespace PISTON
{
	enum class DIR
	{
		STATIONARY,
		RETRACT,
		EXTEND,
	};
}

namespace ACTIVITY
{
	enum class TYPE
	{
		ANCHOR,
		MOVER,
		PLATFORM,
		PISTON,
		SINGLEPLATFORM,
		BREAKER,
		GRABBER,
		PLANT,
		RAILCRANE,
		LUA,
		READER,
		DETECTOR,
		INCINERATOR,
		FORWARDER,
		_MAX,
	};

	const std::vector<std::string> TYPE_NAMES{
		"anchor",
		"mover",
		"platform",
		"piston",
		"single_platform",
		"breaker",
		"grabber",
		"plant",
		"railcrane",
		"lua",
		"reader",
		"detector",
		"incinerator",
		"forwarder",
		"_MAX_SHOULD_NOT_SEE_THIS",
	};

	enum DIR
	{
		RIGHT,
		DOWN,
		LEFT,
		UP,
		STATIONARY,
	};

	enum class ROT
	{
		CLOCKWISE,
		COUNTERCLOCKWISE,
	};

	inline ACTIVITY::DIR CLOCKWISEROT(ACTIVITY::DIR dir) {
		return static_cast<ACTIVITY::DIR>((static_cast<int32_t>(dir) + 1) % 4);
	}

	inline ACTIVITY::DIR CLOCKWISEROT_WITH_STATIONARY(ACTIVITY::DIR dir) {
		return static_cast<ACTIVITY::DIR>((static_cast<int32_t>(dir) + 1) % 5);
	}

	inline ACTIVITY::DIR COUNTERWISEROT(ACTIVITY::DIR dir) {
		return static_cast<ACTIVITY::DIR>((static_cast<int32_t>(dir) + 3) % 4);
	}

	inline ACTIVITY::DIR COUNTERWISEROT_WITH_STATIONARY(ACTIVITY::DIR dir) {
		return static_cast<ACTIVITY::DIR>((static_cast<int32_t>(dir) + 4) % 5);
	}

	inline ACTIVITY::DIR ROTATE(ACTIVITY::ROT rot, ACTIVITY::DIR dir) {
		if (rot == ACTIVITY::ROT::CLOCKWISE) {
			return CLOCKWISEROT(dir);
		}
		else {
			return COUNTERWISEROT(dir);
		}
	}

	inline ACTIVITY::DIR ROTATE_WITH_STATIONARY(ACTIVITY::ROT rot, ACTIVITY::DIR dir) {
		if (rot == ACTIVITY::ROT::CLOCKWISE) {
			return CLOCKWISEROT_WITH_STATIONARY(dir);
		}
		else {
			return COUNTERWISEROT_WITH_STATIONARY(dir);
		}
	}

	inline std::string GET_NAME(ACTIVITY::DIR dir) {
		switch (dir) {
			case ACTIVITY::DIR::UP:
				return "up";
				break;
			case ACTIVITY::DIR::RIGHT:
				return "right";
				break;
			case ACTIVITY::DIR::DOWN:
				return "down";
				break;
			case ACTIVITY::DIR::LEFT:
				return "left";
				break;
			case ACTIVITY::DIR::STATIONARY:
				return "stationary";
				break;
			default:
				assert(0);
				return "";
				break;
		}
	}

	inline glm::ivec2 GETDIRECTION(ACTIVITY::DIR dir) {
		switch (dir) {
			case ACTIVITY::DIR::UP:
				return glm::ivec2(0, 1);
				break;
			case ACTIVITY::DIR::RIGHT:
				return glm::ivec2(1, 0);
				break;
			case ACTIVITY::DIR::DOWN:
				return glm::ivec2(0, -1);
				break;
			case ACTIVITY::DIR::LEFT:
				return glm::ivec2(-1, 0);
				break;
			case ACTIVITY::DIR::STATIONARY:
				return glm::ivec2(0, 0);
				break;
			default:
				assert(0);
				return glm::ivec2(0, 0);
				break;
		}
	}

	inline ACTIVITY::DIR FLIP(ACTIVITY::DIR dir) {
		switch (dir) {
			case ACTIVITY::DIR::UP:
				return ACTIVITY::DIR::DOWN;
				break;
			case ACTIVITY::DIR::RIGHT:
				return ACTIVITY::DIR::LEFT;
				break;
			case ACTIVITY::DIR::DOWN:
				return ACTIVITY::DIR::UP;
				break;
			case ACTIVITY::DIR::LEFT:
				return ACTIVITY::DIR::RIGHT;
				break;
			case ACTIVITY::DIR::STATIONARY:
				return ACTIVITY::DIR::STATIONARY;
				break;
			default:
				assert(0);
				return ACTIVITY::DIR::STATIONARY;
				break;
		}
	}
}

namespace RAILCRANE
{
	enum class DIR
	{
		STATIONARY,
		LEFT,
		RIGHT,
	};
}

namespace ELEMENT
{
	enum TYPE
	{
		AIR,
		SILICONDIOXIDE,
		IRON,
		IRONOXIDE,
		CARBON,
		HYDROCARBON,
	};

	static std::unordered_map<std::string, ELEMENT::TYPE> typeMap = {
		{"air", AIR},
		{"silicondioxide", SILICONDIOXIDE},
		{"iron", IRON},
		{"ironoxide", IRONOXIDE},
		{"carbon", CARBON},
		{"hydrocarbon", HYDROCARBON},
	};

	ELEMENT::TYPE getType(std::string s);
	std::string getName(ELEMENT::TYPE type);
}

namespace GRABBER
{
	enum STATE
	{
		RELEASE,
		GRAB,
	};
}

