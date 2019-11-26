/*
 * Author: Michael Buchel
 */
#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <string>
#include <unistd.h>

static const uint16_t MAX_BUFFER = 65535;

/*
 * uart - class to communicate with harvester via uart
 *
 * PUBLIC MEMBERS
 * @recv_msg - reads from the uart
 * @release - closes the uart
 * @send_msg - sends the command to the uart
 * @start - opens the uart
 */
class uart {
protected:
    int fd;
public:
    char recv_buff[MAX_BUFFER];
    char send_buff[MAX_BUFFER];

public:
    uart();

    ssize_t recv_msg();
    ssize_t send_msg(const std::string &msg);

    void release();
    void start(const char *str);
};

#endif
