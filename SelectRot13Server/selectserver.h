#include "nonblocksocket.h"
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <stdlib.h>
#include <unistd.h>

#include <vector>

using namespace std;

class SelectServer {
	private:
		int mListener;
		int mPort;
		vector<NonBlockSocket *> mSockets;
	public:
		SelectServer();
		SelectServer(int port);
		~SelectServer();
		int Build();
		void Run();
};
