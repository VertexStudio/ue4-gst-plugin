
#pragma once

#include "SharedUnreal.h"

enum class EGstKvlKey : uint8
{
	OdometryPosePosition 	= 10,
	OdometryPoseOrientation = 11,
	OdometryTwistLinear 	= 12,
	OdometryTwistAngular 	= 13,
	None
};

struct FGstKlv
{
	// GENERATED_USTRUCT_BODY()

	EGstKvlKey Key;

	double V0;
	double V1;
	double V2;
	double V3;
};
