//
// Created by vrenclouff on 29/11/16.
//

#include "SenderService.h"
#include "NetworkService.h"
#include <sys/socket.h>
#include <climits>

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
    char buf[msg.length()+4];
    int i, check_sum = 0;
    char c;
    char checksum_result;

    for(i=2; i < msg.length()+2; i++)
    {
        c = msg[i-2];
        buf[i] = c;
        check_sum += c;
    }
    checksum_result = check_sum % CHECKSUM;
    checksum_result = (checksum_result + 1) % SCHAR_MAX;
    buf[0] = checksum_result;
    buf[1] = STX;
    buf[i] = ETX;
    buf[i+1] = '\0';
    return buf;
}