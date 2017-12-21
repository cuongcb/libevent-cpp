/* For sockaddr_in */
#include <netinet/in.h>
/* For socket functions */
#include <sys/socket.h>
/* For getaddrinfo */
#include <netdb.h>
#include <sys/types.h>

#include <unistd.h>
#include <cstring>
#include <string>

using namespace std;

/* typedef struct sockaddr_in SockAddr;
 * typedef struct hostent HostEnt; */
typedef struct addrinfo AddrInfo;

class BlockingClient {
	private:
		int mSocketFd;
		AddrInfo *mAddrList;
		/* SockAddr mSin;
		 * HostEnt *mHost; */
	public:
		BlockingClient();
		~BlockingClient();
		int Connect(string hostname, string port);
		ssize_t Post(char *buf, size_t size);
		ssize_t Querry(string msg);
};
