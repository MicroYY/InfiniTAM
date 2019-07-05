#include "ITMMonoTracker_CPU.h"

ITMLib::ITMMonoTracker_CPU::ITMMonoTracker_CPU(Vector2i imgSize_d, Vector2i imgSize_rgb, double focalLength, double cx, double cy)
	:ITMMonoTracker(imgSize_d,
		imgSize_rgb,
		focalLength,
		cx,
		cy)
{
	this->param.calib.f = focalLength;
	this->param.calib.cu = cx;
	this->param.calib.cv = cy;
	this->viso = new VisualOdometryMono(this->param);

	pose = Matrix::eye(4);
}

ITMLib::ITMMonoTracker_CPU::~ITMMonoTracker_CPU()
{
	delete this->viso;
}

void ITMLib::ITMMonoTracker_CPU::TrackCamera(ITMTrackingState * trackingState, const ITMView * view)
{
	if (!trackingState->HasValidPointCloud())
	{
		trackingState->trackerResult = ITMTrackingState::TRACKING_GOOD;
		return;
	}

	ORUChar4Image out(Vector2i(480, 640), true, false);
	out.SetFrom(view->rgb, ORUtils::MemoryBlock<Vector4u>::CUDA_TO_CPU);

	uchar* data = (uchar*)out.GetData(MEMORYDEVICE_CPU);
	cv::Mat imgRGB(480, 640, CV_8UC4, data);
	cv::Mat imgGray(480, 640, CV_8UC1);
	cv::cvtColor(imgRGB, imgGray, cv::COLOR_RGBA2GRAY);
	cv::imshow("", imgGray);
	cv::waitKey(1);

	int32_t width = 640;
	int32_t height = 480;

	uint8_t* img_data = (uint8_t*)malloc(width * height * sizeof(uint8_t));
	memcpy(img_data, imgGray.data, width* height * sizeof(uint8_t));

	int32_t dims[]{ width,height,width };
	std::cout << pose << std::endl << std::endl;
	
	Matrix4f transformation;
	if (viso->process(img_data, dims))
	{
		pose = pose * Matrix::inv(viso->getMotion());
		double num_matches = viso->getNumberOfMatches();
		double num_inliers = viso->getNumberOfInliers();
		std::cout << ", Matches: " << num_matches;
		std::cout << ", Inliers: " << 100.0*num_inliers / num_matches << " %" << ", Current pose: " << std::endl;
		std::cout << pose << std::endl << std::endl;
		transformation.m00 = pose.val[0][0]; transformation.m10 = pose.val[0][1]; transformation.m20 = pose.val[0][2]; transformation.m30 = pose.val[0][3];
		transformation.m01 = pose.val[1][0]; transformation.m11 = pose.val[1][1]; transformation.m21 = pose.val[1][2]; transformation.m31 = pose.val[1][3];
		transformation.m02 = pose.val[2][0]; transformation.m12 = pose.val[2][1]; transformation.m22 = pose.val[2][2]; transformation.m32 = pose.val[2][3];
		transformation.m03 = 0; transformation.m13 = 0; transformation.m23 = 0; transformation.m33 = 1;
	}
	else
	{
		std::cout << " ... failed!" << std::endl;
		pose = Matrix::eye(4);
		transformation.setIdentity();
	}
	trackingState->trackerResult = ITMTrackingState::TRACKING_GOOD;
	trackingState->pose_d->SetInvM(transformation);
	std::cout << transformation << std::endl;
	free(img_data);
}
