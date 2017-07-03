#ifndef ZHMU_SNIPPETS_TCPSERVER_H
#define ZHMU_SNIPPETS_TCPSERVER_H

#include <pthread.h>
#include <vector>

namespace zhmu {
namespace snippets {

//! \brief Implements a TCP server supporting multiple clients in a seperate thread
class TCPServer
{
public:
    TCPServer();
    virtual ~TCPServer();

    /*! \brief Creates a TCP server on a given port number
     *  \param port_number TCP port to bind to
    *  \returns true on success
    */
    bool Start(int port_number);

    //! \brief Closes the TCP server, dropping all connections
    void Stop();

    //! \brief Client beloning to this server
    class Client
    {
    public:
        Client(int fd);
        virtual ~Client();

        /*! \brief Called when a socket event has occured
         *  \returns true to continue, false to close the connection
         *
         *  This function should read the socket as well, as this is the event we'll trigger.
         */
        virtual bool OnEvent() = 0;

        /*! \brief Sends data to the client
         *  \param buf Buffer to send
         *  \param length Number of bytes to send
         *  \returns Amount send, or -1 on failure
         */
        int Write(const void* buf, int length);

        /*! \brief Receives data from the client
         *  \param buf Buffer to fill
         *  \param length Number of bytes to read
         *  \returns Amount read, or -1 on failure
         */
        int Read(void* buf, int length);

        //! \brief Retrieves the client's file descriptor in use
        int GetFD() const;

    private:
        int m_FD;
    };
    typedef std::vector<Client*> TClientPtrVector;

    /*! \brief Called when a new client is about to be accepted
     *  \param fd File descriptor of the client
     *  \returns Client pointer, or NULL if the client is to be dropped
     */
    virtual Client* CreateClient(int fd) = 0;

protected:
    //! \brief Thread code
    void Thread();

    //! \brief Wrapper for pthread
    static void* ThreadWrapper(void* context);

    //! \brief Server file descriptor, or -1 if none
    int m_ServerFD;

    //! \brief Server thread accepting clients, if any
    pthread_t m_Thread;

    //! \brief Do we have a server thread?
    bool m_HaveThread;

    //! \brief Clients
    TClientPtrVector m_Clients;
};

}
}

#endif /* ZHMU_SNIPPETS_TCPSERVER_H */
