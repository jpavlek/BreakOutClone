#pragma once

class Timer
{
public:
	Timer();
	~Timer();
	static const int FPS;
	static const int DELAY_TIME;
	void markFrameStart();
	void markFrameEnd();
	void calculateFrameTime();
	void delayToFrameRate();
	void adjustToFrameRate();
	
private:
	unsigned int frameStart_;
	unsigned int frameEnd_;
	unsigned int frameTime_;
};
