#pragma once
#include <AL/al.h>
#include <AL/alc.h>
#include <sndfile.h>
#include <iostream>
#include <vector>

class Sound {
public:

    std::vector<std::string> channelPaths = {
    "sounds/sound1.wav",
    "sounds/sound2.wav",
    "sounds/sound3.wav"
    };

    ALCdevice* audioDevice = nullptr;
    ALCcontext* audioContext = nullptr;

    ALuint channel1Buffer = 0;
    ALuint channel2Buffer = 0;
    ALuint channel3Buffer = 0;
    ALuint channel1Source = 0;
    ALuint channel2Source = 0;
    ALuint channel3Source = 0;

    ALuint LoadWav(const char* filename)
    {
        SF_INFO info{};
        SNDFILE* file = sf_open(filename, SFM_READ, &info);
        if (!file) {
            std::cerr << "Failed to load sound: " << filename << "\n";
            return 0;
        }

        std::vector<short> samples(info.frames * info.channels);
        sf_readf_short(file, samples.data(), info.frames);
        sf_close(file);

        ALuint buffer;
        alGenBuffers(1, &buffer);

        ALenum format =
            (info.channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;

        alBufferData(buffer,format,samples.data(),(ALsizei)(samples.size() * sizeof(short)),info.samplerate);

        return buffer;
    }
    void setMusToBuffer(ALuint& buffer, std::string path) { 
        ALuint buf = LoadWav(path.c_str());
        if (buf == 0) {
            std::cerr << "Cannot load " << path << "\n";
            return;
        }
        buffer = buf;
    }
    void setupBuffers() {
        audioDevice = alcOpenDevice(nullptr);
        if (!audioDevice) { std::cerr << "Cannot open device\n"; return; }

        audioContext = alcCreateContext(audioDevice, nullptr);
        if (!audioContext) { std::cerr << "Cannot create context\n"; return; }

        alcMakeContextCurrent(audioContext);


        setMusToBuffer(channel1Buffer, channelPaths[0]);
        setMusToBuffer(channel2Buffer, channelPaths[1]);
        setMusToBuffer(channel3Buffer, channelPaths[2]);
    }
    
    void createSources() {
        alGenSources(1, &channel1Source);
        alSourcei(channel1Source, AL_BUFFER, channel1Buffer);
        alSourcei(channel1Source, AL_LOOPING, AL_TRUE);
        alSourcef(channel1Source, AL_GAIN, 1.0f);

        alGenSources(1, &channel2Source);
        alSourcei(channel2Source, AL_BUFFER, channel2Buffer);
        alSourcef(channel2Source, AL_GAIN, 1.0f);

        alGenSources(1, &channel3Source);
        alSourcei(channel3Source, AL_BUFFER, channel3Buffer);
        alSourcef(channel3Source, AL_GAIN, 1.0f);
    }
};

