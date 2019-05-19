#include <iostream>

#include "NetworkEngine.h"

#include "../ORUtils/FileUtils.h"

using namespace InputSource;

//class NetworkEngine::PrivateData
//{
//public:
//	PrivateData() {}
//	SOCKET clientSocket;
//};

NetworkEngine::NetworkEngine(const char * calibFilenamea)
	:BaseImageSourceEngine(calibFilenamea)
{

	TCHAR networkProcess[] = TEXT("C:\\Users\\server1\\Desktop\\InfiniTAM\\build\\x64\\Release\\NetworkProcess.exe");
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
	system("pause");

	this->calib.disparityCalib.SetStandard();
	this->imageSize_d = Vector2i(640, 480);
	this->imageSize_rgb = Vector2i(640, 480);
	this->calib.intrinsics_d = this->calib.intrinsics_rgb;
}

NetworkEngine::~NetworkEngine()
{

}

bool NetworkEngine::hasMoreImages() const
{
	return false;
}

void NetworkEngine::getImages(ORUChar4Image * rgbImage, ORShortImage * rawDepthImage)
{
	short* rawDepth = rawDepthImage->GetData(MEMORYDEVICE_CPU);
	Vector4u* rgb = rgbImage->GetData(MEMORYDEVICE_CPU);

	
}

Vector2i NetworkEngine::getDepthImageSize() const
{
	return Vector2i();
}

Vector2i NetworkEngine::getRGBImageSize() const
{
	return Vector2i();
}
