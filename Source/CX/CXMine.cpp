// Fill out your copyright notice in the Description page of Project Settings.


#include "CXMine.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"
#include "UObject/ConstructorHelpers.h"
#include "CXPawn.h"

ACXMine::ACXMine()
{
	bReplicates = true;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetSphereRadius(120.f);
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SetRootComponent(Collision);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Collision);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetRelativeScale3D(FVector(1.2f, 1.2f, 0.15f));

	// 블루프린트 없이도 바로 쓸 수 있게 StarterContent 에셋을 기본값으로 지정
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MineMesh(TEXT("/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder"));
	if (MineMesh.Succeeded())
	{
		Mesh->SetStaticMesh(MineMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> Particle(TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion"));
	if (Particle.Succeeded())
	{
		ExplosionParticle = Particle.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> Sound(TEXT("/Game/StarterContent/Audio/Explosion_Cue.Explosion_Cue"));
	if (Sound.Succeeded())
	{
		ExplosionSound = Sound.Object;
	}

	Collision->OnComponentBeginOverlap.AddDynamic(this, &ACXMine::OnOverlap);
}

void ACXMine::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority() == false)
	{
		return;
	}

	ACXPawn* Victim = Cast<ACXPawn>(OtherActor);
	if (IsValid(Victim) == false || Victim == GetOwner())
	{
		return;
	}

	Victim->BreakThrottle();
	Destroy();
}

void ACXMine::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// 서버에서 Destroy되면 클라이언트에서도 지워지면서 여기가 불린다
	if (EndPlayReason != EEndPlayReason::Destroyed)
	{
		return;
	}

	if (ExplosionParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, GetActorLocation());
	}

	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
	}
}
