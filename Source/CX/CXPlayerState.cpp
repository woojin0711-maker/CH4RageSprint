// Fill out your copyright notice in the Description page of Project Settings.


#include "CXPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "CXPlayerController.h"

void ACXPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ThisClass, Coins, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ThisClass, BonusMaxSpeed, COND_OwnerOnly);
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

	PC->UpdateStatUI(Coins, BonusMaxSpeed);
}

void ACXPlayerState::OnRep_BonusMaxSpeed()
{
	RefreshOwnerUI();
}