// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CXItemBase.h"
#include "CXCoin.generated.h"

UCLASS()
class CX_API ACXCoin : public ACXItemBase
{
	GENERATED_BODY()

protected:
	virtual void ApplyEffect(ACXPlayerState* PlayerState) override;

	UPROPERTY(EditDefaultsOnly, Category = Item)
	int32 CoinAmount = 10;
};