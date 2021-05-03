#include "network.h"

static const char* host = "127.0.0.1";
static const unsigned int port = 3240;

int main()
{

    SOCKET conn = connect_to_port(host, port);

    closesocket(conn);
    return 0;
}

