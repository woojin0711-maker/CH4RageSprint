// Copyright Epic Games, Inc. All Rights Reserved.

#include "CH4RageSprintWheelFront.h"
#include "UObject/ConstructorHelpers.h"

UCH4RageSprintWheelFront::UCH4RageSprintWheelFront()
{
	AxleType = EAxleType::Front;
	bAffectedBySteering = true;
	MaxSteerAngle = 40.f;
}