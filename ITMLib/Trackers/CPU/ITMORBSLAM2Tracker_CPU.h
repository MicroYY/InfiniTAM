// Copyright 2019 Doubleyoung.me

#pragma once

#include "../Interface/ITMORBSLAM2Tracker.h"

#include "system.h"

namespace ITMLib
{
	class ITMORBSLAM2Tracker_CPU :public ITMORBSLAM2Tracker
	{
	public:
		ITMORBSLAM2Tracker_CPU(Vector2i imgSize_d, Vector2i imgSize_rgb);
		~ITMORBSLAM2Tracker_CPU();

		void TrackCamera(ITMTrackingState* trackingState, const ITMView* view);

	private:
		ORB_SLAM2::System* SLAM;
	};
}