/*
 * Author: Michael Buchel
 */
#include <uart.h>
#include <errno.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

const std::string setup = "Please enter operation mode\n";
const std::string master = "You selected operation mode: M\n";

int main(int argc, char **argv)
{
    uart com;

    com.start(argv[1]);

    while (1) {
        com.recv_msg();

        if (com.recv_msg() == -1)
            continue;

        if (strcmp(com.recv_buff, setup.c_str()) == 0)
            com.send_msg("M");
        else if (strcmp(com.recv_buff, master.c_str()) == 0)
            break;
    }
}
