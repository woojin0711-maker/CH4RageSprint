// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CXPlayerState.generated.h"

/**
 *
 */
UCLASS()
class CX_API ACXPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	// 서버에서만 값이 바뀌어야 함
	void AddCoins(int32 Amount);
	bool RemoveCoins(int32 Amount);   // 부족하면 false, 공격 판정에서 사용
	void AddBonusMaxSpeed(float Amount);

	int32 GetCoins() const { return Coins; }
	float GetBonusMaxSpeed() const { return BonusMaxSpeed; }

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_Coins();

	UFUNCTION()
	void OnRep_BonusMaxSpeed();

private:
	// 로컬 플레이어라면 화면 UI를 갱신
	void RefreshOwnerUI();

	UPROPERTY(ReplicatedUsing = OnRep_Coins)
	int32 Coins = 0;

	UPROPERTY(ReplicatedUsing = OnRep_BonusMaxSpeed)
	float BonusMaxSpeed = 0.f;
};
