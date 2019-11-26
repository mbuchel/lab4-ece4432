/*
 * Author: Michael Buchel
 */
#ifndef RECORDER_H
#define RECORDER_H

#include <string>
#include <portaudiocpp/PortAudioCpp.hxx>
#include <sine_generator.h>
#include <audio_buffer.h>

class recorder {
private:
    int beep_seconds;
    double sample_rate;
    int frames_per_buffer;
    int table_size;
    int input;
    int output;
    audio_buffer buffer;
    sine_generator sine;

    void beep();

public:
    recorder(int BEEP_SECONDS = 1,
             double SAMPLE_RATE = 16000.0,
             int FRAMES_PER_BUFFER = 64,
             int TABLE_SIZE = 200);

    void record();
    void save_file(const std::string &name);
    void play();
    void close();
};

#endif
