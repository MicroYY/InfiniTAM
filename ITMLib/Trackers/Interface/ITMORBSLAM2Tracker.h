// Copyright 2019 Doubleyoung.me

#pragma once

#include "ITMTracker.h"

namespace ITMLib
{
	/** Base class for engine performing ORB-SLAM2 based tracking.
	http://webdiis.unizar.es/~raulmur/orbslam/
	*/

	class ITMORBSLAM2Tracker :public ITMTracker
	{
	private:

	protected:

	public:
		ITMORBSLAM2Tracker(Vector2i imgSize_d, Vector2i imgSize_rgb);

		bool requiresColourRendering() const { return false; }
		bool requiresDepthReliability() const { return true; }
		bool requiresPointCloudRendering() const { return true; }
	};
}