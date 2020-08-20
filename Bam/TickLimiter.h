#pragma once
class TickLimiter
{
private:
	double tickTime;
	double lastTick;

public:
	bool ready();
	void advance();
};

