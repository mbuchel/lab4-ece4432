/*
 * Author: Michael Buchel
 */
#include <audio_buffer.h>
#include <iostream>
#include <fstream>

audio_buffer::audio_buffer(int size_hint)
{
    if (size_hint > 0)
        samples.reserve(size_hint);

    playback_iter = samples.begin();
}

int audio_buffer::record_callback(const void *input,
                                  void *output,
                                  const uint64_t frames,
                                  const PaStreamCallbackTimeInfo *time_info,
                                  PaStreamCallbackFlags status)
{
    short **data = (short**) input;

    if (input == NULL) {
        std::cout << "audio_buffer::record_callback, input buffer was NULL";
        std::cout << "\n";
        return paContinue;
    }

    for (uint64_t i = 0; i < frames; ++i)
        samples.push_back(data[0][i]);

    return paContinue;
}

int audio_buffer::playback_callback(const void *input,
                                    void *output,
                                    const uint64_t frames,
                                    const PaStreamCallbackTimeInfo *time_info,
                                    PaStreamCallbackFlags status)
{
    short **data = (short**) output;
    uint64_t index_output = 0;

    if (output == NULL) {
        std::cout << "audio_buffer::playback_callback was NULL!\n";
        return paComplete;
    }

    while (index_output < frames) {
        if (playback_iter == samples.end()) {
            while (index_output < frames)
                data[0][index_output++] = (short) 0;

            return paComplete;
        }

        data[0][index_output++] = (short) *(playback_iter++);
    }

    return paContinue;
}

void audio_buffer::clear()
{
    samples.clear();
}

void audio_buffer::write_to_file(const std::string &filename)
{
    std::fstream fout(filename.c_str(), std::ios::out | std::ios::binary);
    short index_sample;
    for (VECTOR_SHORT_ITER i = samples.begin(); i != samples.end(); ++i) {
        index_sample = (short) *i;
        fout.write((char*) &index_sample, sizeof(short));
    }
    fout.close();
}

void audio_buffer::reset_playback()
{
    playback_iter = samples.begin();
}
