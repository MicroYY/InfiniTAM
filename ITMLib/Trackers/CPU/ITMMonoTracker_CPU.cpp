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

	/*uchar* imageData = (uchar*)view->rgb->GetData(MEMORYDEVICE_CPU);
	cv::Mat m(720, 1280, CV_8UC3);
	memcpy(m.data, imageData, sizeof(uchar) * 1280 * 720);
	cv::imshow("", m);
	cv::waitKey(1);*/

}
