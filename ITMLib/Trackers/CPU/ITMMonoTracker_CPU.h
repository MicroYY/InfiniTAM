// Copyright 2019 DoubleYoung.me

#pragma once

#include "../Interface/ITMMonoTracker.h"
#include "libviso2/viso_mono.h"

namespace ITMLib
{
	class ITMMonoTracker_CPU :public ITMMonoTracker
	{
	public:
		ITMMonoTracker_CPU(Vector2i imgSize_d,
			Vector2i imgSize_rgb,
			double focalLength,
			double cx,
			double cy);
		~ITMMonoTracker_CPU();

		void TrackCamera(ITMTrackingState *trackingState, const ITMView *view);

	private:
		VisualOdometryMono::parameters param;
		VisualOdometryMono* viso;
	};
}