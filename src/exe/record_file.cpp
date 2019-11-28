/*
 * Author: Michael Buchel
 */
#include <recorder.h>
#include <iostream>
#include <uart.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <streambuf>

const std::string setup = "Please enter operation mode";
const std::string master = "You selected operation mode: M";
const std::string command_choice = "Please send me a command!";

const std::string send_transcript = "SEND TRANSCRIPT";

void send_message(uart &com)
{
    std::string command;
    std::cout << "Input command to send: ";
    std::getline(std::cin, command);
    if (command == send_transcript) {
        std::ifstream t("transcript");
        command = "";

        t.seekg(0, std::ios::end);
        command.reserve(t.tellg());
        t.seekg(0, std::ios::beg);

        command.assign((std::istreambuf_iterator<char>(t)),
                       std::istreambuf_iterator<char>());
    }
    com.send_msg(command);
}

void handle_uart(uart &com)
{
    while (1) {
        if (com.recv_msg() == -1)
            continue;

        if (strncmp(com.recv_buff, setup.c_str(), setup.size()) == 0)
            com.send_msg("M");
        else if (strncmp(com.recv_buff, master.c_str(), master.size()) == 0)
            break;
    }

    while (1) {
        if (com.recv_msg() == -1)
            continue;

        if (strncmp(com.recv_buff, command_choice.c_str(), command_choice.size()) == 0)
            send_message(com);
    }
}

int main(int argc, const char **argv)
{
    if (argc < 2) {
        return -1;
    }

    try {
        recorder rec;
        uart comm;
        rec.record();
        //rec.play();
        rec.save_file("audio.raw");
        rec.close();
        system("sox -t raw -r 16000 -b 16 -c 1 -L -e signed-integer audio.raw audio.wav");
        system("deepspeech --model models/output_graph.pbmm --alphabet models/alphabet.txt --lm models/lm.binary --trie models/trie --audio audio.wav > transcript 2> log");
        comm.start(argv[1]);
        handle_uart(comm);
    } catch (...) {

    }

    return 0;
}
