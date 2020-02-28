#pragma once

#include "openal-soft/include/AL/al.h"
#include "openal-soft/include/AL/alc.h"
#include "vorbis/include/vorbis/vorbisfile.h"
#include <iostream>
#include <vector>

class Audio
{
public:
	Audio();
	~Audio();
	void play();
	void load(const char* filename);
	void stop();
	void buffer();
	void reset();
	bool isActive(double time);
	bool isPlaying();
private:
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