// Fill out your copyright notice in the Description page of Project Settings.


#include "CXCoin.h"
#include "CXPlayerState.h"

void ACXCoin::ApplyEffect(ACXPlayerState* PlayerState)
{
	PlayerState->AddCoins(CoinAmount);
}