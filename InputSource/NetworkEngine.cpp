#include <iostream>

#include "NetworkEngine.h"

#include "../ORUtils/FileUtils.h"

using namespace InputSource;

class NetworkEngine::PrivateData
{
public:
	PrivateData() {}
	SOCKET clientSocket;
};

NetworkEngine::NetworkEngine(const char * calibFilenamea)
	:BaseImageSourceEngine(calibFilenamea)
{
	data = new PrivateData();

	WSADATA wsaData;
	int iRet = 0;
	iRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iRet != 0)
	{
		std::cout << "WSAStartup(MAKEWORD(2, 2), &wsaData) execute failed!" << std::endl;
		return;
	}

	if (2 != LOBYTE(wsaData.wVersion) || 2 != HIBYTE(wsaData.wVersion))
	{
		WSACleanup();
		std::cout << "WSADATA version is not correct!" << std::endl;
		return;
	}

	data->clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (data->clientSocket == INVALID_SOCKET)
	{
		std::cout << "clientSocket = socket(AF_INET, SOCK_STREAM, 0) execute failed!" << std::endl;
		return;
	}

	SOCKADDR_IN srvAddr;
	srvAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_port = htons(8080);

	iRet = connect(data->clientSocket, (SOCKADDR*)&srvAddr, sizeof(SOCKADDR));
	if (iRet != 0)
	{
		std::cout << "connect(clientSocket, (SOCKADDR*)&srvAddr, sizeof(SOCKADDR)) execute failed!" << std::endl;
		return;
	}
	printf("连接已建立");
}
