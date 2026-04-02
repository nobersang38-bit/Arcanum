#include "Component/Stats/CharacterBattleStatsComponent.h"
#include "DataInfo/BattleCharacter/BattleStats/DataTable/DTBattleStats.h"

// ========================================================
// 브로드 캐스트
// ========================================================
void UCharacterBattleStatsComponent::NotifyRegenStatChanged(const FRegenStat& Stat)
{
	OnCharacterRegenStatChanged.Broadcast(Stat);
	NotifyStatChanged(Stat, FNonRegenStat());
}
void UCharacterBattleStatsComponent::NotifyNonRegenStatChanged(const FNonRegenStat& Stat)
{
	OnCharacterNonRegenStatChanged.Broadcast(Stat);
	NotifyStatChanged(FRegenStat(), Stat);
}
void UCharacterBattleStatsComponent::NotifyStatChanged(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat)
{
	OnCharacterStatChanged.Broadcast(RegenStat, NonRegenStat);
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

	if (DTBattleStatsRow.DataTable && !DTBattleStatsRow.RowName.IsNone()) {
		const FDTBattleStatsContainerRow* Row = DTBattleStatsRow.DataTable->FindRow<FDTBattleStatsContainerRow>(DTBattleStatsRow.RowName, TEXT("Editor StatsRegen Load"));

		if (Row && !Row->GradeDataSteps.IsEmpty())
		{
			SetData((*Row).GradeDataSteps[0]);
		}
	}

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(RegenTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(RegenTimerHandle, this, &UCharacterBattleStatsComponent::ProcessRegen, TimerTick, true);
	}
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
	if (MemberPropertyName == GET_MEMBER_NAME_CHECKED(UCharacterBattleStatsComponent, DTBattleStatsRow)) {
		if (DTBattleStatsRow.DataTable && !DTBattleStatsRow.RowName.IsNone()) {
			const FDTBattleStatsContainerRow* Row = DTBattleStatsRow.DataTable->FindRow<FDTBattleStatsContainerRow>(DTBattleStatsRow.RowName, TEXT("Editor StatsRegen Load"));

			if (!Row->GradeDataSteps.IsEmpty())
			{
				if (Row) GradeStatData = (*Row).GradeDataSteps[0];
			}
		}
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(UCharacterBattleStatsComponent, GradeStatData))
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
	for (int i = 0; i < GradeStatData.RegenStats.Num(); i++)
	{
		FRegenStat RegenStat = GradeStatData.RegenStats[i];
		RegenStat.InitializeTags();
		BaseRegenStats.Add(RegenStat);
	}

	BaseNonRegenStats.Empty();
	for (int i = 0; i < GradeStatData.NonRegenStats.Num(); i++)
	{
		const FNonRegenStat& NonRegenStat = GradeStatData.NonRegenStats[i];
		BaseNonRegenStats.Add(NonRegenStat);
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
void UCharacterBattleStatsComponent::BroadcastAllStats()
{
	for (const FRegenStat& Stat : TotalRegenStats)
	{
		NotifyRegenStatChanged(Stat);
	}
	for (const FNonRegenStat& Stat : TotalNonRegenStats)
	{
		NotifyNonRegenStatChanged(Stat);
	}
}
void UCharacterBattleStatsComponent::SetData(const FGradeStatData& InGradeStatData)
{
	GradeStatData = InGradeStatData;
	InitComponent();
}
void UCharacterBattleStatsComponent::SetCurrentValueMax()
{
	for (FRegenStat& Stat : TotalRegenStats)
	{
		const float MaxValue = Stat.GetTotalMax();
		if (FMath::IsNearlyEqual(Stat.Current, MaxValue)) continue;
		Stat.Current = MaxValue;
		NotifyRegenStatChanged(Stat);
	}
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
		//Stat.Current = Stat.GetTotalMax();
	}
}
void UCharacterBattleStatsComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(RegenTimerHandle);

		for (TPair<FGuid, FTimerHandle>& timerPair : ModifierTimers)
		{
			GetWorld()->GetTimerManager().ClearTimer(timerPair.Value);
		}
	}

	ModifierTimers.Empty();
	ActiveModifiers.Empty();

	Super::EndPlay(EndPlayReason);
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
				OnBuffUpdated.Broadcast(Modifier.SourceTag, 1.0f);

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
				OnBuffAdded.Broadcast(NewMod.SourceTag, 1.0f);
				return;
			}
			}
		}
	}

	FDerivedStatModifier NewMod = Modifier;
	NewMod.ModifierId = FGuid::NewGuid();
	ActiveModifiers.Add(NewMod);
	UpdateFinalStatValue(NewMod.StatTag);
	StartModifierTimer(NewMod);
	OnBuffAdded.Broadcast(NewMod.SourceTag, 1.0f);
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
		const FGameplayTag SourceTag = Mod.SourceTag;
		const float TotalDuration = Mod.Duration;
		const double StartTime = GetWorld()->GetTimeSeconds();

		FTimerHandle& NewHandle = ModifierTimers.FindOrAdd(ModifierId);
		GetWorld()->GetTimerManager().SetTimer(NewHandle, [WeakThis, ModifierId, StatTag, SourceTag, TotalDuration, StartTime]() {
			if (UCharacterBattleStatsComponent* StrongThis = WeakThis.Get()) {
				const double currentTime = StrongThis->GetWorld()->GetTimeSeconds();
				const float elapsedTime = static_cast<float>(currentTime - StartTime);
				const float remainTime = FMath::Max(TotalDuration - elapsedTime, 0.0f);
				const float durationPercent = (TotalDuration > 0.0f) ? (remainTime / TotalDuration) : 0.0f;

				StrongThis->OnBuffUpdated.Broadcast(SourceTag, durationPercent);

				if (remainTime <= 0.0f) {
					if (FTimerHandle* TimerHandle = StrongThis->ModifierTimers.Find(ModifierId))
					{
						StrongThis->GetWorld()->GetTimerManager().ClearTimer(*TimerHandle);
					}
					int32 Index = StrongThis->ActiveModifiers.IndexOfByPredicate([&](const FDerivedStatModifier& M) {
						return M.ModifierId == ModifierId;
						});

					if (Index != INDEX_NONE) {
						StrongThis->ActiveModifiers.RemoveAt(Index);
						StrongThis->ModifierTimers.Remove(ModifierId);
						StrongThis->UpdateFinalStatValue(StatTag);
						StrongThis->OnBuffRemoved.Broadcast(SourceTag);
					}
					return;
				}
			}
		}, 0.05f, true); // Mod.Duration, false);
	}
}
void UCharacterBattleStatsComponent::UpdateFinalStatValue(FGameplayTag Tag)
{
	FRegenStat* RStat = TotalRegenStats.FindByPredicate([&](const FRegenStat& S) { return Tag.MatchesTag(S.ParentTag); });
	if (RStat) {
		float B_Flat = 0.f, B_Mul = 0.f;
		float C_Flat = 0.f, C_Mul = 0.f;
		for (const auto& Mod : ActiveModifiers) {
			if (Mod.StatTag == Tag) {
				B_Flat += Mod.Value.Flat;
				B_Mul += Mod.Value.Mul;
			}
		}

		if (Tag == RStat->Child_Max) {

			// TODO : 김도현 출력 값이 더해져 더하는 부분 제거
			RStat->BonusMax = B_Flat /*+ (RStat->BaseMax * B_Mul)*/;
			RStat->ModifierMax = C_Flat/* + (RStat->BaseMax * C_Mul)*/;
			RStat->Current = FMath::Clamp(RStat->Current, 0.f, RStat->GetTotalMax());
		}
		else if (Tag == RStat->Child_Tick) {
			// TODD: Regen Tick Modifier에서 Mul 값이 반영되지 않아 ModifierTick 계산식 수정
			//RStat->ModifierTick = C_Flat;
			RStat->ModifierTick = B_Flat + (RStat->BaseTick * B_Mul);
		}

		NotifyRegenStatChanged(*RStat);
		return;
	}

	if (FNonRegenStat* NStat = TotalNonRegenStats.FindByPredicate([&](const FNonRegenStat& S) { return Tag.MatchesTag(S.TagName); })) {
		float B_Flat = 0.f, B_Mul = 0.f;
		float C_Flat = 0.f, C_Mul = 0.f;

		for (const auto& Mod : ActiveModifiers) {
			if (Mod.StatTag == Tag) { B_Flat += Mod.Value.Flat; B_Mul += Mod.Value.Mul; }
		}
		NStat->BonusValue = B_Flat + (NStat->BaseValue * B_Mul);
		//NStat->ModifierValue = C_Flat;
		NotifyNonRegenStatChanged(*NStat);
		return;
	}
}

const FRegenStat* UCharacterBattleStatsComponent::FindRegenStat(const FGameplayTag& InFindTag) const
{
	for (int i = 0; i < TotalRegenStats.Num(); i++)
	{
		if (TotalRegenStats[i].ParentTag == InFindTag)
		{
			return &TotalRegenStats[i];
		}
	}
	return nullptr;
}

const FNonRegenStat* UCharacterBattleStatsComponent::FindNonRegenStat(const FGameplayTag& InFindTag) const
{
	for (int i = 0; i < TotalNonRegenStats.Num(); i++)
	{
		if (TotalNonRegenStats[i].TagName == InFindTag)
		{
			return &TotalNonRegenStats[i];
		}
	}
	return nullptr;
}