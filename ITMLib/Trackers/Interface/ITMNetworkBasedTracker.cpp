// Copyright 2019 DoubleYoung.me

#include "ITMNetworkBasedTracker.h"

#include <iostream>

void ITMLib::ITMNetworkBasedTracker::TrackCamera(ITMTrackingState * trackingState, const ITMView * view)
{
	trackingState->trackerResult = ITMTrackingState::TRACKING_FAILED;

	memcpy(quaternionPose, lpbase, sizeof(float) * 7);
	
	float q0 = quaternionPose[3];
	float q1 = quaternionPose[0];
	float q2 = quaternionPose[1];
	float q3 = quaternionPose[2];
	transformation.m00 = 1 - 2 * q2 * q2 - 2 * q3 * q3;
	transformation.m10 = 2 * q1 * q2 - 2 * q0 * q3;
	transformation.m20 = 2 * q1 * q3 + 2 * q0 * q2;
	transformation.m30 = quaternionPose[4];

	transformation.m01 = 2 * q1 * q2 + 2 * q0 * q3;
	transformation.m11 = 1 - 2 * q1 * q1 - 2 * q3 * q3;
	transformation.m21 = 2 * q2 * q3 - 2 * q0 * q1;
	transformation.m31 = quaternionPose[5];

	transformation.m02 = 2 * q1 * q3 - 2 * q0 * q2;
	transformation.m12 = 2 * q2 * q3 + 2 * q0 * q1;
	transformation.m22 = 1 - 2 * q1 * q1 - 2 * q2 * q2;
	transformation.m32 = quaternionPose[6];

	transformation.m03 = 0;
	transformation.m13 = 0;
	transformation.m23 = 0;
	transformation.m33 = 1;

	trackingState->trackerResult = ITMTrackingState::TRACKING_GOOD;
	trackingState->pose_d->SetInvM(transformation);
	std::cout << transformation << std::endl;
	//printf("%f %f %f %f %f %f % f\n", quaternionPose[0], quaternionPose[1],
	//	quaternionPose[2], quaternionPose[3], quaternionPose[4], quaternionPose[5], quaternionPose[6]);
}
