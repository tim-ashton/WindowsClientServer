#ifndef NETWORK_H
#define NETWORK_H

#include <ws2tcpip.h>


SOCKET connect_to_port(PCWSTR hostname, unsigned int port);


#endif


