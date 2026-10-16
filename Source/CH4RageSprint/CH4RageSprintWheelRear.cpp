// Copyright Epic Games, Inc. All Rights Reserved.

#include "CH4RageSprintWheelRear.h"
#include "UObject/ConstructorHelpers.h"

UCH4RageSprintWheelRear::UCH4RageSprintWheelRear()
{
	AxleType = EAxleType::Rear;
	bAffectedByHandbrake = true;
	bAffectedByEngine = true;
}