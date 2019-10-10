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
		std::cout << "Audio msg: successfully loaded 'song.ogg'" << std::endl;
	}
	buffer();
}

void Audio::buffer() {
	unsigned int bufferId = 0;
	unsigned int bufferRemoved = 0;
	int processed = 0;
	char bufferData[4096];
	int bytesRead = 0;
	vorbis_info* info;

	info = ov_info(&m_oggFile, -1);
	m_frequency = info->rate;

	alGetSourcei(m_source, AL_BUFFERS_PROCESSED, &processed);
	while (processed >= 0) {
		alGenBuffers(1, &bufferId);
		alSourceUnqueueBuffers(m_source, 1, &bufferRemoved);

		bytesRead = ov_read(&m_oggFile, bufferData, 4096, 0, 2, 1, &m_currentSection);
		alBufferData(bufferId, AL_FORMAT_STEREO16, bufferData, bytesRead, m_frequency);
		alSourceQueueBuffers(m_source, 1, &bufferId);

		alDeleteBuffers(1, &bufferId);
		processed--;
	}

}

void Audio::play() {
	if (firstRun) {
		alSourcePlay(m_source);
		firstRun = false;
	}
}

void Audio::stop() {
	alSourceStop(m_source);
}

Audio::~Audio() {
	alSourceStop(m_source);
	ov_clear(&m_oggFile);
}