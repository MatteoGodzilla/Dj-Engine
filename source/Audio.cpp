#include "Audio.h"

#include "Player.h"

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

void checkError(PaError code) {
	if (code != paNoError) {
		std::cerr << "PortAudio Error:" << Pa_GetErrorText(code) << std::endl;
		Pa_Terminate();
		exit(1);
	}
}

void fillBuffer(Audio* audio) {
	size_t bufferLoad = 2048;
	while (audio->loaderThreadRunning) {
		size_t redSize = audio->redPCM.getLength();
		size_t greenSize = audio->greenPCM.getLength();
		size_t blueSize = audio->bluePCM.getLength();

		//size_t length = std::max(std::max(redSize,greenSize),blueSize);

		long redSamples = 0;
		long greenSamples = 0;
		long blueSamples = 0;

		if (redSize + bufferLoad < MAX_SIZE) {
			float** redPCM;
			redSamples = ov_read_float(&audio->redFile, &redPCM, bufferLoad, &audio->bitstream);

			if (redSamples > 0) {
				for (int i = 0; i < redSamples; ++i) {
					/*  audio layout
						channel = left:0, right:1
						pcm[channel][sample_index];
					*/
					float left = redPCM[0][i];
					float right = redPCM[1][i];

					audio->redPCM.push(left);
					audio->redPCM.push(right);
				}
			}
		}
		if (audio->streams == 3) {
			if (greenSize + bufferLoad < MAX_SIZE) {
				float** greenPCM;
				greenSamples = ov_read_float(&audio->greenFile, &greenPCM, bufferLoad, &audio->bitstream);

				if (greenSamples > 0) {
					for (int i = 0; i < greenSamples; ++i) {
						/*  audio layout
							channel = left:0, right:1
							pcm[channel][sample_index];
						*/
						float left = greenPCM[0][i];
						float right = greenPCM[1][i];

						audio->greenPCM.push(left);
						audio->greenPCM.push(right);
					}
				}
			}
			if (blueSize + bufferLoad < MAX_SIZE) {
				float** bluePCM;
				blueSamples = ov_read_float(&audio->blueFile, &bluePCM, bufferLoad, &audio->bitstream);

				if (blueSamples > 0) {
					for (int i = 0; i < blueSamples; ++i) {
						/*  audio layout
							channel = left:0, right:1
							pcm[channel][sample_index];
						*/

						float left = bluePCM[0][i];
						float right = bluePCM[1][i];
						audio->bluePCM.push(left);
						audio->bluePCM.push(right);
					}
				}
			}
		}

		//std::cout << "buffer:" << greenSamples << "\t" << redSamples << "\t" << blueSamples << std::endl;

		//std::cout << ov_time_tell(&(audio->redFile)) << "\t" << audio->getFileLength() << std::endl;

		if (ov_time_tell(&(audio->redFile)) >= audio->getFileLength()) {
			audio->stop();
		}

		//std::cout << (audio->writeIndex < audio->readindex ? audio->writeIndex + MAX_SIZE:audio->writeIndex) - audio->readindex << "\t" << audio->getLength()<< std::endl;
	}
	std::cout << "Audio Message: Loader thread stopped" << std::endl;
}

int PACallback(const void* input, void* output, unsigned long framecount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags flags, void* userdata) {
	auto* audio = (Audio*)userdata;
	auto* out = (float*)output;

	size_t redSize = audio->redPCM.getLength();
	size_t greenSize = audio->greenPCM.getLength();
	size_t blueSize = audio->bluePCM.getLength();

	//	std::cout << framecount << std::endl;

	size_t length = 0;
	if (audio->streams == 1) {
		length = redSize;
	} else if (audio->streams == 3) {
		length = std::min(std::min(redSize, greenSize), blueSize);
	}

	if (length > framecount * 2) {
		for (size_t i = 0; i < framecount; i++) {
			float rl = audio->redPCM.pop();
			float rr = audio->redPCM.pop();

			float gl = 0.0f;
			float gr = 0.0f;
			float bl = 0.0f;
			float br = 0.0f;

			if (audio->streams == 3) {
				gl = audio->greenPCM.pop();
				gr = audio->greenPCM.pop();
				bl = audio->bluePCM.pop();
				br = audio->bluePCM.pop();
			}

			rl *= (audio->redPan < 0.1f ? 1.0f : 1.0f - audio->redPan) * audio->redGain;
			rr *= (audio->redPan > -0.1f ? 1.0f : 1.0f + audio->redPan) * audio->redGain;
			gl *= (audio->greenPan < 0.1f ? 1.0f : 1.0f - audio->greenPan) * audio->greenGain;
			gr *= (audio->greenPan > -0.1f ? 1.0f : 1.0f + audio->greenPan) * audio->greenGain;
			bl *= (audio->bluePan < 0.1f ? 1.0f : 1.0f - audio->bluePan) * audio->blueGain;
			br *= (audio->bluePan > -0.1f ? 1.0f : 1.0f + audio->bluePan) * audio->blueGain;

			float left = rl + gl + bl;
			float right = rr + gr + br;

			*out = (left > -1.0f ? (left < 1.0f ? left : 1.0f) : -1.0f);
			out++;
			*out = (right > -1.0f ? (right < 1.0f ? right : 1.0f) : -1.0f);
			out++;
		}
	}
	//std::cout << framecount << std::endl;
	return 0;
}

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
		checkError(Pa_Initialize());
		redPCM.clear();
		greenPCM.clear();
		bluePCM.clear();

		initialized = true;
	}
}

void Audio::load(const SongEntry& entry) {
	if (entry.streams == 3) {
		std::string redPath = entry.path + std::string("/red.ogg");
		std::string greenPath = entry.path + std::string("/green.ogg");
		std::string bluePath = entry.path + std::string("/blue.ogg");
		ov_fopen(redPath.c_str(), &redFile);
		ov_fopen(greenPath.c_str(), &greenFile);
		ov_fopen(bluePath.c_str(), &blueFile);
		streams = 3;
	} else {
		std::string path = entry.path + std::string("/song.ogg");
		ov_fopen(path.c_str(), &redFile);
		streams = 1;
	}

	vorbis_info* info;
	info = ov_info(&redFile, -1);

	ov_time_seek(&redFile, 0.0);
	ov_time_seek(&greenFile, 0.0);
	ov_time_seek(&blueFile, 0.0);

	loaderThreadRunning = true;
	loader = std::thread(fillBuffer, this);

	checkError(Pa_OpenDefaultStream(&audioStream, 0, 2, paFloat32, info->rate, paFramesPerBufferUnspecified, PACallback, this));
}

void Audio::play() {
	if (!Pa_IsStreamActive(audioStream)) {
		checkError(Pa_StartStream(audioStream));
		playing = true;
	}
}

void Audio::stop() {
	if (Pa_IsStreamActive(audioStream)) {
		checkError(Pa_StopStream(audioStream));
		playing = false;
		loaderThreadRunning = false;
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
	return ov_time_total(&redFile, -1);
}

void Audio::destroy() {
	if (initialized) {
		if (playing) {
			stop();
		}
		checkError(Pa_CloseStream(audioStream));
		loader.join();
		ov_clear(&redFile);
		ov_clear(&greenFile);
		ov_clear(&blueFile);
		checkError(Pa_Terminate());
		initialized = false;
	}
	std::cout << "Audio Message: destroyed audio context" << std::endl;
}
