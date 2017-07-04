#include "tcp-server.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <assert.h>
#include <algorithm>
#include <unistd.h>

namespace zhmu {
namespace snippets {

TCPServer::TCPServer()
	: m_ServerFD(-1), m_HaveThread(false)
{
}

TCPServer::~TCPServer()
{
	Stop();
}

bool TCPServer::Start(int port_number)
{
	assert(m_ServerFD < 0); // already started?

	m_ServerFD = socket(AF_INET, SOCK_STREAM, 0);
	if (m_ServerFD < 0)
		return false;

	// Enable address re-use so we don't have to wait until it expires
	{
		int on = 1;
    setsockopt(m_ServerFD, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
  }

	// Bind to our port number
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port_number);
	if (bind(m_ServerFD, (struct sockaddr*)&sin, sizeof(sin)) < 0)
		return false;

	// We want incoming connections
	if (listen(m_ServerFD, 5) < 0)
		return false;

	// And we have a thread to take care of them
	if (pthread_create(&m_Thread, NULL, &ThreadWrapper, this) < 0)
		return false;
	
	m_HaveThread = true;
	return true;
}

void TCPServer::Stop()
{
	if (m_ServerFD < 0)
		return; // wasn't started
 
	// Get rid of the server; the select() call will stop
	close(m_ServerFD);
	m_ServerFD = -1;

	// If we have a thread, get rid of it - it will have stopped with the loop as
	// we have closed the server socket
	if (m_HaveThread) {
		pthread_join(m_Thread, NULL);
		m_HaveThread = false;
	}

	// Get rid of any clients, now that the thread is gone
	for(TClientPtrVector::iterator it = m_Clients.begin(); it != m_Clients.end(); it++)
		delete *it;
	m_Clients.clear();
}

void* TCPServer::ThreadWrapper(void* context)
{
	((TCPServer*)context)->Thread();
	return NULL;
}

void TCPServer::Thread()
{
	while(m_ServerFD >= 0) {
		// Construct the list of file descriptors; this will be the server and all clients
		int max_fd = m_ServerFD;
		fd_set fds;
		FD_ZERO(&fds);
		FD_SET(m_ServerFD, &fds);
		for(TClientPtrVector::iterator it = m_Clients.begin(); it != m_Clients.end(); it++) {
			Client& client = **it;

			// Add the client's file descriptor and update the max
			FD_SET(client.GetFD(), &fds);
			max_fd = std::max(max_fd, client.GetFD());
		}

		// Wait until some socket has some event to report
		int n = select(max_fd + 1, &fds, NULL, NULL, NULL);
		if (n < 0)
			break;

		// If our server socket has an event, it's a new client
		if (FD_ISSET(m_ServerFD, &fds)) {
			// New connection; accept it
			int fd = accept(m_ServerFD, NULL, NULL);
			if (fd >= 0) {
				Client* client = CreateClient(fd);
				if (client != NULL) {
					// Client accepted; add to the list of clients
					m_Clients.push_back(client);
				} else {
					// Client rejected; ditch the connection
					close(fd);
				}
			}
		}

		for (TClientPtrVector::iterator it = m_Clients.begin(); it != m_Clients.end(); /* nothing */) {
			Client& client = **it;
			if (!FD_ISSET(client.GetFD(), &fds)) {
				// Not this client; ignore
				it++;
				continue;
			}

			if (client.OnEvent()) {
				// Client accepted the event; continue
				it++;
				continue;
			}

			// Clean up the client
			it = m_Clients.erase(it);
			delete &client;
		}
	}
}

TCPServer::Client::Client(int fd)
	: m_FD(fd)
{
}

TCPServer::Client::~Client()
{
	close(m_FD);
}

int TCPServer::Client::GetFD() const
{
	return m_FD;
}

int TCPServer::Client::Read(void* buf, int length)
{
	return (int)recv(m_FD, buf, length, 0);
}


}
}
