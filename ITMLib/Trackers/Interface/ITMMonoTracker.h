// Copyright 2019 DoubleYoung.me

#pragma once

#include "ITMTracker.h"

//#include 


namespace ITMLib
{
	/** Base class for engine performing libviso2 based RGB tracking.
	http://www.cvlibs.net/software/libviso/
	*/

	class ITMMonoTracker : public ITMTracker
	{

	private:

	protected:

		

	public:
		//virtual void TrackCamera(ITMTrackingState *trackingState, const ITMView *view) = 0;

		ITMMonoTracker(Vector2i imgSize_d,
			Vector2i imgSize_rgb,
			double focalLength,
			double cx,
			double cy);

		bool requiresColourRendering() const { return false; }
		bool requiresDepthReliability() const { return true; }
		bool requiresPointCloudRendering() const { return true; }
	};
}