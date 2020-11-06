#pragma once
#include "DJEUtils.h"
#include "SongScanner.h"
#include "TimerManager.h"

#include <array>
#include <atomic>
#include <iostream>
#include <portaudio.h>
#include <thread>
#include <vorbis/vorbisfile.h>

const size_t MAX_SIZE = 16384;

template <typename T>
class CircularBuffer {
public:
	void push(const T& value);
	T pop();
	size_t getLength() const;
	void clear();

private:
	std::array<T, MAX_SIZE> buffer;
	size_t writeIndex = 0;
	size_t readIndex = 0;
};

class Player;

enum AnimationID {
	AN_GREEN_IN,
	AN_GREEN_OUT,
	AN_BLUE_IN,
	AN_BLUE_OUT
};

class Audio {
public:
	Audio();
	void init();
	void play();
	void stop();
	void load(const SongEntry& entry);
	void pollState(double time, const Player* p);
	void resetEffects();
	void destroy();
	bool isPlaying() const;
	double getFileLength();

	CircularBuffer<float> redPCM;
	CircularBuffer<float> greenPCM;
	CircularBuffer<float> bluePCM;

	OggVorbis_File redFile;
	OggVorbis_File greenFile;
	OggVorbis_File blueFile;
	int streams = 0;

	int bitstream = 0;
	bool loaderThreadRunning = true;

	float greenGain = 1.0;
	float redGain = 1.0;
	float blueGain = 1.0;

	float greenPan = 0.0;
	float redPan = 0.0;
	float bluePan = 0.0;

private:
	int lastPlayerPos = 0;
	PaStream* audioStream;
	std::thread loader;
	bool playing = false;

	bool initialized = false;
	TimerManager animManager;
};
