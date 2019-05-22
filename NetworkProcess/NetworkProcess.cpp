#include <iostream>
#include <WinSock2.h>

#pragma comment(lib,"ws2_32.lib")

void main()
{
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

	SOCKET clientSocket;
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
	{
		std::cout << "clientSocket = socket(AF_INET, SOCK_STREAM, 0) execute failed!" << std::endl;
		return;
	}


	SOCKADDR_IN srvAddr;
	srvAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_port = htons(8080);

	iRet = connect(clientSocket, (SOCKADDR*)&srvAddr, sizeof(SOCKADDR));
	if (iRet != 0)
	{
		std::cout << "connect(clientSocket, (SOCKADDR*)&srvAddr, sizeof(SOCKADDR)) execute failed!" << std::endl;
		return;
	}
	printf("连接已建立");

	unsigned int bufSize = 640 * 480 * 3 * 2;

	TCHAR szNameImage[] = TEXT("imageData");
	HANDLE hMapFileImage = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		sizeof(unsigned char) * bufSize,
		szNameImage
	);
	unsigned char* recvImage = (unsigned char*)MapViewOfFile(
		hMapFileImage,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		sizeof(unsigned char) * bufSize
	);

	TCHAR szNamePose[] = TEXT("pose");
	HANDLE hMapFilePose = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		sizeof(unsigned char) * 28,
		szNamePose
	);
	unsigned char* recvPose = (unsigned char*)MapViewOfFile(
		hMapFilePose,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		sizeof(unsigned char) * 28
	);

	char sendBuf = 0xa5;
	send(clientSocket, &sendBuf, 1, 0);

	printf("开始接收图片...\n");
	while (1)
	{
		iRet = recv(clientSocket, (char*)recvImage, bufSize, 0);
		while (iRet != bufSize)
		{
			iRet += recv(clientSocket, &((char*)recvImage)[iRet], bufSize - iRet, 0);
		}

		iRet = recv(clientSocket, (char*)recvPose, 28, 0);
		while (iRet != 28)
		{
			iRet += recv(clientSocket, &((char*)recvPose)[iRet], 28 - iRet, 0);
		}
	}
	closesocket(clientSocket);
	WSACleanup();
	return;
}