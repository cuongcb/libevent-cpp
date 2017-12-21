#include "blockingclient.h"

BlockingClient::BlockingClient()
{
	mSocketFd = 0;
	mAddrList = NULL;
	/* memset(mSin, 0x00, sizeof(SockAddr));
	 * mHost = NULL; */
}

BlockingClient::~BlockingClient()
{
	if (mSocketFd)
		close(mSocketFd);

	if (mAddrList)
	{
		freeaddrinfo(mAddrList);
	}
}

int BlockingClient::Connect(string hostname, string port)
{
	if (hostname.empty() || port.empty())
		return -1;

	AddrInfo hints;
	/* Add filter parameters */
	memset(&hints, 0x00, sizeof(AddrInfo));
	hints.ai_family = AF_INET; /* IPv4 */
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0; /* Any protocols */
	hints.ai_flags = 0;

	int ret = getaddrinfo(hostname.c_str(), port.c_str(), &hints, &mAddrList);
	if (ret != 0)
	{
		/* cout << "getaddrinfo() failed!" << endl; */
		return -1;
	}

	AddrInfo *traverser;
	for (traverser = mAddrList; traverser != NULL; traverser = traverser->ai_next)
	{
		mSocketFd = socket(traverser->ai_family, traverser->ai_socktype, traverser->ai_protocol);
		if (mSocketFd == -1)
			continue;

		if (connect(mSocketFd, traverser->ai_addr, traverser->ai_addrlen) != -1)
			break;

		close(mSocketFd);
	}

	if (traverser == NULL)
		return -1;

	return 0;
}

ssize_t BlockingClient::Querry(string msg)
{
	const char *c = msg.c_str();
	ssize_t len = msg.length();
	ssize_t nWritten = 0;
	while (len)
	{
		nWritten = send(mSocketFd, c, len, 0);
		if (nWritten < 0)
		{
			return -1;
		}
		len -= nWritten;
		c += nWritten;	
	}

	return nWritten;
}

ssize_t BlockingClient::Post(char *buf, size_t size)
{
	/* while (true) */
	ssize_t result = recv(mSocketFd, buf, size, 0);
	return result;
}
