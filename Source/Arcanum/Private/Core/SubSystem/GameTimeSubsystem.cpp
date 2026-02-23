#include "Core/SubSystem/GameTimeSubsystem.h"

void UGameTimeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UGameTimeSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UGameTimeSubsystem::Tick(float DeltaTime)
{
	if (bBannerActive) {
		FDateTime CurrentTimeKST = FDateTime::UtcNow() + FTimespan(9, 0, 0);
		FTimespan Diff = BannerEndTime - CurrentTimeKST;

		int32 RemainingSeconds = FMath::Max(0, static_cast<int32>(Diff.GetTotalSeconds()));
		if (RemainingSeconds != LastBannerSecond) {
			LastBannerSecond = RemainingSeconds;
			OnBannerSecondChanged.Broadcast(RemainingSeconds);

			if (RemainingSeconds <= 0) {
				StopBanner();
				// 여기에 GachaSubsystem->ProcessSettlement() 같은 정산 트리거를 연결하면 됨
			}
		}
	}

	if (bIsBattleActive) {
		CurrentStageTime -= DeltaTime;
		int32 DisplaySecond = FMath::Max(0, FMath::CeilToInt(CurrentStageTime));

		if (DisplaySecond != LastStageSecond) {
			LastStageSecond = DisplaySecond;
			OnStageSecondChanged.Broadcast(DisplaySecond);

			if (CurrentStageTime <= 0.f) {
				StopStage();
			}
		}
	}
}

TStatId UGameTimeSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UGameTimeSubsystem, STATGROUP_Tickables);
}

// ---------------------------
// Banner Logic
// ---------------------------
void UGameTimeSubsystem::StartBanner(FDateTime InEndTime)
{
	BannerEndTime = InEndTime;
	bBannerActive = true;
	LastBannerSecond = -1;
}
void UGameTimeSubsystem::StopBanner()
{
	bBannerActive = false;
}
// ---------------------------
// Stage Logic
// ---------------------------
void UGameTimeSubsystem::StartStage(float DurationSeconds)
{
	CurrentStageTime = DurationSeconds;
	bIsBattleActive = true;
	LastStageSecond = -1;
}
void UGameTimeSubsystem::StopStage()
{
	bIsBattleActive = false;
}