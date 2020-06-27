#pragma once

#include "AL/al.h"
#include "AL/alc.h"
#include "vorbis/vorbisfile.h"

#include <array>
#include <iostream>
#include <vector>

class Audio {
public:
	Audio();
	~Audio();
	void play();
	void load(const char* filename);
	void stop() const;
	void buffer(double time);
	void reset();
	bool isActive(double time) const;
	bool isPlaying() const;

private:
	void setupBuffers() const;
	void removeBuffers() const;
	ALCdevice* m_device;
	ALCcontext* m_context;
	unsigned int m_source;
	unsigned int m_buffer;
	bool firstRun = true;
	OggVorbis_File m_oggFile;
	int m_frequency = 0;
	int m_currentSection = 0;
	double m_songLength = 0.0;
};
