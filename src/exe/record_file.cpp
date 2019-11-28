/*
 * Author: Michael Buchel
 */
#include <recorder.h>

int main()
{
    try {
        recorder rec = recorder();
        rec.record();
        //rec.play();
        rec.save_file("audio.raw");
        rec.close();
        system("sox -t raw -r 16000 -b 16 -c 1 -L -e signed-integer audio.raw audio.wav");
    } catch (...) {

    }
}
