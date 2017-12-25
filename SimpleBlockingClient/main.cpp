#include <iostream>
#include <stdlib.h>
#include "blockingclient.h"

using namespace std;

int main()
{
	string hostname;
	string port;
	string msg;
	string querry;
	int ret = 0;
	/* string msg = "GET / HTTP/1.1\r\n"
	 *         "Host: www.google.com\r\n"
	 *         "\r\n"; */

	cout << "Host >> ";
	cin >> hostname;
	cout << "Port >> "; 
	cin >> port;
	cout << "Message >> ";
	getline(cin, querry);
	while (getline(cin, querry) && !querry.empty())
	{
		msg += querry;
		msg += '\n';
	}
	msg += '\n';
	/* getline(cin >> ws, msg);  */
	/* cin >> msg; */

	char *buf;
	buf = (char *)malloc(1024);
	memset(buf, 0x00, 1024);

	BlockingClient *mClient = new BlockingClient();
	if (mClient->Connect(hostname, port) < 0)
	{
		cout << "Failed to connect to " << hostname << ":" << port << endl;
		goto cleanup;
	}

	cout << "Successfully connect to " << hostname << ":" << port << "!!!" << endl;

	if (mClient->Querry(msg) < 0)
	{
		cout << "Failed to send querry!" << endl;
		goto cleanup;
	}

	cout << "Sending querry: \"" << msg << "\"" << endl;

	cout << "Waiting responses from " << hostname << "..." << endl;
	cout << "*****/*****/*****" << endl;

	ret = mClient->Post(buf, 1023);
	if (ret == 0)
	{
		cout << "Peer is disconnected!!!" << endl;
	}
	else if (ret < 0)
	{
		cout << "Failed to get response!" << endl;
	}
	else
	{
		buf[1023] = '\0';
		cout << buf << endl;
	}

	cout << "*****/*****/*****" << endl;
	cout << "Disconnected from " << hostname << ":" << port << "!!!" << endl;

cleanup:
	if (buf)
		free(buf);
	delete mClient;
	return 0;
}
