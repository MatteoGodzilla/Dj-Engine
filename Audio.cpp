#include "Audio.h"

Audio::Audio() {
	m_device = alcOpenDevice(NULL);
	if (m_device != NULL) {
		m_context = alcCreateContext(m_device, NULL);
		if (m_context != NULL) {
			alcMakeContextCurrent(m_context);
			alGenSources(1, &m_source);
			alGenBuffers(1, &m_buffer);
			alListener3f(AL_POSITION, 0.0, 0.0, 0.0);
			alSource3f(m_source, AL_POSITION, 0.0, 0.0, 0.0);
		}
	}
}

void Audio::load(const char* filename) {
	if (ov_fopen(filename, &m_oggFile) == 0 && firstRun) {
		std::cout << "successfully loaded 'song.ogg'" << std::endl;
	}
	buffer();
}

void Audio::buffer() {
	unsigned int bufferId = 0;
	int processed = 0;
	unsigned int buffersRemoved;
	char bufferData[4096];
	int bytesRead = 0;
	vorbis_info* info;

	alGenBuffers(1, &bufferId);
	info = ov_info(&m_oggFile, -1);
	m_frequency = info->rate;
	bytesRead = ov_read(&m_oggFile, bufferData, 4096, 0, 2, 1, &m_currentSection);
	alBufferData(bufferId, AL_FORMAT_STEREO16, bufferData, bytesRead, m_frequency);
	alSourceQueueBuffers(m_source, 1, &bufferId);
	alGetSourcei(m_source, AL_BUFFERS_PROCESSED, &processed);
	alSourceUnqueueBuffers(m_source, processed, &buffersRemoved);
}

void Audio::play() {
	if (firstRun) {
		std::cout << "first run" << std::endl;
		alSourcePlay(m_source);
		firstRun = false;
	}
}

Audio::~Audio() {
	alSourceStop(m_source);
	ov_clear(&m_oggFile);
}