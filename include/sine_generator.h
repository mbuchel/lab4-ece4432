/*
 * Author: Michael Buchel
 */
#ifndef SINE_GENERATOR_H
#define SINE_GENERATOR_H

#include <stdint.h>
#include <vector>
#include <portaudiocpp/PortAudioCpp.hxx>

class sine_generator {
private:
    std::vector<float> table;
    int left_phase;
    int right_phase;

public:
    sine_generator(int table_size);

    int generate(const void *input,
                 void *output,
                 const uint64_t frames,
                 const PaStreamCallbackTimeInfo *time_info,
                 PaStreamCallbackFlags status);
};

#endif
