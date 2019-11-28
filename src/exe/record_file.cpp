/*
 * Author: Michael Buchel
 */
#include <recorder.h>

int main()
{
    try {
        recorder rec = recorder();
        rec.record();
        rec.play();
        rec.close();
    } catch (...) {

    }
}
