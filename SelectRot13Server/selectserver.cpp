#include "selectserver.h"
#include <iostream>

SelectServer::SelectServer()
{
	mListener = -1;
	mPort = 8000;
	/* mSockets.reserve(FD_SETSIZE); */
	/* for (int socket_i = 0; socket_i < FD_SETSIZE; socket_i++)
	 * {
	 *         mSockets[socket_i] = NULL;
	 * } */
}

SelectServer::SelectServer(int port)
{
	mListener = -1;
	mPort = port;
	/* mSockets.reserve(FD_SETSIZE); */
	/* for (int socket_i = 0; socket_i < FD_SETSIZE; socket_i++)
	 * {
	 *         mSockets[socket_i] = NULL;
	 * } */
}

SelectServer::~SelectServer()
{
	if (mListener != -1)
		close(mListener);

	for (int socket_i = 0; socket_i < (int)mSockets.size(); socket_i++)
	{
		if (mSockets[socket_i] != NULL)
			free(mSockets[socket_i]);
	}
}

int SelectServer::Build()
{
	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = 0;
	sin.sin_port = htons(mPort);

	mListener = socket(AF_INET, SOCK_STREAM, 0);
	if (mListener == -1)
		return -1;

	if (bind(mListener, (struct sockaddr *)&sin, sizeof(sin)) < 0)
		return -1;

	if (listen(mListener, MAX_LISTENER) < 0)
		return -1;

	return 0;
}

int SelectServer::Run()
{
	int maxFds; // argument for select, equal to maximum fd plus 1
	fd_set readset, writeset, exset;

	FD_ZERO(&readset);
	FD_ZERO(&writeset);
	FD_ZERO(&exset);

	while (true)
	{
		maxFds = mListener;

		// Clear previous state
		FD_ZERO(&readset);
		FD_ZERO(&writeset);
		FD_ZERO(&exset);

		// Add mListener to readset
		FD_SET(mListener, &readset);

		// Category other sockets
		std::cout << "Vector size(" << mSockets.size() << ")" << std::endl;
		std::cout << "FD_SETSIZE(" << FD_SETSIZE << ")" << std::endl;

		for (int socket_i = 0; socket_i < (int)mSockets.size(); socket_i++)
		{
			/* if (mSockets[socket_i] != NULL) */
			{
				std::cout << "Before Socket_i(" << mSockets[socket_i]->socket() << ")" << std::endl;
				if (mSockets[socket_i]->socket() >= maxFds) maxFds = mSockets[socket_i]->socket();
				FD_SET(mSockets[socket_i]->socket(), &readset);
				if (mSockets[socket_i]->isWriting())
				{
					FD_SET(mSockets[socket_i]->socket(), &writeset);
				}
			}
		}

		std::cout << "Max FD(" << maxFds << ")" << std::endl;

		// Polling on non-blocking sockets by select()
		if (select(maxFds + 1, &readset, &writeset, &exset, NULL) < 0)
			return -1;
		
		// New connection handler
		if (FD_ISSET(mListener, &readset))
		{
			struct sockaddr_storage ss;
			socklen_t slen = sizeof(ss);
			int newFd = accept(mListener, (struct sockaddr *)&ss, &slen);
			if (newFd < 0)
				return -1;
			else if (newFd > FD_SETSIZE)
				close(newFd);
			else
			{
				/* mSockets[newFd-1] = new NonBlockSocket(); */
				NonBlockSocket *socket = new NonBlockSocket(newFd);
				mSockets.push_back(socket);
				std::cout << "New connection(" << newFd << ") " << std::endl;
			}
		}

		// I/O operation
		for (int socket_i = 0; socket_i < (int)mSockets.size(); socket_i++)
		{
			int ret = 0;
			std::cout << "Socket(" << socket_i << ")" << std::endl;
			if (socket_i == mListener)
				continue; // already handle above
			
			if (FD_ISSET(mSockets[socket_i]->socket(), &readset))
			{
				std::cout << "Do read..." << std::endl;
				ret = mSockets[socket_i]->read();
				std::cout << "Read done!" << std::endl;
			}
			
			if (ret == 0 && FD_ISSET(mSockets[socket_i]->socket(), &writeset))
			{
				std::cout << "Do write..." << std::endl;
				ret = mSockets[socket_i]->write();
				std::cout << "Write done!" << std::endl;
			}

			if (ret)
			{
				mSockets.erase(mSockets.begin() + socket_i);
				/* delete mSockets[socket_i]; */
			}
		}
	}
}
