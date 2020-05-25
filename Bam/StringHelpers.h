#pragma once

#include "common.h"

// split string on spaces
// maximum number of n splits, n <= 0 -> no limit
inline void split(size_t n, std::string& in, std::vector<std::string>& out, char c = ' ', bool emptySegments = false) {
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
