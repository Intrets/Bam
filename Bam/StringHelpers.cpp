#include "common.h"

#include "StringHelpers.h"
#include <sstream>

void split(size_t n, std::string& in, std::vector<std::string>& out, char c, bool emptySegments) {
	if (n <= 0) {
		n = in.size() + 1;
	}
	size_t i = 0;
	while (i < in.size()) {
		while (i < in.size() && in[i] == c) {
			i++;
			if (emptySegments) {
				break;
			}
		}
		auto first = i;
		while (i < in.size() && in[i] != c) {
			i++;
		}
		auto second = i;
		if (out.size() == n) {
			out.push_back(in.substr(first, in.size() - 1));
			return;
		}
		else {
			out.push_back(in.substr(first, second - first));
		}
	}
}

std::string join(std::vector<std::string> const& in, std::optional<char> c) {
	if (in.size() == 0) {
		return "";
	}
	std::stringstream out;
	for (int32_t i = 0; i < in.size() - 1; i++) {
		out << in[i];
		if (c.has_value()) {
			out << c.value();
		}
	}
	out << in.back();

	return out.str();
}
