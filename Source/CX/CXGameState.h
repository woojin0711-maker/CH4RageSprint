// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CXGameState.generated.h"

UENUM(BlueprintType)
enum class ECXMatchPhase : uint8
{
	Waiting,
	Playing,
	Finished
};

/**
 * 모든 플레이어가 함께 보는 게임 진행 상태 (단계, 남은 시간, 우승자)
 */
UCLASS()
class CX_API ACXGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	// 서버(GameMode)만 호출
	void SetMatchPhase(ECXMatchPhase NewPhase);
	void SetRemainingTime(int32 NewTime);
	void SetWinners(const TArray<APlayerState*>& NewWinners);

	ECXMatchPhase GetMatchPhase() const { return MatchPhase; }
	int32 GetRemainingTime() const { return RemainingTime; }

	// 이 컴퓨터 플레이어의 화면 갱신
	void RefreshLocalUI();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_MatchState();

	UPROPERTY(ReplicatedUsing = OnRep_MatchState)
	ECXMatchPhase MatchPhase = ECXMatchPhase::Waiting;

	UPROPERTY(ReplicatedUsing = OnRep_MatchState)
	int32 RemainingTime = 0;

	UPROPERTY(ReplicatedUsing = OnRep_MatchState)
	TArray<TObjectPtr<APlayerState>> Winners;
};
