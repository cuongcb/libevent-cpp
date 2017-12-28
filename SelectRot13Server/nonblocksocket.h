#include <sys/socket.h>
#include <fcntl.h>

#define MAX_SIZE 16384

class NonBlockSocket {
	private:
		int mSocketFd;
		char *mBuf;
		size_t mUsed;
		bool mWriting;
		size_t mWritten;
		size_t mMaxWrite;
	public:
		NonBlockSocket(int fd = 0);
		~NonBlockSocket();
		int read(/*arguments*/);
		int write(/*arguments*/);
	private:
		char Rot13(char c);
};
