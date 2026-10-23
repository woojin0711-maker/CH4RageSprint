// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CXItemSpawner.generated.h"

class ACXItemBase;

UCLASS()
class CX_API ACXItemSpawner : public AActor
{
	GENERATED_BODY()

public:
	ACXItemSpawner();

protected:
	virtual void BeginPlay() override;

	void SpawnItem();

	UFUNCTION()
	void OnItemDestroyed(AActor* DestroyedActor);

	// 레벨에 놓은 스포너마다 따로 고를 수 있게 EditAnywhere
	UPROPERTY(EditAnywhere, Category = Spawner)
	TSubclassOf<ACXItemBase> ItemClass;

	UPROPERTY(EditAnywhere, Category = Spawner)
	float RespawnDelay = 2.f;

	UPROPERTY()
	TObjectPtr<ACXItemBase> SpawnedItem;

	FTimerHandle RespawnTimer;
};