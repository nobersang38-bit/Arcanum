#include "Component/Stats/PlayerBattleStatsComponent.h"

// ========================================================
// 브로드 캐스트
// ========================================================
void UPlayerBattleStatsComponent::NotifyRegenStatChanged(const FRegenStat& Stat)
{
	OnRegenStatChanged.Broadcast(Stat);
}
void UPlayerBattleStatsComponent::NotifyNonRegenStatChanged(const FNonRegenStat& Stat)
{
	OnNonRegenStatChanged.Broadcast(Stat);
}
// ========================================================
// 언리얼 기본 생성
// ========================================================
UPlayerBattleStatsComponent::UPlayerBattleStatsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}
void UPlayerBattleStatsComponent::BeginPlay()
{
	Super::BeginPlay();

    InitComponent();

    if (GetWorld()) GetWorld()->GetTimerManager().SetTimer(RegenTimerHandle, this, &UPlayerBattleStatsComponent::ProcessRegen, TimerTick, true);
}
void UPlayerBattleStatsComponent::ProcessRegen()
{
    for (FRegenStat& Stat : TotalRegenStats) {
        const float TotalMax = Stat.GetTotalMax();
        const float FinalTick = Stat.GetTotalTick();

        if (FMath::IsNearlyZero(FinalTick)) continue;

        if (FinalTick > 0.f) {
            if (Stat.Current >= TotalMax) continue;
            Stat.Current = FMath::Min(Stat.Current + FinalTick, TotalMax);
        }
        else Stat.Current = FMath::Max(Stat.Current + FinalTick, 0.f);

        NotifyRegenStatChanged(Stat);
    }
}
// ========================================================
// DataTable 관련
// ========================================================
#if WITH_EDITOR
void UPlayerBattleStatsComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    const FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
    const FName MemberPropertyName = (PropertyChangedEvent.MemberProperty != nullptr) ? PropertyChangedEvent.MemberProperty->GetFName() : NAME_None;
    if (MemberPropertyName == GET_MEMBER_NAME_CHECKED(UPlayerBattleStatsComponent, DTPlayerBattleRegenStatsRowHandle)) {
        if (DTPlayerBattleRegenStatsRowHandle.DataTable && !DTPlayerBattleRegenStatsRowHandle.RowName.IsNone()) {
            const FDTPlayerBattleRegenStatsRow* Row = DTPlayerBattleRegenStatsRowHandle.DataTable->FindRow<FDTPlayerBattleRegenStatsRow>(DTPlayerBattleRegenStatsRowHandle.RowName, TEXT("Editor StatsRegen Load"));

            if (Row) DTPlayerBattleRegenStatsRow = *Row;
        }
    }
    if (MemberPropertyName == GET_MEMBER_NAME_CHECKED(UPlayerBattleStatsComponent, DTPlayerBattleNonRegenStatsRowHandle)) {
        if (DTPlayerBattleNonRegenStatsRowHandle.DataTable && !DTPlayerBattleNonRegenStatsRowHandle.RowName.IsNone()) {
            const FDTPlayerBattleNonRegenStatsRow* Row = DTPlayerBattleNonRegenStatsRowHandle.DataTable->FindRow<FDTPlayerBattleNonRegenStatsRow>(DTPlayerBattleNonRegenStatsRowHandle.RowName, TEXT("Editor StatsRegen Load"));

            if (Row) DTPlayerBattleNonRegenStatsRow = *Row;
        }
    }

    if (PropertyName == GET_MEMBER_NAME_CHECKED(UPlayerBattleStatsComponent, DTPlayerBattleRegenStatsRow) || 
        PropertyName == GET_MEMBER_NAME_CHECKED(UPlayerBattleStatsComponent, DTPlayerBattleNonRegenStatsRow))
    {
        InitComponent();
    }
}
#endif
// ========================================================
// 초기화
// ========================================================
void UPlayerBattleStatsComponent::InitComponent()
{
    BaseRegenStats.Empty();
    for (TFieldIterator<FProperty> It(FDTPlayerBattleRegenStatsRow::StaticStruct()); It; ++It) {
        if (FStructProperty* StructProp = CastField<FStructProperty>(*It)) {
            if (StructProp->Struct == FRegenStat::StaticStruct()) {
                FRegenStat* StatPtr = StructProp->ContainerPtrToValuePtr<FRegenStat>(&DTPlayerBattleRegenStatsRow);
                if (StatPtr) {
                    StatPtr->InitializeTags();
                    BaseRegenStats.Add(*StatPtr);
                }
            }
        }
    }
    BaseNonRegenStats.Empty();
    for (TFieldIterator<FProperty> It(FDTPlayerBattleNonRegenStatsRow::StaticStruct()); It; ++It) {
        if (FStructProperty* StructProp = CastField<FStructProperty>(*It)) {
            if (StructProp->Struct == FNonRegenStat::StaticStruct()) {
                FNonRegenStat* StatPtr = StructProp->ContainerPtrToValuePtr<FNonRegenStat>(&DTPlayerBattleNonRegenStatsRow);
                if (StatPtr) {
                    BaseNonRegenStats.Add(*StatPtr);
                }
            }
        }
    }

    RebuildTotalStats();
    bInitialized = true;
}
void UPlayerBattleStatsComponent::AddRegenBonus(const TArray<FRegenStat>& InBonusStats)
{
    if (InBonusStats.Num() == 0) return;

    for (const FRegenStat& Incoming : InBonusStats) {
        FRegenStat* Found = BounsRegenStats.FindByPredicate([&](const FRegenStat& S) { return S.ParentTag == Incoming.ParentTag; });

        if (Found) Found->AddFrom(Incoming);
        else BounsRegenStats.Add(Incoming);
    }

    RebuildTotalStats();
}
void UPlayerBattleStatsComponent::AddNonRegenBonus(const TArray<FNonRegenStat>& InBonusStats)
{
    if (InBonusStats.Num() == 0) return;
    if (BounsNonRegenStats.Num() == 0) BounsNonRegenStats = InBonusStats;
    else {
        const int32 Count = FMath::Min(BounsNonRegenStats.Num(), InBonusStats.Num());

        for (int32 i = 0; i < Count; i++) {
            BounsNonRegenStats[i].AddFrom(InBonusStats[i]);
        }
    }

    RebuildTotalStats();
}
void UPlayerBattleStatsComponent::RebuildTotalStats()
{
    TotalRegenStats = BaseRegenStats;
    for (int32 i = 0; i < TotalRegenStats.Num(); i++) {
        if (BounsRegenStats.IsValidIndex(i)) {
            TotalRegenStats[i].AddFrom(BounsRegenStats[i]);
        }
    }
    TotalNonRegenStats = BaseNonRegenStats;
    for (int32 i = 0; i < TotalNonRegenStats.Num(); i++) {
        if (BounsNonRegenStats.IsValidIndex(i)) {
            TotalNonRegenStats[i].AddFrom(BounsNonRegenStats[i]);
        }
    }

    for (FRegenStat& Stat : TotalRegenStats) {
        Stat.Current = FMath::Clamp(Stat.Current, 0.f, Stat.GetTotalMax());
    }
}


void UPlayerBattleStatsComponent::ChangeStatValue(FGameplayTag StatTag, float DamageAmount, AActor* DamageCauser)
{
    for (FRegenStat& Stat : TotalRegenStats) {
        if (Stat.ParentTag == StatTag) {
            const float OldValue = Stat.Current;
            const float TotalMax = Stat.GetTotalMax();

            Stat.Current = FMath::Clamp(Stat.Current + DamageAmount, 0.f, TotalMax);

            if (DamageCauser) LastDamageCauser = DamageCauser;
            if (!FMath::IsNearlyEqual(OldValue, Stat.Current)) NotifyRegenStatChanged(Stat);
            return;
        }
    }

    for (FNonRegenStat& Stat : TotalNonRegenStats) {
        if (Stat.TagName == StatTag) {
            const float OldValue = Stat.BaseValue;
            Stat.BaseValue = FMath::Max(Stat.BaseValue + DamageAmount, 0.f);

            if (DamageCauser) LastDamageCauser = DamageCauser;
            if (!FMath::IsNearlyEqual(OldValue, Stat.BaseValue)) NotifyNonRegenStatChanged(Stat);
            return;
        }
    }
}