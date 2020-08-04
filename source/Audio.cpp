#include "Audio.h"

void checkError(PaError code){
	if(code != paNoError){
		std::cerr << "PortAudio Error:" << Pa_GetErrorText(code) << std::endl;
		Pa_Terminate();
		exit(1);
	}
}

void fillBuffer(Audio* audio){
	size_t bufferLoad = 2048;
	while(audio->loaderThreadRunning){
		if(audio->getLength() + bufferLoad < MAX_SIZE){
			float **pcm;
			int samples = ov_read_float(&audio->file,&pcm,bufferLoad,&audio->bitstream);
			if(samples == 0) {
				audio->stop();
			} else if (samples > 0){
				for(int i = 0; i < samples; ++i){
					/*  audio layout
						channel = left:0, right:1
						pcm[channel][sample_index];
					*/
					audio->push(pcm[0][i]);
					audio->push(pcm[1][i]);
				}
			}
			//std::cout << (audio->writeIndex < audio->readindex ? audio->writeIndex + MAX_SIZE:audio->writeIndex) - audio->readindex << "\t" << audio->getLength()<< std::endl;
		}
	}
	std::cout << "Audio Message: Loader thread stopped" << std::endl;
}

int PACallback(const void* input, void *output,unsigned long framecount,const PaStreamCallbackTimeInfo* timeInfo,PaStreamCallbackFlags flags, void* userdata){
	Audio* obj = (Audio*)userdata;
	float* out = (float*) output;

	for(size_t i=0; i<framecount; i++ )
	{
		float first = 0.0f;
		float second = 0.0f;
		if(obj->getLength() > 2){
			first = obj->pop();
			second = obj->pop();
		}
		*out = first;
		out++;
		*out = second;
		out++;
	}
	//std::cout << framecount << std::endl;
	return 0;
}

void Audio::init(){
	if(!initialized){
		checkError(Pa_Initialize());
		buffer.fill(0.0f);
		initialized = true;
	}
}

void Audio::load(std::string path){
	ov_fopen(path.c_str(),&file);

	vorbis_info *info;
	info = ov_info(&file,-1);

	ov_time_seek(&file,0.0);

	loaderThreadRunning = true;
	loader = std::thread(fillBuffer,this);

	checkError(Pa_OpenDefaultStream(&audioStream,0,2,paFloat32,info->rate,paFramesPerBufferUnspecified,PACallback,this));
}

void Audio::play(){
	if(!Pa_IsStreamActive(audioStream)){
		checkError(Pa_StartStream(audioStream));
		playing = true;
	}
}

void Audio::stop(){
	if(Pa_IsStreamActive(audioStream)){
		checkError(Pa_StopStream(audioStream));
		playing = false;
		loaderThreadRunning = false;
	}
}

bool Audio::isPlaying(){
	return playing;
}

void Audio::push(float value){
	buffer[writeIndex] = value;
	writeIndex++;
	if(writeIndex >= MAX_SIZE) writeIndex = 0;
}

float Audio::pop(){
	float ret = buffer[readIndex];
	readIndex++;
	if(readIndex >= MAX_SIZE) readIndex = 0;
	return ret;
}

size_t Audio::getLength(){
	return (writeIndex < readIndex ? writeIndex + MAX_SIZE:writeIndex) - readIndex;
}

double Audio::getFileLength(){
	return ov_time_total(&file,-1);
}

void Audio::destroy(){
	if(initialized){
		if(playing) stop();
		checkError(Pa_CloseStream(audioStream));
		loader.join();
		ov_clear(&file);
		checkError(Pa_Terminate());
		initialized = false;
	}
}