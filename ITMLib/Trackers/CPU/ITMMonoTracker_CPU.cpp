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
	viso = new VisualOdometryMono(this->param);
}

ITMLib::ITMMonoTracker_CPU::~ITMMonoTracker_CPU()
{
}

void ITMLib::ITMMonoTracker_CPU::TrackCamera(ITMTrackingState * trackingState, const ITMView * view)
{
}
