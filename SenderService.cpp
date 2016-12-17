//
// Created by vrenclouff on 29/11/16.
//

#include "SenderService.h"
#include "NetworkService.h"
#include <sys/socket.h>

void SenderService::run()
{
    while(1)
    {
        logger->debug("Waiting for send message.");
        SNDMessage message = send_queue->pop();
        std::string msg = createValidMessage(StringUtils::format(3, EnumUtils::to_string(message.state).c_str(), " ", message.parameters.c_str()));
        logger->debug(StringUtils::format(4, "Sending to user: ", std::to_string(message.socket).c_str() ," message: ", msg.c_str()));
        send(message.socket, msg.c_str(), msg.length(), 0);
    }
}

std::string SenderService::createValidMessage(std::string const &msg)
{
    char buf[msg.length()+3];
    int i;
    long check_sum = 0;
    char c;

    for(i=2; i < msg.length()+2; i++)
    {
        c = msg[i-2];
        buf[i] = c;
        check_sum += c;
    }
    buf[0] = check_sum % CHECKSUM;
    buf[1] = STX;
    buf[i] = ETX;
    return buf;
}