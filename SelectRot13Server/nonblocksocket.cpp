#include "nonblocksocket.h"
#include <errno.h>

NonBlockSocket::NonBlockSocket(int fd)
{
	mSocketFd = fd;
	fcntl(mSocketFd, F_SETFL, O_NONBLOCK);
	mBuf = new char[MAX_SIZE];
	mUsed = mWritten = mMaxWrite = 0;
	mWriting = false;
}

NonBlockSocket::~NonBlockSocket()
{
	if (mBuf)
	{
		delete [] mBuf;
	}
}

int NonBlockSocket::read()
{
	char localBuf[1024];
	ssize_t result = 0;
	
	while (true)
	{
		result = recv(mSocketFd, localBuf, sizeof(localBuf), 0);
		if (result <= 0)
			break;

		for (int i = 0; i < result; i++)
		{
			if (mUsed < MAX_SIZE)
				mBuf[mUsed++] = Rot13(localBuf[i]);
			if (mBuf[i] == '\n')
			{
				mWriting = true;
				mMaxWrite = mUsed;
			}
		}
	}

	if (result == 0)
		return 1;
	else if (result < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return 0;
		return -1;
	}

	return 0;
}

int NonBlockSocket::write()
{
	while (mWritten < mMaxWrite)
	{
		ssize_t result = send(mSocketFd, mBuf + mWritten, mMaxWrite - mWritten, 0);
		if (result < 0)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				return 0;
			return -1;
		}
		else if (result == 0)
			return -1;

		mWritten += result;
	}

	if (mWritten == mUsed)
	{
		mWritten = mMaxWrite = mUsed = 0;
	}

	mWriting = false;

	return 0;
}

char NonBlockSocket::Rot13(char c)
{
	if ((c >= 'a' && c <= 'm') || (c >= 'A' && c <= 'M'))
		return c + 13;
	else if ((c >= 'n' && c <= 'z') || (c >= 'N' && c <= 'Z'))
		return c - 13;
	else
		return c;
}
