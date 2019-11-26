/*
 * Author: Michael Buchel
 */
#ifndef AUDIO_BUFFER_H
#define AUDIO_BUFFER_H

#include <stdint.h>
#include <string>
#include <vector>
#include <portaudiocpp/PortAudioCpp.hxx>

typedef std::vector<short> VECTOR_SHORT;
typedef std::vector<short>::iterator VECTOR_SHORT_ITER;

class audio_buffer {
private:
    VECTOR_SHORT samples;
    VECTOR_SHORT_ITER playback_iter;

public:
    audio_buffer(int size_hint);

    int record_callback(const void *input,
                        void *output,
                        const uint64_t frames,
                        const PaStreamCallbackTimeInfo *time_info,
                        PaStreamCallbackFlags status);

    int playback_callback(const void *input,
                          void *output,
                          const uint64_t frames,
                          const PaStreamCallbackTimeInfo *time_info,
                          PaStreamCallbackFlags status);

    void clear();
    void write_to_file(const std::string &filename);
    void reset_playback();
};

#endif
