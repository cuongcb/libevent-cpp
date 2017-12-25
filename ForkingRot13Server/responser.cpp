#include "responser.h"

char Responser::Rot13(char ch)
{
	if ((ch >= 'a' && ch <= 'm') || (ch >= 'A' && ch <= 'M'))
		return ch + 13;
	else if ((ch >= 'n' && ch <= 'z') || (ch >= 'N' && ch <= 'Z'))
		return ch - 13;

	return ch;
}

int Responser::Run()
{
	size_t used = 0;
	ssize_t result = 0;
	while (true)
	{
		char ch;
		result = recv(mSocketFd, &ch, 1, 0);
		if (result <= 0)
			break;

		if (used < sizeof(mBuf))
		{
			mBuf[used++] = Rot13(ch);
		}

		if (ch == '\n')
		{
			send(mSocketFd, mBuf, used, 0);
			used = 0;
			continue;
		}
		
		/* if (ch == '-')
		 *         break; */
	}

	return result;
}
