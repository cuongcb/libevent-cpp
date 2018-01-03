#include "selectserver.h"
#include <iostream>

SelectServer::SelectServer()
{
	mListener = -1;
	mPort = 8000;
}

SelectServer::SelectServer(int port)
{
	mListener = -1;
	mPort = port;
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
		for (int socket_i = 0; socket_i < (int)mSockets.size(); socket_i++)
		{
			if (mSockets[socket_i]->socket() >= maxFds) maxFds = mSockets[socket_i]->socket();
			FD_SET(mSockets[socket_i]->socket(), &readset);
			if (mSockets[socket_i]->isWriting())
			{
				FD_SET(mSockets[socket_i]->socket(), &writeset);
			}
		}

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
				NonBlockSocket *socket = new NonBlockSocket(newFd);
				mSockets.push_back(socket);
			}
		}

		// I/O operation
		for (int socket_i = 0; socket_i < (int)mSockets.size(); socket_i++)
		{
			int ret = 0;
			/* std::cout << "Socket(" << mSockets[socket_i]->socket() << ")" << std::endl; */
			if (socket_i == mListener)
				continue; // already handle above
			
			if (FD_ISSET(mSockets[socket_i]->socket(), &readset))
			{
				ret = mSockets[socket_i]->read();
			}
			
			if (ret == 0 && FD_ISSET(mSockets[socket_i]->socket(), &writeset))
			{
				ret = mSockets[socket_i]->write();
			}

			if (ret)
			{
				delete mSockets[socket_i];
				/* cout << "Vector's size(" << mSockets.size() << ")" << endl;
				 * cout << "Element socket(" << mSockets[socket_i]->socket() << ")" << endl; */
				mSockets.erase(mSockets.begin() + socket_i);
			}
		}
	}
}
