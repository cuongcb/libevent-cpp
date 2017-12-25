#include "forkingserver.h"
#include <iostream>
#include <sstream>

using namespace std;

#define MAX_LINE 16384

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		cout << "[Usage]: " << argv[0] << " <port>" << endl;
		return -1;
	}

	int port;
	istringstream myStream(argv[1]);
	myStream >> port;

	ForkingRot13Server *mServer = new ForkingRot13Server(port);
	if (mServer->Build() < 0)
	{
		cout << "Failed: Build Server(" << argv[1] << ")" << endl;
		return -1;
	}

	cout << "Server Running on port(" << argv[1] << ")" << endl;
	mServer->Run();

	return 0;
}
