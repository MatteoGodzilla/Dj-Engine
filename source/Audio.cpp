#include "Audio.h"

Audio::Audio() {
	m_device = alcOpenDevice(nullptr);
	if (m_device != nullptr) {
		m_context = alcCreateContext(m_device, nullptr);
		if (m_context != nullptr) {
			//initialize audio
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
		//initial song load to kickstart the streaming

		unsigned int b = 0; //pointer never used
		vorbis_info* info;

		info = ov_info(&m_oggFile, -1);
		m_frequency = info->rate;
		m_songLength = ov_time_total(&m_oggFile, -1);

		setupBuffers();
		buffer(0.0);
	} else {
		std::cerr << "Audio Error: Cannot load 'song.ogg'" << std::endl;
	}
}

void Audio::buffer(double time) {
	unsigned int bufferRemoved = 0;
	int processed = 0;
	std::array<char, 4096> bufferData;
	int bytesRead = 0;

	if(time > ov_time_tell(&m_oggFile)){
		ov_time_seek_lap(&m_oggFile,time);
		removeBuffers();
		setupBuffers();
	}

	//get already processed buffers
	alGetSourcei(m_source, AL_BUFFERS_PROCESSED, &processed);

	//upload buffers until all are processed
	while (processed > 0) {
		alSourceUnqueueBuffers(m_source, 1, &bufferRemoved);

		//read and upload
		bytesRead = ov_read(&m_oggFile, bufferData.data(), 4096, 0, 2, 1, &m_currentSection);
		alBufferData(bufferRemoved, AL_FORMAT_STEREO16, bufferData.data(), bytesRead, m_frequency);
		alSourceQueueBuffers(m_source, 1, &bufferRemoved);

		processed--;
	}
}

void Audio::setupBuffers(){
	int buffers = 0;
	unsigned int b;
	alGetSourcei(m_source, AL_BUFFERS_QUEUED, &buffers);
	while (buffers < 50) {
		//temporary buffer
		alGenBuffers(1, &b);
		//upload
		alBufferData(b, AL_FORMAT_STEREO16, nullptr, 0, m_frequency);
		alSourceQueueBuffers(m_source, 1, &b);
		//remove temporary buffer
		//alDeleteBuffers(1, &b);
		alGetSourcei(m_source, AL_BUFFERS_QUEUED, &buffers);
	}
}

void Audio::removeBuffers(){
	int queue;
	unsigned int bufferPointer;
	alGetSourcei(m_source,AL_BUFFERS_PROCESSED,&queue);
	while(queue > 0){
		alSourceUnqueueBuffers(m_source,1,&bufferPointer);
		alDeleteBuffers(1,&bufferPointer);

		alGetSourcei(m_source,AL_BUFFERS_PROCESSED,&queue);
	}
}

void Audio::reset() {
	stop();
	removeBuffers();
	firstRun = true;
	m_frequency = 0;
	m_currentSection = 0;
	m_songLength = 0.0;

	//reset audio source
	alDeleteSources(1, &m_source);
	alGenSources(1, &m_source);
	alSource3f(m_source, AL_POSITION, 0.0, 0.0, 0.0);
}

void Audio::play() {
	int state;
	alGetSourcei(m_source,AL_SOURCE_STATE,&state);
	if(state == AL_STOPPED || firstRun){
		alSourcePlay(m_source);
		firstRun = false;
	}
}

bool Audio::isPlaying() {
	int result = 0;
	alGetSourcei(m_source, AL_SOURCE_STATE, &result);
	return result == AL_PLAYING;
}

void Audio::stop() {
	alSourceStop(m_source);
}

bool Audio::isActive(double time) {
	return time < m_songLength;
}

Audio::~Audio() {
	alSourceStop(m_source);
	ov_clear(&m_oggFile);
}