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
        com.recv_msg();

        if (com.recv_msg() == -1)
            continue;

        if (strcmp(com.recv_buff, "Arduino is ready, please input mode\n"))
            com.send_msg("M");
    }
}
