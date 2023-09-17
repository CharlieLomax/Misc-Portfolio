#include "Timeline.h"

Timeline::Timeline(Timeline* anchor, float tic) {
	this->tic = tic;
	this->anchor = anchor;
	elapsedPauseTime = 0;
	paused = false;

	if (anchor == NULL) {
		startTime = getSysTime();
	}
	else {
		startTime = anchor->currentTime();
	}
}

int Timeline::getTime() {
	std::unique_lock<std::mutex> lock(m);
	return currentTime();
}

void Timeline::pause() {
	std::unique_lock<std::mutex> lock(m);
	if (!paused) {
		lastPausedTime = currentTime();
		paused = true;
	}
}

void Timeline::unpause() {
	std::unique_lock<std::mutex> lock(m);
	if (paused) {

		int currentTime;
		if (anchor == NULL)
			currentTime = getSysTime();
		else
			currentTime = anchor->getTime();
		currentTime = ((currentTime - startTime) / tic) - elapsedPauseTime;

		elapsedPauseTime += currentTime - lastPausedTime;
		paused = false;
	}
}

bool Timeline::isPaused() {
	std::unique_lock<std::mutex> lock(m);
	return paused;
}

void Timeline::restart()
{
	std::unique_lock<std::mutex> lock(m);
	if (anchor == NULL) {
		startTime = getSysTime();
	}
	else {
		startTime = anchor->currentTime();
	}
}

float Timeline::getTic()
{
	std::unique_lock<std::mutex> lock(m);
	return tic;
}

void Timeline::setTic(float newTic)
{
	std::unique_lock<std::mutex> lock(m);
	tic = newTic;
}

int Timeline::getSysTime() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

int Timeline::currentTime()
{
	if (paused) {
		return lastPausedTime;
	}

	int currentTime;

	if (anchor == NULL)
		currentTime = getSysTime();
	else
		currentTime = anchor->getTime();

	return ((currentTime - startTime) / tic) - elapsedPauseTime;
}
