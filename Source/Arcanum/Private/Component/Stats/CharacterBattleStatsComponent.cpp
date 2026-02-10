#include "Component/Stats/CharacterBattleStatsComponent.h"

// ========================================================
// 브로드 캐스트
// ========================================================
void UCharacterBattleStatsComponent::NotifyRegenStatChanged(const FRegenStat& Stat)
{
	OnCharacterRegenStatChanged.Broadcast(Stat);
}
void UCharacterBattleStatsComponent::NotifyNonRegenStatChanged(const FNonRegenStat& Stat)
{
	OnCharacterNonRegenStatChanged.Broadcast(Stat);
}
// ========================================================
// 언리얼 기본 생성
// ========================================================
UCharacterBattleStatsComponent::UCharacterBattleStatsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}
void UCharacterBattleStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	InitComponent();

	if (GetWorld()) GetWorld()->GetTimerManager().SetTimer(RegenTimerHandle, this, &UCharacterBattleStatsComponent::ProcessRegen, TimerTick, true);
}
void UCharacterBattleStatsComponent::ProcessRegen()
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
void UCharacterBattleStatsComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    const FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
    const FName MemberPropertyName = (PropertyChangedEvent.MemberProperty != nullptr) ? PropertyChangedEvent.MemberProperty->GetFName() : NAME_None;
    if (MemberPropertyName == GET_MEMBER_NAME_CHECKED(UCharacterBattleStatsComponent, DTCharacterBattleRegenStatsRowHandle)) {
        if (DTCharacterBattleRegenStatsRowHandle.DataTable && !DTCharacterBattleRegenStatsRowHandle.RowName.IsNone()) {
            const FDTCharacterBattleRegenStatsRow* Row = DTCharacterBattleRegenStatsRowHandle.DataTable->FindRow<FDTCharacterBattleRegenStatsRow>(DTCharacterBattleRegenStatsRowHandle.RowName, TEXT("Editor StatsRegen Load"));

            if (Row) DTCharacterBattleRegenStatsRow = *Row;
        }
    }
    if (MemberPropertyName == GET_MEMBER_NAME_CHECKED(UCharacterBattleStatsComponent, DTCharacterBattleNonRegenStatsRowHandle)) {
        if (DTCharacterBattleNonRegenStatsRowHandle.DataTable && !DTCharacterBattleNonRegenStatsRowHandle.RowName.IsNone()) {
            const FDTCharacterBattleNonRegenStatsRow* Row = DTCharacterBattleNonRegenStatsRowHandle.DataTable->FindRow<FDTCharacterBattleNonRegenStatsRow>(DTCharacterBattleNonRegenStatsRowHandle.RowName, TEXT("Editor StatsRegen Load"));

            if (Row) DTCharacterBattleNonRegenStatsRow = *Row;
        }
    }

    if (PropertyName == GET_MEMBER_NAME_CHECKED(UCharacterBattleStatsComponent, DTCharacterBattleRegenStatsRow) ||
        PropertyName == GET_MEMBER_NAME_CHECKED(UCharacterBattleStatsComponent, DTCharacterBattleNonRegenStatsRow))
    {
        InitComponent();
    }
}
#endif
// ========================================================
// 초기화
// ========================================================
void UCharacterBattleStatsComponent::InitComponent()
{
    BaseRegenStats.Empty();
    for (TFieldIterator<FProperty> It(FDTCharacterBattleRegenStatsRow::StaticStruct()); It; ++It) {
        if (FStructProperty* StructProp = CastField<FStructProperty>(*It)) {
            if (StructProp->Struct == FRegenStat::StaticStruct()) {
                FRegenStat* StatPtr = StructProp->ContainerPtrToValuePtr<FRegenStat>(&DTCharacterBattleRegenStatsRow);
                if (StatPtr) {
                    StatPtr->InitializeTags();
                    BaseRegenStats.Add(*StatPtr);
                }
            }
        }
    }
    BaseNonRegenStats.Empty();
    for (TFieldIterator<FProperty> It(FDTCharacterBattleNonRegenStatsRow::StaticStruct()); It; ++It) {
        if (FStructProperty* StructProp = CastField<FStructProperty>(*It)) {
            if (StructProp->Struct == FNonRegenStat::StaticStruct()) {
                FNonRegenStat* StatPtr = StructProp->ContainerPtrToValuePtr<FNonRegenStat>(&DTCharacterBattleNonRegenStatsRow);
                if (StatPtr) {
                    BaseNonRegenStats.Add(*StatPtr);
                }
            }
        }
    }

    RebuildTotalStats();
    bInitialized = true;
}
void UCharacterBattleStatsComponent::AddRegenBonus(const TArray<FRegenStat>& InBonusStats)
{
    if (InBonusStats.Num() == 0) return;

    for (const FRegenStat& Incoming : InBonusStats) {
        FRegenStat* Found = BounsRegenStats.FindByPredicate([&](const FRegenStat& S) { return S.ParentTag == Incoming.ParentTag; });

        if (Found) Found->AddFrom(Incoming);
        else BounsRegenStats.Add(Incoming);
    }

    RebuildTotalStats();
}
void UCharacterBattleStatsComponent::AddNonRegenBonus(const TArray<FNonRegenStat>& InBonusStats)
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
void UCharacterBattleStatsComponent::RebuildTotalStats()
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
// ========================================================
// 배틀 중 부를 함수
// ========================================================
void UCharacterBattleStatsComponent::ChangeStatValue(FGameplayTag StatTag, float DamageAmount, AActor* DamageCauser)
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
void UCharacterBattleStatsComponent::ApplyDurationModifier(const FDerivedStatModifier& Modifier)
{
    if (Modifier.SourceTag.IsValid()) {
        FDerivedStatModifier* Existing = ActiveModifiers.FindByPredicate([&](const FDerivedStatModifier& M) { return M.SourceTag == Modifier.SourceTag && M.StatTag == Modifier.StatTag; });

        if (Existing) {
            switch (Modifier.ModifierPolicy) {
            case EModifierStackPolicy::Refresh: {
                const bool bValueChanged = Existing->Value.Flat != Modifier.Value.Flat || Existing->Value.Mul != Modifier.Value.Mul;
                Existing->Value = Modifier.Value;
                Existing->Duration = Modifier.Duration;

                if (FTimerHandle* OldHandle = ModifierTimers.Find(Existing->ModifierId)) GetWorld()->GetTimerManager().ClearTimer(*OldHandle);
                StartModifierTimer(*Existing);

                if (bValueChanged) UpdateFinalStatValue(Modifier.StatTag);
                return;
            }
            case EModifierStackPolicy::Stack: {
                int32 CurrentStackCount = 0;
                for (const auto& M : ActiveModifiers) {
                    if (M.SourceTag == Modifier.SourceTag && M.StatTag == Modifier.StatTag) CurrentStackCount++;
                }

                if (CurrentStackCount >= Modifier.MaxStack) {
                    int32 OldestIndex = ActiveModifiers.IndexOfByPredicate([&](const FDerivedStatModifier& M) { return M.SourceTag == Modifier.SourceTag && M.StatTag == Modifier.StatTag; });
                    if (OldestIndex != INDEX_NONE) {
                        ActiveModifiers.RemoveAt(OldestIndex);
                    }
                }

                FDerivedStatModifier NewMod = Modifier;
                NewMod.ModifierId = FGuid::NewGuid();
                ActiveModifiers.Add(NewMod);
                UpdateFinalStatValue(NewMod.StatTag);
                StartModifierTimer(NewMod);
                return;
            }
            }
        }
    }

    FDerivedStatModifier NewMod = Modifier;
    ActiveModifiers.Add(NewMod);
    UpdateFinalStatValue(NewMod.StatTag);
    StartModifierTimer(NewMod);
}
// ========================================================
// 외부 호출 함수(GAS의 Duration 계열)
// ========================================================
void UCharacterBattleStatsComponent::StartModifierTimer(const FDerivedStatModifier& Mod)
{
    if (!Mod.bIsPermanent && Mod.Duration > 0.f) {
        TWeakObjectPtr<UCharacterBattleStatsComponent> WeakThis(this);
        const FGuid ModifierId = Mod.ModifierId;
        const FGameplayTag StatTag = Mod.StatTag;

        FTimerHandle& NewHandle = ModifierTimers.FindOrAdd(ModifierId);
        GetWorld()->GetTimerManager().SetTimer(NewHandle, [WeakThis, ModifierId, StatTag]() {
            if (UCharacterBattleStatsComponent* StrongThis = WeakThis.Get()) {
                int32 Index = StrongThis->ActiveModifiers.IndexOfByPredicate([&](const FDerivedStatModifier& M) {
                    return M.ModifierId == ModifierId;
                    });

                if (Index != INDEX_NONE) {
                    StrongThis->ActiveModifiers.RemoveAt(Index);
                    StrongThis->ModifierTimers.Remove(ModifierId);
                    StrongThis->UpdateFinalStatValue(StatTag);
                }
            }
            }, Mod.Duration, false);
    }
}
void UCharacterBattleStatsComponent::UpdateFinalStatValue(FGameplayTag Tag)
{
    FRegenStat* RStat = TotalRegenStats.FindByPredicate([&](const FRegenStat& S) { return Tag.MatchesTag(S.ParentTag); });
    if (RStat) {
        float B_Flat = 0.f, B_Mul = 0.f;
        float C_Flat = 0.f, C_Mul = 0.f;
        for (const auto& Mod : ActiveModifiers) {
            if (Mod.StatTag == Tag) { B_Flat += Mod.Value.Flat; B_Mul += Mod.Value.Mul; }
        }

        if (Tag == RStat->Child_Max) {
            RStat->BonusMax = B_Flat + (RStat->BaseMax * B_Mul);
            RStat->ModifierMax = C_Flat + (RStat->BaseMax * C_Mul);
            RStat->Current = FMath::Clamp(RStat->Current, 0.f, RStat->GetTotalMax());
        }
        else if (Tag == RStat->Child_Tick) {
            RStat->ModifierTick = C_Flat;
        }

        NotifyRegenStatChanged(*RStat);
        return;
    }

    if (FNonRegenStat* NStat = TotalNonRegenStats.FindByPredicate([&](const FNonRegenStat& S) { return S.TagName == Tag; })) {
        float B_Flat = 0.f, B_Mul = 0.f;
        float C_Flat = 0.f, C_Mul = 0.f;

        for (const auto& Mod : ActiveModifiers) {
            if (Mod.StatTag == Tag) { B_Flat += Mod.Value.Flat; B_Mul += Mod.Value.Mul; }
        }
        NStat->BonusValue = B_Flat + (NStat->BaseValue * B_Mul);

        NotifyNonRegenStatChanged(*NStat);
        return;
    }
}