#pragma once
class RenderLimiter
{
private:
	double lastRender;
	double renderTime;

public:
	RenderLimiter();

	void advance();
	bool ready();
};
