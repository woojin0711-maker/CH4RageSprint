// Fill out your copyright notice in the Description page of Project Settings.


#include "CXPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "CXPlayerController.h"

void ACXPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ThisClass, Coins, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ThisClass, BonusMaxSpeed, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ThisClass, SpeedPenalty, COND_OwnerOnly);
}

void ACXPlayerState::OnRep_Coins()
{
	RefreshOwnerUI();
}

void ACXPlayerState::AddCoins(int32 Amount)
{
	if (HasAuthority() == false || Amount <= 0)
	{
		return;
	}

	Coins += Amount;
	RefreshOwnerUI();
}

void ACXPlayerState::RefreshOwnerUI()
{
	ACXPlayerController* PC = Cast<ACXPlayerController>(GetPlayerController());
	if (IsValid(PC) == false || PC->IsLocalController() == false)
	{
		return;
	}

	PC->UpdateStatUI(Coins, GetBonusMaxSpeed());
}

void ACXPlayerState::OnRep_BonusMaxSpeed()
{
	RefreshOwnerUI();
}

bool ACXPlayerState::RemoveCoins(int32 Amount)
{
	if (HasAuthority() == false || Amount <= 0 || Coins < Amount)
	{
		return false;
	}

	Coins -= Amount;
	RefreshOwnerUI();
	return true;
}

void ACXPlayerState::AddBonusMaxSpeed(float Amount)
{
	if (HasAuthority() == false || Amount <= 0.f)
	{
		return;
	}
	BonusMaxSpeed += Amount;
	RefreshOwnerUI();
}

void ACXPlayerState::ApplySpeedPenalty(float Amount)
{
	if (HasAuthority() == false || Amount <= 0.f)
	{
		return;
	}

	SpeedPenalty -= Amount;
	RefreshOwnerUI();

	if (GetWorldTimerManager().IsTimerActive(PenaltyRecoveryTimer) == false)
	{
		GetWorldTimerManager().SetTimer(PenaltyRecoveryTimer, this, &ACXPlayerState::RecoverSpeedPenalty, 0.1f, true);
	}
}

void ACXPlayerState::RecoverSpeedPenalty()
{
	// 0을 넘어 양수가 되지 않게 Min으로 막는다
	SpeedPenalty = FMath::Min(SpeedPenalty + PenaltyRecoveryPerSecond * 0.1f, 0.f);
	RefreshOwnerUI();

	if (SpeedPenalty >= 0.f)
	{
		GetWorldTimerManager().ClearTimer(PenaltyRecoveryTimer);
	}
}