#pragma once

#include <array>
#include <atomic>
#include <iostream>
#include <portaudio.h>
#include <thread>
#include <vorbis/vorbisfile.h>

const size_t MAX_SIZE = 16384;

class Audio {
public:
	void init();
	void play();
	void stop();
	void load(const std::string& path);
	void destroy();
	bool isPlaying() const;

	void push(float value);
	float pop();
	size_t getLength() const;
	double getFileLength();

	std::array<float, MAX_SIZE> buffer;
	OggVorbis_File file;
	int bitstream = 0;
	bool loaderThreadRunning = true;

	size_t writeIndex = 0;
	size_t readIndex = 0;

private:
	PaStream* audioStream;
	std::thread loader;
	bool playing = false;

	bool initialized = false;
};