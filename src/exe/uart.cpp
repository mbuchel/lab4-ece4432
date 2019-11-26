/*
 * Author: Michael Buchel
 */
#include <uart.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <errno.h>
#include <iostream>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

static uart com;

static void write_uart()
{
	std::string send = readline("Input> ");

	add_history(send.c_str());
	
	if (!strcasecmp(send.c_str(), "exit"))
		exit(EXIT_SUCCESS);
	
	send = send + "\n";
	com.send_msg(send);
	usleep(33000);
}

static void *listen(void *input)
{
	while (1) {
		errno = 0;
		if (com.recv_msg() == -1 && errno == 0) {
			continue;
		} else if (errno) {
			fprintf(stderr, "Error> %s\n", strerror(errno));
			continue;
		}

		printf("%s", com.recv_buff);
	}
	
	return input;
}

int main(int argc, char **argv)
{
	pthread_t thread;

	com.start(argv[1]);
	
	sleep(1);
	pthread_create(&thread, NULL, listen, NULL);
	pthread_detach(thread);
	
	while (1)
		write_uart();
}
