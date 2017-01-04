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
        std::string msg = createValidMessage(EnumUtils::to_string(message.state) + std::string(" ") + message.parameters);
        if (message.state != enums::PONG)
        {
            logger->info(StringUtils::format(4, "Sending to user: ", std::to_string(message.socket).c_str() ," message: ", msg.c_str()));
        }
        if (msg.length() == 0) { continue; }
        send(message.socket, msg.c_str(), msg.length(), 0);
    }
}

std::string SenderService::createValidMessage(std::string const &msg)
{
    char buf[msg.length()+2];
    int i;
    char c;

    for(i=1; i < msg.length()+1; i++)
    {
        c = msg[i-1];
        buf[i] = c;
    }
    buf[0] = STX;
    buf[i] = ETX;
    return buf;
}