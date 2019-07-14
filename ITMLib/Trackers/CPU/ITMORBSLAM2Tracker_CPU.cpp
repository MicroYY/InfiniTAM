#include <time.h>
#include <chrono>

#include "ITMORBSLAM2Tracker_CPU.h"

ITMLib::ITMORBSLAM2Tracker_CPU::ITMORBSLAM2Tracker_CPU(Vector2i imgSize_d, Vector2i imgSize_rgb)
	:ITMORBSLAM2Tracker(imgSize_d, imgSize_rgb)
{
	SLAM = new ORB_SLAM2::System("D:\\TestingCode\\ORBSLAM24Windows\\Vocabulary\\ORBvoc.txt",
		"D:\\TestingCode\\ORBSLAM24Windows\\Examples\\Monocular\\MynteyeVGA.yaml", ORB_SLAM2::System::MONOCULAR, true);
}
ITMLib::ITMORBSLAM2Tracker_CPU::~ITMORBSLAM2Tracker_CPU()
{
	delete SLAM;
}

void ITMLib::ITMORBSLAM2Tracker_CPU::TrackCamera(ITMTrackingState * trackingState, const ITMView * view)
{
	if (!trackingState->HasValidPointCloud())
	{
		trackingState->trackerResult == ITMTrackingState::TRACKING_GOOD;
		return;
	}

	ORUChar4Image out(Vector2i(480, 640), true, false);
	out.SetFrom(view->rgb, ORUtils::MemoryBlock<Vector4u>::CUDA_TO_CPU);

	uchar* data = (uchar*)out.GetData(MEMORYDEVICE_CPU);
	cv::Mat imgRGB(480, 640, CV_8UC4, data);
	cv::Mat imgGray(480, 640, CV_8UC1);
	cv::cvtColor(imgRGB, imgGray, cv::COLOR_RGBA2GRAY);
	//cv::imshow("", imgGray);
	//cv::waitKey(1);

	cv::Mat transM = SLAM->TrackMonocular(imgGray, (double)clock() / CLOCKS_PER_SEC);
	std::cout << transM << std::endl;
	//std::cout << transM.type() << std::endl;
	if (!transM.data)
	{
		trackingState->trackerResult = ITMTrackingState::TRACKING_POOR;
		std::cout << "Reset tracking..." << std::endl;
		//trackingState->Reset();
		return;
	}
	else
	{
		Matrix4f transformation;
		transformation.m00 = transM.at<float>(0, 0); transformation.m10 = transM.at<float>(0, 1); transformation.m20 = transM.at<float>(0, 2); transformation.m30 = transM.at<float>(0, 3);
		transformation.m01 = transM.at<float>(1, 0); transformation.m11 = transM.at<float>(1, 1); transformation.m21 = transM.at<float>(1, 2); transformation.m31 = transM.at<float>(1, 3);
		transformation.m02 = transM.at<float>(2, 0); transformation.m12 = transM.at<float>(2, 1); transformation.m22 = transM.at<float>(2, 2); transformation.m32 = transM.at<float>(2, 3);
		transformation.m03 = transM.at<float>(3, 0); transformation.m13 = transM.at<float>(3, 1); transformation.m23 = transM.at<float>(3, 3); transformation.m33 = transM.at<float>(3, 3);
		//std::cout << transformation << std::endl;
		trackingState->trackerResult = ITMTrackingState::TRACKING_GOOD;
		trackingState->pose_d->SetM(transformation);
	}
	
}
