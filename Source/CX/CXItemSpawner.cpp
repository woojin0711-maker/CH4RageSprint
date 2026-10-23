



#include "CXItemSpawner.h"
#include "CXItemBase.h"

ACXItemSpawner::ACXItemSpawner()
{
	bReplicates = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void ACXItemSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SpawnItem();
	}
}

void ACXItemSpawner::SpawnItem()
{
	if (ItemClass == nullptr)
	{
		return;
	}

	SpawnedItem = GetWorld()->SpawnActor<ACXItemBase>(ItemClass, GetActorTransform());
	if (IsValid(SpawnedItem))
	{
		SpawnedItem->OnDestroyed.AddDynamic(this, &ACXItemSpawner::OnItemDestroyed);
	}
}

void ACXItemSpawner::OnItemDestroyed(AActor* DestroyedActor)
{
	SpawnedItem = nullptr;
	GetWorldTimerManager().SetTimer(RespawnTimer, this, &ACXItemSpawner::SpawnItem, RespawnDelay, false);
}