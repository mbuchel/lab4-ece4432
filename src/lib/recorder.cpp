/*
 * Author: Michael Buchel
 */
#include <recorder.h>
#include <iostream>
#include <cmath>
#include <cassert>
#include <cstddef>

portaudio::AutoSystem a;
portaudio::System &sys = portaudio::System::instance();

recorder::recorder(int BEEP_SECONDS,
                   double SAMPLE_RATE,
                   int FRAMES_PER_BUFFER,
                   int TABLE_SIZE)
    : beep_seconds(BEEP_SECONDS),
      sample_rate(SAMPLE_RATE),
      frames_per_buffer(FRAMES_PER_BUFFER),
      table_size(TABLE_SIZE),
      input(-1),
      output(-1),
      buffer((int) SAMPLE_RATE * 60),
      sine(TABLE_SIZE)
{
    input = sys.defaultInputDevice().index();
    output = sys.defaultOutputDevice().index();
}

void recorder::beep()
{
    char wait;

    portaudio::DirectionSpecificStreamParameters
        out_beep(sys.deviceByIndex(output),
                 2,
                 portaudio::FLOAT32,
                 false,
                 sys.deviceByIndex(output).defaultLowOutputLatency(),
                 NULL);
    portaudio::StreamParameters
        params_beep(portaudio::DirectionSpecificStreamParameters::null(),
                    out_beep,
                    sample_rate,
                    frames_per_buffer,
                    paClipOff);

    portaudio::MemFunCallbackStream<sine_generator> stream_beep(params_beep,
                                                                sine,
                                                                &sine_generator::generate);

    std::cout << "Press enter to START recording after the beep.";
    std::cin.get(wait);

    stream_beep.start();
    sys.sleep(beep_seconds * 1000);
    stream_beep.stop();
    stream_beep.close();
}

void recorder::record()
{
    char wait;
   
    beep();

    portaudio::DirectionSpecificStreamParameters
        in_params(sys.deviceByIndex(input),
                  1,
                  portaudio::INT16,
                  false,
                  sys.deviceByIndex(input).defaultLowInputLatency(),
                  NULL);

    portaudio::StreamParameters
        params_record(in_params,
                      portaudio::DirectionSpecificStreamParameters::null(),
                      sample_rate,
                      frames_per_buffer,
                      paClipOff);

    portaudio::MemFunCallbackStream<audio_buffer>
        stream_record(params_record,
                      buffer,
                      &audio_buffer::record_callback);


    std::cout << "Press enter to STOP recording.";
    stream_record.start();
    std::cin.get(wait);
    stream_record.stop();
    stream_record.close();
}

void recorder::save_file(const std::string &name)
{
    buffer.reset_playback();
    buffer.write_to_file(name);
}

void recorder::play()
{
    portaudio::DirectionSpecificStreamParameters
        out_params(sys.deviceByIndex(output),
                   1,
                   portaudio::INT16,
                   false,
                   sys.deviceByIndex(output).defaultLowOutputLatency(),
                   NULL);

    portaudio::StreamParameters
        params(portaudio::DirectionSpecificStreamParameters::null(),
               out_params,
               sample_rate,
               frames_per_buffer,
               paClipOff);

    portaudio::MemFunCallbackStream<audio_buffer>
        stream_playback(params,
                        buffer,
                        &audio_buffer::playback_callback);

    buffer.reset_playback();
    stream_playback.start();
    while (stream_playback.isActive())
        sys.sleep(100);
    stream_playback.stop();
    stream_playback.close();
}

void recorder::close()
{
    sys.terminate();
}
