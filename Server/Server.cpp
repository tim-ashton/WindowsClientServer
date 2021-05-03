#include <stdio.h>
#include <stdlib.h>
#include <winsock.h>

#define BUFFER_SIZE 1024

void on_error(char *s, int *errCode = NULL)
{
    int err = (errCode) ? *errCode : WSAGetLastError();
    fprintf(stderr, "%s: %d\n", s, err);
    fflush(stderr);
    exit(1);
}

int main(int argc, char *argv[])
{
    WSADATA wsadata;
    SOCKET server_fd, client_fd;
    struct sockaddr_in server, client;
    int port = 3240, err;
    char buf[BUFFER_SIZE];

    err = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (err != 0)
        on_error("Error in WSAStartup.", &err);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == INVALID_SOCKET)
        on_error("Failed to create socket.");

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;

    /** bind & listen **/
    const BOOL opt_val = TRUE;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt_val, sizeof(opt_val));
    err = bind(server_fd, (struct sockaddr *) &server, sizeof(server));
    if (err == SOCKET_ERROR)
        on_error("Unable to bind socket.");
    err = listen(server_fd, 1);
    if (err == SOCKET_ERROR)
        on_error("Unable to listen on socket.");

    printf("SERVER LISTENING ON PORT %d.\n", port);

    while (1)
    {
        int client_len = sizeof(client);
        client_fd = accept(server_fd, (struct sockaddr *) &client, &client_len);

        if (client_fd == INVALID_SOCKET)
        on_error("Unable to establish a new connection.");

        bool keepLooping = true;
        do
        {
        int read = recv(client_fd, buf, BUFFER_SIZE, 0);

        if (read == 0)
            break;

            if (read == SOCKET_ERROR)
            {
                err = WSAGetLastError();
                if ((err != WSAENOTCONN) && (err != WSAECONNABORTED) && (err == WSAECONNRESET))
                    on_error("Error reading from client", &err);
                break;
            }

            char *pbuf = buf;
            do
            {
                int sent = send(client_fd, pbuf, read, 0);
                if (sent == SOCKET_ERROR)
                {
                    err = WSAGetLastError();
                    if ((err != WSAENOTCONN) && (err != WSAECONNABORTED) && (err == WSAECONNRESET))
                        on_error("Error in writing to client", &err);

                    keepLooping = false;
                    break;
                }

                pbuf += sent;
                read -= sent;
            } while (read > 0);
        } while (keepLooping);

        closesocket(client_fd);
    }

    WSACleanup();
    return 0;
}

