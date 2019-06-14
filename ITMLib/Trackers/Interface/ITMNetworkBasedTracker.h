// Copyright 2019 DoubleYoung.me

#pragma once

#include "ITMTracker.h"

namespace ITMLib
{
	/**
	 * \brief Tracker that reads precomputed poses from network.
	 */
	class ITMNetworkBasedTracker :public ITMTracker
	{
	public:
		ITMNetworkBasedTracker()
		{
			hMapFilePose = OpenFileMapping(FILE_MAP_READ, FILE_MAP_READ, TEXT("pose"));
			lpbase = MapViewOfFile(hMapFilePose, FILE_MAP_READ, 0, 0, 0);
		}

		void TrackCamera(ITMTrackingState* trackingState, const ITMView* view);

		bool requiresColourRendering() const { return false; }
		bool requiresDepthReliability() const { return true; }
		bool requiresPointCloudRendering() const { return true; }


	private:
		size_t frameCount;

		HANDLE hMapFilePose;
		LPVOID lpbase;
		float quaternionPose[7];
		Matrix4f transformation;
	};
}