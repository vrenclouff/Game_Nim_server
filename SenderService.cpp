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
    int i, checksum = 0;
    char c;

    buf[1] = STX;
    for(i=2; i < msg.length()+2; i++)
    {
        c = msg[i-2];
        buf[i] = c;
        checksum = (checksum + c) % CHECKSUM;
    }
    logger->debug(StringUtils::format(2, "Checksum for sender message: ", std::to_string(checksum).c_str()));
    buf[0] = checksum;
    buf[i] = ETX;
    return buf;
}