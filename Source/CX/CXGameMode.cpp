// CXGameMode.cpp

#include "CXGameMode.h"
#include "CXPlayerController.h"
#include "EngineUtils.h"
#include "Engine/World.h"
#include "GameFramework/PlayerStart.h"
#include "Subsystem/ChatSubsystem.h"
#include "CXPlayerState.h"
#include "CXGameState.h"

ACXGameMode::ACXGameMode()
{
	PlayerControllerClass = ACXPlayerController::StaticClass();
	PlayerStateClass = ACXPlayerState::StaticClass();
	GameStateClass = ACXGameState::StaticClass();
}

void ACXGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	if (IsValid(NewPlayer) == false)
	{
		return;
	}

	if (ACXPlayerController* CXPlayerController = Cast<ACXPlayerController>(NewPlayer))
	{
		// ���� ������ Ŭ��� ä�� �α׸� ������
		if (UChatSubsystem* ChatSubsystem = GetWorld()->GetSubsystem<UChatSubsystem>())
		{
			const TArray<FChatData> ChatLog = ChatSubsystem->GetChatLog();
			if (!ChatLog.IsEmpty())
			{
				CXPlayerController->STCChatLog(ChatLog);
			}
		}
	}

	// 첫 플레이어가 들어왔을 때만 대기 카운트다운 시작
	if (GetWorldTimerManager().IsTimerActive(MatchTimer) == false)
	{
		StartMatchTimer();
	}
}

AActor* ACXGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<APlayerStart*> AvailablePlayerStarts;

	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		APlayerStart* PlayerStart = *It;

		if (IsPlayerStartAvailable(PlayerStart))
		{
			AvailablePlayerStarts.Add(PlayerStart);
		}
	}

	checkf(AvailablePlayerStarts.IsEmpty() == false, TEXT("There is no available Vehicle PlayerStart."));

	const int32 RandomIndex = FMath::RandHelper(AvailablePlayerStarts.Num());
	return Cast<AActor>(AvailablePlayerStarts[RandomIndex]);
}

bool ACXGameMode::IsPlayerStartAvailable(const APlayerStart* PlayerStart) const
{
	if (IsValid(PlayerStart) == false || IsValid(GetWorld()) == false)
	{
		return false;
	}

	const FTransform PlayerStartTransform = PlayerStart->GetActorTransform();
	const FVector CheckLocation = PlayerStartTransform.TransformPosition(VehicleSpawnCheckOffset);
	const FQuat CheckRotation = PlayerStartTransform.GetRotation();
	const FCollisionShape CheckShape = FCollisionShape::MakeBox(VehicleSpawnCheckHalfExtent);
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Vehicle);

	const FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(VehiclePlayerStartCheck), false);

	const bool bVehicleExists =
		GetWorld()->OverlapAnyTestByObjectType(
			CheckLocation,
			CheckRotation,
			ObjectQueryParams,
			CheckShape,
			QueryParams
		);

	return !bVehicleExists;
}

void ACXGameMode::StartMatchTimer()
{
	ACXGameState* GS = GetGameState<ACXGameState>();
	if (IsValid(GS) == false)
	{
		return;
	}

	GS->SetMatchPhase(ECXMatchPhase::Waiting);
	GS->SetRemainingTime(WaitingTime);
	GetWorldTimerManager().SetTimer(MatchTimer, this, &ACXGameMode::OnMatchTick, 1.f, true);
}

void ACXGameMode::OnMatchTick()
{
	ACXGameState* GS = GetGameState<ACXGameState>();
	if (IsValid(GS) == false)
	{
		return;
	}

	const int32 NewTime = GS->GetRemainingTime() - 1;
	GS->SetRemainingTime(NewTime);

	if (NewTime > 0)
	{
		return;
	}

	if (GS->GetMatchPhase() == ECXMatchPhase::Waiting)
	{
		GS->SetMatchPhase(ECXMatchPhase::Playing);
		GS->SetRemainingTime(PlayingTime);
	}
	else if (GS->GetMatchPhase() == ECXMatchPhase::Playing)
	{
		FinishMatch();
	}
}

void ACXGameMode::FinishMatch()
{
	GetWorldTimerManager().ClearTimer(MatchTimer);

	ACXGameState* GS = GetGameState<ACXGameState>();
	if (IsValid(GS) == false)
	{
		return;
	}

	// -1에서 시작해야 모두 0원일 때도 공동 우승으로 잡힌다
	int32 BestCoins = -1;
	TArray<APlayerState*> Winners;

	for (APlayerState* PS : GS->PlayerArray)
	{
		ACXPlayerState* CXPS = Cast<ACXPlayerState>(PS);
		if (IsValid(CXPS) == false)
		{
			continue;
		}

		if (CXPS->GetCoins() > BestCoins)
		{
			BestCoins = CXPS->GetCoins();
			Winners.Reset();
			Winners.Add(CXPS);
		}
		else if (CXPS->GetCoins() == BestCoins)
		{
			Winners.Add(CXPS);
		}
	}

	// 우승자를 먼저 넣어야 Finished가 될 때 결과가 바로 맞게 나온다
	GS->SetWinners(Winners);
	GS->SetMatchPhase(ECXMatchPhase::Finished);
}
