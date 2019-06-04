#include <iostream>


#include "NetworkEngine.h"

#include "../ORUtils/FileUtils.h"

#ifdef COMPILE_WITH_NETWORK

using namespace InputSource;

class NetworkEngine::PrivateData
{
public:
	PrivateData() { szName = "imageData"; }
	HANDLE hMapFile;
	LPVOID lpbase;
	TCHAR* szName;
};

NetworkEngine::NetworkEngine(const char * calibFilename)
	:BaseImageSourceEngine(calibFilename)
{
	TCHAR networkProcess[] = TEXT("..\\..\\x64\\Release\\NetworkProcess.exe");
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi;
	auto iRet = CreateProcess(networkProcess, NULL, NULL, NULL, false, NULL, NULL, NULL, &si, &pi);
	if (iRet)
	{
		std::cout << "Process started." << std::endl
			<< "Process ID:\t"
			<< pi.dwProcessId << std::endl;
	}
	else
	{
		std::cout << "Cannot start process!" << std::endl
			<< "Error code:\t" << GetLastError() << std::endl;
		return;
	}
	//system("pause");
	Sleep(2000);
	data = new PrivateData();
	data->hMapFile = OpenFileMapping(FILE_MAP_READ, FILE_MAP_READ, data->szName);
	data->lpbase = MapViewOfFile(data->hMapFile, FILE_MAP_READ, 0, 0, 0);

	this->calib.disparityCalib.SetStandard();
	this->imageSize_d = Vector2i(640, 480);
	this->imageSize_rgb = Vector2i(640, 480);
	this->calib.intrinsics_d = this->calib.intrinsics_rgb;
	 
}

NetworkEngine::~NetworkEngine()
{
	delete data;
}

bool NetworkEngine::hasMoreImages() const
{
	return (data != NULL);
}

void NetworkEngine::getImages(ORUChar4Image * rgbImage, ORShortImage * rawDepthImage)
{
	std::cout << "12345" << std::endl;
	short* rawDepth = rawDepthImage->GetData(MEMORYDEVICE_CPU);
	Vector4u* rgb = rgbImage->GetData(MEMORYDEVICE_CPU);
	std::cout << "12345" << std::endl;
	//Vector2i noDims = rawDepthImage->noDims;

	uchar* imageColor = (uchar*)malloc(sizeof(uchar) * 640 * 480 * 3);
	memcpy(imageColor, (uchar*)data->lpbase, sizeof(uchar) * 640 * 480 * 3);
	std::cout << "12345" << std::endl;
	uchar* imageDepth = (uchar*)malloc(sizeof(uchar) * 640 * 480 * 3);
	memcpy(imageDepth, (uchar*)data->lpbase + 640 * 480 * 3, sizeof(uchar) * 640 * 480 * 3);
	
	for (size_t i = 0; i < rgbImage->noDims.height; i++)
	{
		for (size_t j = 0; j < rgbImage->noDims.width; j++)
		{
			unsigned int index = i * rgbImage->noDims.width + j;
			rgb[index].z = imageColor[index * 3 + 0];
			rgb[index].y = imageColor[index * 3 + 1];
			rgb[index].x = imageColor[index * 3 + 2];
			rgb[index].w = 255;
		}
	}
	
	for (size_t i = 0; i < rawDepthImage->noDims.height; i++)
	{
		for (size_t j = 0; j < rawDepthImage->noDims.width; j++)
		{
			unsigned int index = i * rawDepthImage->noDims.width + j;
			rawDepth[index] = (imageDepth[index * 3 + 0] << 8) | imageDepth[index * 3 + 1];
		}
	}
	free(imageColor);
	free(imageDepth);
	
}

Vector2i NetworkEngine::getDepthImageSize() const
{
	return (data != NULL) ? imageSize_d : Vector2i(0, 0);
}

Vector2i NetworkEngine::getRGBImageSize() const
{
	return (data != NULL) ? imageSize_rgb : Vector2i(0, 0);
}

#else
using namespace InputSource;

NetworkEngine::NetworkEngine(const char * calibFilename)
	:BaseImageSourceEngine(calibFilename)
{
	printf("compiled without network support\n");
}
NetworkEngine::~NetworkEngine()
{}
void NetworkEngine::getImages(ORUChar4Image* rgbImage, ORShortImage* rawDepthImage)
{
	return;
}
bool NetworkEngine::hasMoreImages() const
{
	return false;
}
Vector2i NetworkEngine::getDepthImageSize() const
{
	return Vector2i(0, 0);
}
Vector2i NetworkEngine::getRGBImageSize() const
{
	return Vector2i(0, 0);
}

#endif // COMPILE_WITH_NETWORK