#pragma once

constexpr uint32_t char_to_hex(const char c) {
	if (c >= '0' && c <= '9') {
		return c - '0';
	}
	else if (c >= 'A' && c <= 'F') {
		return c - 'A' + 10;
	}
	else if (c >= 'a' && c <= 'f') {
		return c - 'a' + 10;
	}
	else {
		assert(0);
		return 0;
	}
};

constexpr uint32_t operator"" _hex(const char* c, std::size_t size) {
	int32_t hex = 0;
	int32_t offset = 0;
	if (size == 7 || size == 6) {
		hex = 0xff;
	}
	else {
		hex = (char_to_hex(c[size - 2 - offset]) << 4) + char_to_hex(c[size - 1 - offset]);
		offset += 2;
	}

	for (int32_t i = 0; i < 6; i++) {
		hex += char_to_hex(c[size - i - 1 - offset]) << (4 * (i + 2));
	}
	return hex;
};

namespace COLORS
{

	glm::vec4 rgb(int32_t r, int32_t g, int32_t b);
	glm::vec4 rgba(int32_t r, int32_t g, int32_t b, int32_t a);
	glm::vec4 hex(std::string h);
	glm::vec4 hex(uint32_t h);

	static glm::vec4 CURSOR = rgb(191, 0, 0);
	static glm::vec4 RED = rgb(255, 153, 153);
	static glm::vec4 GREEN = rgb(153, 255, 153);

	glm::vec4 DARKEN(glm::vec4 color);

	static glm::vec4 TEXT = hex("#dec165"_hex);
	static glm::vec4 BACKGROUND = hex("#25120z"_hex);
	static glm::vec4 FOREGROUND = hex("#e55a1c"_hex);
	static glm::vec4 FOCUSSED = rgb(242, 242, 242);
}
