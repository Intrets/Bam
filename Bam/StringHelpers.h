#pragma once

#include <optional>

// split string on spaces
// maximum number of n splits, n <= 0 -> no limit

void split(size_t n, std::string& in, std::vector<std::string>& out, char c = ' ', bool emptySegments = false);

std::string join(std::vector<std::string> const& in, std::optional<char> c = std::nullopt);

