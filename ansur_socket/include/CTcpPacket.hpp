#ifndef CTCPPACKET_HPP_
#define CTCPPACKET_HPP_

#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "socketDef.hpp"

class CTcpPacket
{
public:
    CTcpPacket();
    ~CTcpPacket();

    short getHeader()
    {
        return header;
    }

    template<class T>
    void decode(T &val)
    {
        memcpy(&val, &packetBuffer[decodeIndex], sizeof(val));
        decodeIndex = decodeIndex + sizeof(val);
    }

    int readPacket();
    bool isRemoteON = true;

private:
    struct sockaddr_in server_addr, client_addr;
    int client_addr_size = sizeof(client_addr);
    int server_fd, client_fd;

    short header;
    int decodeIndex;
    uint16_t dataSize;

    unsigned char rxBuffer[RX_BUFFER_SIZE];
    unsigned char packetBuffer[PACKET_BUFFER_SIZE];

    int retval;
};

#endif  // CTCPPACKET_HPP_
