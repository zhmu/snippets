#include <err.h>
#include <stdio.h>
#include <unistd.h>
#include "tcp-server.h"

class DemoTCPClient;

class DemoTCPServer : public zhmu::snippets::TCPServer
{
public:
    class DemoClient : public Client
    {
    public:
        DemoClient(int fd);

        virtual bool OnEvent();
    };

    virtual DemoClient* CreateClient(int fd);
};

DemoTCPServer::DemoClient::DemoClient(int fd)
    : Client(fd)
{
}

DemoTCPServer::DemoClient* DemoTCPServer::CreateClient(int fd)
{
    // Create a new client with the file descriptor we just got
    return new DemoClient(fd);
}

bool DemoTCPServer::DemoClient::OnEvent()
{
    // Try to read a chunk of at most 64 bytes
    char buf[64];
    int n = Read(buf, sizeof(buf) - 1);
    if (n <= 0)
        return false; // short read or error; ditch the client
    buf[n] = '\0';

    // And just display it
    printf("got data [%s]\n", buf);
    return true;
}

int main()
{
    DemoTCPServer server;

    if (!server.Start(12345))
        err(1, "unable to start server");

    for(;;) {
        // Not much going on here...
        sleep(1);
    }

    server.Stop();

    return 0;
}
