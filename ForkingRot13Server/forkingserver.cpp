#include <netinet/in.h> //htons
#include "forkingserver.h"
#include "responser.h"

ForkingRot13Server::~ForkingRot13Server()
{

}

int ForkingRot13Server::Build()
{
	struct sockaddr_in sin;

	sin.sin_family = AF_INET; //IPv4
	sin.sin_addr.s_addr = 0;
	sin.sin_port = htons(mPort);

	mListener = socket(AF_INET, SOCK_STREAM, 0);
	if (mListener == -1)
		return -1;

	if (bind(mListener, (struct sockaddr *)&sin, sizeof(sin)) == -1)
		return -1;
	
	if (listen(mListener, MAX_RESPONSER) == -1)
		return -1;

	return 0;
}

int ForkingRot13Server::Run()
{
	while (true)
	{
		struct sockaddr_storage ss;
		socklen_t slen = sizeof(ss);
		int fd = accept(mListener, (struct sockaddr *)&ss, &slen);
		if (fd < 0)
			return -1;
		else
		{
			if (fork() == 0)
			{
				Responser *resp = new Responser(fd);
				resp->Run();
				delete resp;
			}
		}
	}
}
