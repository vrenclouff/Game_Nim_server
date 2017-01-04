//
// Created by vrenclouff on 26/11/16.
//

#include "NetworkService.h"

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <climits>

void NetworkService::run()
{
    logger->debug("Starting inicialization server socket.");
    initSocket(port);

    logger->debug("Starting listen users.");
    startListeningLoop();
}

void NetworkService::initSocket(int port)
{
    struct sockaddr_in my_addr;

    logger->debug("Creating socket with param AF_INET, SOCK_STREAM, 0");
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    memset(&my_addr, 0, sizeof(struct sockaddr_in));

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    int optval = 1;
    int optlen = sizeof(optval);

    logger->debug("Setting setsockopt() for enable tcp_keep_alive.");
    if(setsockopt(server_socket, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen) < 0)
    {
        logger->error("Set socket FD's option SO_KEEPALIVE at protocol level SOL_SOCKET");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    optval = 3;
    /* tcp_keepalive_time */
    logger->debug("Settings setsockopt() for tcp_keepalive_time.");
    if(setsockopt(server_socket, IPPROTO_TCP, TCP_KEEPIDLE, &optval, optlen) < 0)
    {
        logger->error("Set socket FD's option TCP_KEEPIDLE at protocol level IPPROTO_TCP");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    optval = 2;
    /* tcp_keepalive_probes */
    logger->debug("Settings setsockopt() for tcp_keepalive_probes.");
    if(setsockopt(server_socket, IPPROTO_TCP, TCP_KEEPCNT, &optval, optlen) < 0)
    {
        logger->error("Set socket FD's option TCP_KEEPCNT at protocol level IPPROTO_TCP");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    optval = 2;
    /* tcp_keepalive_intvl */
    logger->debug("Settings setsockopt() for tcp_keepalive_intvl.");
    if(setsockopt(server_socket, IPPROTO_TCP, TCP_KEEPINTVL, &optval, optlen) < 0)
    {
        logger->error("Set socket FD's option TCP_KEEPINTVL at protocol level IPPROTO_TCP");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    logger->debug("Settings setsockopt() for reuse port.");
    if(setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &optval, optlen) < 0)
    {
        logger->error("Set socket FD's option SO_REUSEADDR at protocol level SOL_SOCKET");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    logger->debug("Binding the host address using bind() call.");
    if (bind(server_socket, (struct sockaddr *) &my_addr, sizeof(struct sockaddr_in)) < 0)
    {
        logger->error("Give the socket FD the local address ADDR");
        exit(EXIT_FAILURE);
    }

    logger->debug("Start listening for the clients.");
    if (listen(server_socket, 10) < 0)
    {
        logger->error("Prepare to accept connections on socket FD");
        exit(EXIT_FAILURE);
    }

    logger->debug("Adding server socket to FD_SET.");
    FD_ZERO( &socks );
    FD_SET( server_socket, &socks );

    logger->info(StringUtils::format(4, "Listening at ", inet_ntoa(my_addr.sin_addr), ":", std::to_string(port).c_str()));
}

void NetworkService::startListeningLoop()
{
    int client_socket, fd, return_value, len_addr, a2read;
    char cbuf[1024];
    struct sockaddr_in peer_addr;
    fd_set temp_socks;

    while (1)
    {
        temp_socks = socks;
        return_value = select(FD_SETSIZE, &temp_socks, (fd_set *) 0, (fd_set *) 0, (struct timeval *) 0);

        if (return_value < 0)  { logger->error("Select - ERR"); break; }
        if (return_value == 0) { logger->error("select() timed out.  End program."); break; }

        for (fd = 3; fd < FD_SETSIZE; ++fd) {
            if (FD_ISSET(fd, &temp_socks))
            {
                if (fd == server_socket)
                {
                    client_socket = accept(server_socket, (struct sockaddr *) &peer_addr, (socklen_t *) &len_addr);
                    FD_SET(client_socket, &socks);
                    logger->info(StringUtils::format(2, "New connection with socket ID: ", std::to_string(client_socket).c_str()));
                } else
                {
                    ioctl(fd, FIONREAD, &a2read);
                    if (a2read > 0 && a2read < sizeof(cbuf))
                    {
                        int ln = recv(fd, cbuf, sizeof(cbuf), 0);
                        cbuf[ln] = '\0';

                        std::vector<std::string> receive_messages;
                        validationMessage(cbuf, receive_messages);
                        for(int i=0;i<receive_messages.size();i++)
                        {
                            std::string validated_message = receive_messages[i];
                            RCVMessage msg = RCVMessage(fd, validated_message);
                            if (msg.state == enums::PONG) {
                                snd_queue->push(SNDMessage(fd, enums::PONG, ""));
                            }else {
                                logger->info(StringUtils::format(4, "User with ID ", std::to_string(fd).c_str(), " receive message: ", validated_message.c_str()));
                                rcv_queue->push(msg);
                            }
                        }
                        memset(cbuf, 0, sizeof cbuf);
                    } else
                    {
                        close(fd); FD_CLR(fd, &socks);
                        rcv_queue->push(RCVMessage(fd, EnumUtils::network_state_str[enums::HARD_LOGOUT]));
                    }
                }
            }
        }
    }
}

/**
 * Validace prijate zpravy
 * Zprava ma format:
 *      8b - checksum
 *      8b - STX znacka (zacatek zpravy)
 *      obsah zpravy
 *      konecnych 8b - ETX znacka (konec zpravy)
 *
 * @param original_message - prijata zprava
 * @param valided_message - zvalidovana zprava (oriznuta o cislo checksumy, pocatecni a konecne znacky)
 * @return true/false vysledek validace
 */
void NetworkService::validationMessage(std::string const &original_message, std::vector<std::string> &validated_messages)
{
    logger->debug(StringUtils::format(2, "Validation receive message: ", original_message.c_str()));

    /* Kontrola minimalni delky zpravy */
    if (original_message.length() < 2)
    {
        logger->debug("The message does not have correct length.");return;
    }

    /* Kontrola pocatecniho znacky zpravy */
    char stx_mark = original_message[0];
    if (stx_mark != STX)
    {
        logger->debug("The message does not have STX mark.");return;
    }

    char temp[original_message.length()];
    char c;
    int i;
    bool start_end_mark = false;

    /* Kontrola konecne znacky zpravy a checksumy */
    for(i = 0; i < original_message.length()-1; i++)
    {
        if ((c = original_message[i+1]) == ETX) { start_end_mark = true; break; }
        temp[i] = c;
    }

    if (!start_end_mark)
    {
        logger->debug("The message does not have ETX mark.");
        return;
    }

    std::string validated_message = std::string(temp, i);

    bool contains_status = false;
    int statuses_count = EnumUtils::network_state_str.size();
    std::string state_string = StringUtils::split(validated_message, " ")[0];

    /* Kontrola, zda zprava obsahuje validni prikaz */
    for(int i=0;i<statuses_count;i++)
    {
        std::string status = EnumUtils::network_state_str[i];
        if (state_string.compare(status)) { contains_status = true; break; }
    }

    if (!contains_status)
    {
        logger->debug("The message does not have STATE.");
        return;
    }

    validated_messages.push_back(validated_message);
    logger->debug("The message is successful validated.");

    int start = validated_message.size() + 1 + 1;
    int end = original_message.size() - start;
    std::string next_message = original_message.substr(start, end);

    if (next_message.compare("") == 0) { return; }
    else { validationMessage(next_message, validated_messages); }
}