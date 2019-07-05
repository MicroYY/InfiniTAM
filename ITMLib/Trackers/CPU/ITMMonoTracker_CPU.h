// Copyright 2019 DoubleYoung.me

#pragma once

#include <opencv2/opencv.hpp>

#include "../Interface/ITMMonoTracker.h"
#include "libviso2/viso_mono.h"


namespace ITMLib
{
	class ITMMonoTracker_CPU :public ITMMonoTracker
	{
	public:
		ITMMonoTracker_CPU(Vector2i imgSize_d,
			Vector2i imgSize_rgb,
			double focalLength = 518.52905273437500000,
			double cx = 319.55499267578125000,
			double cy = 239.06936645507812500);
		~ITMMonoTracker_CPU();

		void TrackCamera(ITMTrackingState *trackingState, const ITMView *view);

	private:
		VisualOdometryMono::parameters param;
		VisualOdometryMono* viso;
		Matrix pose;
	};
}