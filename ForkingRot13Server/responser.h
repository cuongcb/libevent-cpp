#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>

#define MAX_SIZE 16384

class Responser {
	private:
		int mSocketFd;
		char mBuf[MAX_SIZE+1];
		char Rot13(char ch);
	public:
		Responser(int fd = -1): mSocketFd(fd) { std::cout << "New Responser - " << fd << std::endl; }
		~Responser() { std::cout << "Destroyed a responser - " << mSocketFd << std::endl; }
		int Run();

};
