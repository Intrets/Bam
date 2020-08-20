#pragma once

constexpr uint64_t operator"" _ms(uint64_t ms) {
	return ms * 1000;
}

constexpr uint64_t operator"" _s(uint64_t s) {
	return s * 1000000;
}

constexpr uint64_t operator"" _mus(uint64_t mus) {
	return mus;
}


#ifdef WIN32
#include <Windows.h>

// Sleep for time given in microseconds
// truncated to miliseconds but at minimum 1 milisecond
void sleep(int64_t microseconds) {
	int64_t miliseconds = microseconds / 1000;
	if (miliseconds == 0) {
		miliseconds = 1;
	}
	Sleep(miliseconds);
}

#elif UNIX
void sleep(int64_t microseconds) {
	static_assert(0);
}
#endif