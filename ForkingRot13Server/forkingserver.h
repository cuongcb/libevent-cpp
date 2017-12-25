#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_RESPONSER 16

class ForkingRot13Server {
	private:
		int mListener;
		int mPort;
	public:
		ForkingRot13Server(int port = 5555): mPort(port) {  }
		~ForkingRot13Server();
		int Build();
		int Run();
};
