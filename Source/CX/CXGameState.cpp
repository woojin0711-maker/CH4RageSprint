// Fill out your copyright notice in the Description page of Project Settings.


#include "CXGameState.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerState.h"
#include "CXPlayerController.h"

void ACXGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, MatchPhase);
	DOREPLIFETIME(ThisClass, RemainingTime);
	DOREPLIFETIME(ThisClass, Winners);
}

void ACXGameState::SetMatchPhase(ECXMatchPhase NewPhase)
{
	if (HasAuthority() == false)
	{
		return;
	}

	MatchPhase = NewPhase;
	RefreshLocalUI();
}

void ACXGameState::SetRemainingTime(int32 NewTime)
{
	if (HasAuthority() == false)
	{
		return;
	}

	RemainingTime = NewTime;
	RefreshLocalUI();
}

void ACXGameState::SetWinners(const TArray<APlayerState*>& NewWinners)
{
	if (HasAuthority() == false)
	{
		return;
	}

	Winners.Reset();
	for (APlayerState* Winner : NewWinners)
	{
		Winners.Add(Winner);
	}
	RefreshLocalUI();
}

void ACXGameState::OnRep_MatchState()
{
	RefreshLocalUI();
}

void ACXGameState::RefreshLocalUI()
{
	// 서버에는 남의 컨트롤러도 있어서 "이 컴퓨터에서 조작하는" 컨트롤러만 찾는다
	ACXPlayerController* PC = Cast<ACXPlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
	if (IsValid(PC) == false)
	{
		return;
	}

	PC->UpdateMatchUI(MatchPhase, RemainingTime);

	if (MatchPhase == ECXMatchPhase::Finished)
	{
		const bool bIsWinner = Winners.Contains(PC->PlayerState);
		PC->ShowMatchResult(bIsWinner);
	}
}
