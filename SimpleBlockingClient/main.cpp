#include <iostream>
#include <stdlib.h>
#include "blockingclient.h"

using namespace std;

int main()
{
	string hostname;
	string port;
	string msg;

	cout << "Host >> ";
	cin >> hostname;
	cout << "Port >> "; 
	cin >> port;
	cout << "Message >> ";
	cin >> msg;

	char *buf;
	buf = (char *)malloc(1024);
	memset(buf, 0x00, 1024);

	BlockingClient *mClient = new BlockingClient();
	if (mClient->Connect(hostname, port) < 0)
	{
		cout << "Failed to connect to " << hostname << "-" << port << endl;
		goto cleanup;
	}

	if (mClient->Querry(msg) < 0)
	{
		cout << "Failed to send querry!" << endl;
		goto cleanup;
	}

	/* char *buf;
	 * buf = (char *)malloc(1024);
	 * memset(buf, 0x00, 1024); */

	for (int i = 0; i < 10; i++)
	{
		int ret = mClient->Post(buf, 1023);
		if (ret == 0)
			break;
		else if (ret < 0)
		{
			cout << "Failed to get response!" << endl;
			goto cleanup;
		}
		else
		{
			buf[1023] = '\0';
			cout << buf << endl;
		}
	}

cleanup:
	if (buf)
		free(buf);
	delete mClient;
	return 0;
}
