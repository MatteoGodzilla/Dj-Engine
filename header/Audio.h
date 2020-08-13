#pragma once

#include "SongScanner.h"

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

class Audio {
public:
	void init();
	void play();
	void stop();
	void load(const SongEntry& entry);
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

private:
	PaStream* audioStream;
	std::thread loader;
	bool playing = false;

	bool initialized = false;
};