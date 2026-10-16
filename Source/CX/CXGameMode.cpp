// Copyright Epic Games, Inc. All Rights Reserved.

#include "CXGameMode.h"
#include "CXPlayerController.h"

ACXGameMode::ACXGameMode()
{
	PlayerControllerClass = ACXPlayerController::StaticClass();
}
