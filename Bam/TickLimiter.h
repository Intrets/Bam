#pragma once


class TickLimiter
{
private:
	double lastTick;
	double tickTime;

public:
	TickLimiter();

	void advance();
	bool ready();
};

