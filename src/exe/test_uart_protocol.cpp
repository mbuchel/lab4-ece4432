/*
 * Author: Michael Buchel
 */
#include <uart.h>
#include <errno.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    uart com;

    com.start(argv[1]);

    while (1) {
        std::cout << com.recv_msg() << "\n";
    }
}
