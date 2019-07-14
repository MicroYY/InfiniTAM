#include <iostream>
#include <WinSock2.h>

#include "Network2Engine.h"

#include "../ORUtils/FileUtils.h"

#pragma comment(lib,"ws2_32.lib")

#ifdef COMPILE_WITH_NETWORK2

using namespace InputSource;

class Network2Engine::PrivateData
{
public:
	PrivateData()
	{
		TCHAR* szName = "imageData";
		HANDLE hMapFileImageRGB = CreateFileMapping(
			INVALID_HANDLE_VALUE,
			NULL,
			PAGE_READWRITE,
			0,
			sizeof(unsigned char) * bufSize / 2,
			szName
		);
		recvImgRGB = (unsigned char*)MapViewOfFile(
			hMapFileImageRGB,
			FILE_MAP_ALL_ACCESS,
			0,
			0,
			sizeof(unsigned char) * bufSize / 2);
	}
	~PrivateData()
	{
		//free(recvImgRGB);
		free(recvImgDepth);
	}

	void TCPConnect() 
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
		clientSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (clientSocket == INVALID_SOCKET)
		{
			std::cout << "clientSocket = socket(AF_INET, SOCK_STREAM, 0) execute failed!" << std::endl;
			return;
		}

		SOCKADDR_IN srvAddr;
		srvAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
		srvAddr.sin_family = AF_INET;
		srvAddr.sin_port = htons(8088);

		iRet = connect(clientSocket, (SOCKADDR*)&srvAddr, sizeof(SOCKADDR));
		if (iRet != 0)
		{
			std::cout << "connect(clientSocket, (SOCKADDR*)&srvAddr, sizeof(SOCKADDR)) execute failed!" << std::endl;
			return;
		}
		printf("连接已建立");


		char sendBuf = 0xa5;
		send(clientSocket, &sendBuf, 1, 0);
	}

	void rec()
	{
		auto iRet = recv(clientSocket, (char*)recvImgRGB, bufSize / 2, 0);
		while (iRet != bufSize / 2)
		{
			iRet += recv(clientSocket, &((char*)recvImgRGB)[iRet], bufSize / 2 - iRet, 0);
		}

		iRet = recv(clientSocket, (char*)recvImgDepth, bufSize / 2, 0);
		while (iRet != bufSize / 2)
		{
			iRet += recv(clientSocket, &((char*)recvImgDepth)[iRet], bufSize / 2 - iRet, 0);
		}
	}

	SOCKET clientSocket;
	uchar* recvImgRGB;
	uchar* recvImgDepth;
	unsigned int bufSize = 640 * 480 * 3 * 2;
};


Network2Engine::Network2Engine(const char * calibFilename)
	:BaseImageSourceEngine(calibFilename)
{
	data = new PrivateData();
	data->TCPConnect();
	//data->recvImgRGB = (uchar*)malloc(sizeof(uchar) * data->bufSize / 2);
	data->recvImgDepth = (uchar*)malloc(sizeof(uchar)* data->bufSize / 2);

	this->calib.disparityCalib.SetStandard();
	this->imageSize_d = Vector2i(640, 480);
	this->imageSize_rgb = Vector2i(640, 480);
	this->calib.intrinsics_d = this->calib.intrinsics_rgb;

}

Network2Engine::~Network2Engine()
{
	delete data;
}

bool Network2Engine::hasMoreImages() const
{
	return (data != NULL);
}

void Network2Engine::getImages(ORUChar4Image * rgbImage, ORShortImage * rawDepthImage)
{
	short* rawDepth = rawDepthImage->GetData(MEMORYDEVICE_CPU);
	Vector4u* rgb = rgbImage->GetData(MEMORYDEVICE_CPU);

	data->rec();

	for (size_t i = 0; i < rgbImage->noDims.height; i++)
	{
		for (size_t j = 0; j < rgbImage->noDims.width; j++)
		{
			unsigned int index = i * rgbImage->noDims.width + j;
			rgb[index].z = data->recvImgRGB[index * 3 + 0];
			rgb[index].y = data->recvImgRGB[index * 3 + 1];
			rgb[index].x = data->recvImgRGB[index * 3 + 2];
			rgb[index].w = 255;
		}
	}

	for (size_t i = 0; i < rawDepthImage->noDims.height; i++)
	{
		for (size_t j = 0; j < rawDepthImage->noDims.width; j++)
		{
			unsigned int index = i * rawDepthImage->noDims.width + j;
			rawDepth[index] = (data->recvImgDepth[index * 3 + 0] << 8) | data->recvImgDepth[index * 3 + 1];
		}
	}
}

Vector2i Network2Engine::getDepthImageSize() const
{
	return (data != NULL) ? imageSize_d : Vector2i(0, 0);
}

Vector2i Network2Engine::getRGBImageSize() const
{
	return (data != NULL) ? imageSize_rgb : Vector2i(0, 0);
}

#else
using namespace InputSource;


Network2Engine::Network2Engine(const char * calibFilename)
	:BaseImageSourceEngine(calibFilename)
{
	printf("compiled without network2 support\n");
}
Network2Engine::~Network2Engine()
{}
void Network2Engine::getImages(ORUChar4Image* rgbImage, ORShortImage* rawDepthImage)
{
	return;
}
bool Network2Engine::hasMoreImages() const
{
	return false;
}
Vector2i Network2Engine::getDepthImageSize() const
{
	return Vector2i(0, 0);
}
Vector2i Network2Engine::getRGBImageSize() const
{
	return Vector2i(0, 0);
}
#endif // COMPILE_WITH_NETWORK2

