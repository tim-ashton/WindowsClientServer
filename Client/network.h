#ifndef NETWORK_H
#define NETWORK_H

#include <ws2tcpip.h>


SOCKET connect_to_port(const char *hostname, unsigned int port);


#endif


