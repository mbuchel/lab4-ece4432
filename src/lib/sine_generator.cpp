/*
 * Author: Michael Buchel
 */
#include <sine_generator.h>
#include <math.h>
#include <assert.h>

/*
 * sine_generator - Constructor for sine generators.
 * @table_size - Size of the table.
 */
sine_generator::sine_generator(int table_size)
{
    table.resize(table_size);

    for (int i = 0; i < table_size; ++i)
        table[i] = 0.125f * sin((i / table_size) * M_PI * 2);
}

/*
 * generate - Generate sin signal.
 * @input - Not used.
 * @output - Output array.
 * @frames - Number of frames.
 * @time_info - Not used.
 * @status - Status.
 */
int sine_generator::generate(const void *input,
                             void *output,
                             const uint64_t frames,
                             const PaStreamCallbackTimeInfo *time_info,
                             PaStreamCallbackFlags status)
{
    assert(output != NULL);

    float **out = static_cast<float**>(output);

    for (uint64_t i = 0; i < frames; ++i) {
        out[0][i] = table[left_phase];
        out[1][i] = table[right_phase];

        left_phase += 1;
        if (left_phase >= table.size())
            left_phase -= table.size();

        right_phase += 3;
        if (right_phase >= table.size())
            right_phase -= table.size();
    }

    return paContinue;
}
