#pragma once
#include <mutex>
#include <chrono>
class Timeline
{
	public:
		Timeline(Timeline* anchor, float tic);
		int getTime();
		void pause();
		void unpause();
		bool isPaused();
		void restart();
		float getTic();
		void setTic(float newTic);

	private:
		std::mutex m;
		int startTime;
		int elapsedPauseTime;
		int lastPausedTime;
		float tic;
		bool paused;
		Timeline *anchor;
		int getSysTime();
		/* A private method for getting current time without needing to lock the mutex, to avoid double locking */
		int currentTime();
};

