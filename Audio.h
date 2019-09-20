#pragma once

#include "AL/al.h"
#include "AL/alc.h"
#include "vorbis/vorbisfile.h"
#include <iostream>
#include <vector>

class Audio
{
public:
	Audio();
	~Audio();
	void play();
	void load(const char* filename);
	void buffer();
private:
	ALCdevice* m_device;
	ALCcontext* m_context;
	unsigned int m_source;
	unsigned int m_buffer;
	bool firstRun = true;
	std::vector<unsigned int> m_buffersId;
	OggVorbis_File m_oggFile;
	int m_frequency = 0;
	int m_currentSection = 0;
};

