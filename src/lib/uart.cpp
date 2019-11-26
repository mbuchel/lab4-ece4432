/*
 * Author: Michael Buchel
 */
#include <uart.h>

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <termios.h>
#include <fcntl.h>
#include <poll.h>
#include <stdlib.h>
#include <unistd.h>

#include <iostream>

/*
 * uart - default constructor so you do not need to clear the buffer
 */
uart::uart()
{
    memset(recv_buff, 0, MAX_BUFFER);
}

/*
 * recv_msg - reads from the uart
 */
ssize_t uart::recv_msg()
{
    ssize_t ret;
    struct pollfd temp;

    temp.fd = fd;
    temp.events = POLLIN;

    memset(recv_buff, 0, strlen(recv_buff));
    poll(&temp, 1, 1000);

    if (temp.revents & POLLIN)
        ret = read(fd, recv_buff, MAX_BUFFER - 1);
    else
        ret = -1;

    if (ret)
        std::cerr << "Receive: " << recv_buff;

    return ret;
}

/*
 * send_msg - sends to device
 * @msg - message to send to device
 */
ssize_t uart::send_msg(const std::string &msg)
{
    std::cerr << "Send: " << msg << "\n";
    return write(fd, msg.c_str(), msg.size());
}

/*
 * release - closes the uart port
 */
void uart::release()
{
    close(fd);
}

/*
 * start - starts the uart port
 * @str - name of the device to open
 */
void uart::start(const char *str)
{
    /*
     * The following baud rates work on linux:
     *
     * B50, B75, B110, B134, B150, B200, B300,
     * B600, B1200, B1800, B2400, B4800, B9600,
     * B19200, B38400, B57600, B115200, B230400,
     * B460800, B500000, B576000, B921600, B1000000,
     * B1152000, B1500000, B2000000, B2500000,
     * B3000000, B3500000, B4000000
     */
    static const speed_t BAUD_RATE = B9600;

    char errorstr[MAX_BUFFER];

    struct termios tty;

    fd = open(str, O_RDWR | O_NOCTTY);

    if (fd == -1) {
        sprintf(errorstr, "Error opening uart: %s\n", strerror(errno));
        errno = 0;
    }

    cfsetispeed(&tty, BAUD_RATE);
    cfsetospeed(&tty, BAUD_RATE);

    tty.c_lflag = ICANON;
    tty.c_iflag = IGNCR;
    tty.c_oflag = OPOST | ONLCR;
#if 0
    tty.c_cflag = 0x54B8FCB8;
#else
    tty.c_cflag |= CREAD | CS8;
#endif

    tcflush(fd, TCIFLUSH);

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        sprintf(errorstr, "Error setting uart: %s\n", strerror(errno));
        errno = 0;
    }
}
