#include "selectserver.h"

SelectServer::SelectServer()
{
	mListener = -1;
	mPort = 8000;
	mSockets.reserve(FD_SETSIZE);
}

SelectServer::SelectServer(int port)
{
	mListener = -1;
	mPort = port;
	mSockets.reserve(FD_SETSIZE);
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

int Build()
{
	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = 0;
	sin.sin_port = htons(mPort);

	mListener = socket(AF_INET, SOCK_STREAM, 0);
	if (mListener == -1)
		return -1;
}
