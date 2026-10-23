// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CXMine.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UParticleSystem;
class USoundBase;

/**
 * 차 뒤에 설치하는 지뢰. 다른 차가 밟으면 그 차의 가속 페달이 잠시 고장난다.
 * 설치한 차(Owner)는 밟아도 터지지 않는다.
 */
UCLASS()
class CX_API ACXMine : public AActor
{
	GENERATED_BODY()

public:
	ACXMine();

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Collision;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditDefaultsOnly, Category = Effect)
	TObjectPtr<UParticleSystem> ExplosionParticle;

	UPROPERTY(EditDefaultsOnly, Category = Effect)
	TObjectPtr<USoundBase> ExplosionSound;
};
