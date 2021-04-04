// SPDX-FileCopyrightText: 2021 MatteoGodzilla
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Audio.h"

#include "Player.h"
/*
template <typename T>
void CircularBuffer<T>::push(const T& value) {
	buffer[writeIndex] = value;
	writeIndex++;
	if (writeIndex >= MAX_SIZE) {
		writeIndex = 0;
	}
}

template <typename T>
T CircularBuffer<T>::pop() {
	T ret = buffer[readIndex];
	readIndex++;
	if (readIndex >= MAX_SIZE) {
		readIndex = 0;
	}
	return ret;
}

template <typename T>
size_t CircularBuffer<T>::getLength() const {
	return (writeIndex < readIndex ? writeIndex + MAX_SIZE : writeIndex) - readIndex;
}

template <typename T>
void CircularBuffer<T>::clear() {
	writeIndex = 0;
	readIndex = 0;
}
*/

Audio::Audio() {
	Timer greenIn = Timer(AN_GREEN_IN, 0.002);
	Timer greenOut = Timer(AN_GREEN_OUT, 0.002);
	Timer blueIn = Timer(AN_BLUE_IN, 0.002);
	Timer blueOut = Timer(AN_BLUE_OUT, 0.002);

	animManager.pushTimer(greenIn);
	animManager.pushTimer(greenOut);
	animManager.pushTimer(blueIn);
	animManager.pushTimer(blueOut);
}

void Audio::init() {
	if (!initialized) {
		initialized = true;
	}
}

void Audio::load(const SongEntry& entry) {
	if (entry.streams == 3) {
		std::string redPath = entry.path + std::string("/red.ogg");
		std::string greenPath = entry.path + std::string("/green.ogg");
		std::string bluePath = entry.path + std::string("/blue.ogg");
		greenStream.openFromFile(greenPath);
		redStream.openFromFile(redPath);
		blueStream.openFromFile(bluePath);
		streams = 3;
	} else {
		std::string path = entry.path + std::string("/song.ogg");
		redStream.openFromFile(path);
		streams = 1;
	}
}

void Audio::play() {
	if(!playing){
		if(streams == 3){
			greenStream.play();
			blueStream.play();
		}
		redStream.play();
		playing = true;
	}
}

void Audio::stop() {
	if(playing){
		if(streams == 3){
			greenStream.stop();
			blueStream.stop();
		}
		redStream.stop();
		playing = false;
	}
}

void Audio::resetEffects() {
	greenGain = 1.0f;
	greenPan = 0.0f;
	redGain = 1.0f;
	redPan = 0.0f;
	blueGain = 1.0f;
	bluePan = 0.0f;
}

void Audio::pollState(double time, const Player* p) {
	animManager.tick(time);
	int playerCross = p->m_cross;
	if (p->m_insideFSCross) {
		if (playerCross != lastPlayerPos) {
			//There is a change
			if (lastPlayerPos == 0 && playerCross >= 1) {
				//moved either to center or to blue
				//aka fade in blue track
				animManager.triggerTimer(AN_BLUE_IN, time);
				animManager.disableTimer(AN_BLUE_OUT);
				//std::cout << "BLUE IN" << std::endl;
			} else if (lastPlayerPos == 1) {
				if (playerCross == 0) {
					//fade out blue track
					animManager.triggerTimer(AN_BLUE_OUT, time);
					animManager.disableTimer(AN_BLUE_IN);
					//std::cout << "BLUE OUT" << std::endl;
				} else if (playerCross == 2) {
					//fade out green track
					animManager.triggerTimer(AN_GREEN_OUT, time);
					animManager.disableTimer(AN_GREEN_IN);
					//std::cout << "GREEN OUT" << std::endl;
				}
			} else if (lastPlayerPos == 2 && playerCross <= 1) {
				//moved either to center or to green
				//aka fade in green track
				animManager.triggerTimer(AN_GREEN_IN, time);
				animManager.disableTimer(AN_GREEN_OUT);
				//std::cout << "GREEN IN" << std::endl;
			}
		}

		Timer greenIn = animManager.getAnimById(AN_GREEN_IN);
		Timer greenOut = animManager.getAnimById(AN_GREEN_OUT);
		Timer blueIn = animManager.getAnimById(AN_BLUE_IN);
		Timer blueOut = animManager.getAnimById(AN_BLUE_OUT);

		float position = 0;

		if (greenIn.isEnabled()) {
			position = (float)(1.0 - greenIn.getPercent());
		} else if (greenOut.isEnabled()) {
			position = (float)(0.0 + greenOut.getPercent());
		} else if (blueIn.isEnabled()) {
			position = (float)(-1.0 + blueIn.getPercent());
		} else if (blueOut.isEnabled()) {
			position = (float)(0.0 - blueOut.getPercent());
		} else {
			//get position from crossfade
			position = (float)playerCross - 1.0f;
		}

		if (streams == 3) {
			greenGain = (position < 0.1 ? 1.0f : 1.0f - position);
			blueGain = (position > -0.1 ? 1.0f : 1.0f + position);
		} else {
			redPan = position;
		}
	} else {
		resetEffects();
	}
	lastPlayerPos = playerCross;
}

bool Audio::isPlaying() const {
	return playing;
}

double Audio::getFileLength() {
	return (double) redStream.getDuration().asSeconds();
}

void Audio::destroy() {
	if (initialized) {
		if (playing) {
			stop();
		}
		initialized = false;
	}
	std::cout << "Audio Message: destroyed audio context" << std::endl;
}
