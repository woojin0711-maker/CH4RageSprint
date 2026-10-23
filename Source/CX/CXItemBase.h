// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CXItemBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UParticleSystem;
class USoundBase;
class ACXPlayerState;

UCLASS(Abstract)
class CX_API ACXItemBase : public AActor
{
	GENERATED_BODY()

public:
	ACXItemBase();

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// 자식 클래스가 덮어써서 효과를 정함
	virtual void ApplyEffect(ACXPlayerState* PlayerState) {}

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Collision;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditDefaultsOnly, Category = Effect)
	TObjectPtr<UParticleSystem> PickupParticle;

	UPROPERTY(EditDefaultsOnly, Category = Effect)
	TObjectPtr<USoundBase> PickupSound;
};

