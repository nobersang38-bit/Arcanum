//#include "Component/AwakenComponent.h"
//
//UAwakenComponent::UAwakenComponent()
//{
//	PrimaryComponentTick.bCanEverTick = false;
//}
//
//void UAwakenComponent::BeginPlay()
//{
//	Super::BeginPlay();
//
//	bLastReadyState = IsAwakenReady();
//}
//
//void UAwakenComponent::SetDataTables(UDataTable* InAwakenTable, UDataTable* InEffectTable, UDataTable* InTriggerTable)
//{
//	AwakenTable = InAwakenTable;
//	EffectTable = InEffectTable;
//	TriggerTable = InTriggerTable;
//}
//
//const FAwakenRow* UAwakenComponent::FindAwakenRow(const FGameplayTag& InAwakenTag) const
//{
//	if (!AwakenTable)
//	{
//		return nullptr;
//	}
//
//	if (!InAwakenTag.IsValid())
//	{
//		return nullptr;
//	}
//
//	const FName rowName(*InAwakenTag.ToString());
//	const FAwakenRow* row = AwakenTable->FindRow<FAwakenRow>(rowName, TEXT("FindAwakenRow"));
//
//	return row;
//}
//
//bool UAwakenComponent::SetCurrentAwakenTag(const FGameplayTag& InAwakenTag)
//{
//	const FAwakenRow* row = FindAwakenRow(InAwakenTag);
//	if (!row)
//	{
//		return false;
//	}
//
//	CurrentRow = row;
//
//	Runtime.AwakenTag = InAwakenTag;
//	Runtime.GaugeMax = row->GaugeMax;
//	Runtime.GaugeValue = 0.0f;
//	Runtime.bAwakened = false;
//
//	OnAwakenPassiveChanged.Broadcast(Runtime.AwakenTag, CurrentRow);
//	OnAwakenedChanged.Broadcast(Runtime.bAwakened);
//
//	BroadcastGauge();
//	BroadcastReadyIfChanged();
//
//	return true;
//}
//
//void UAwakenComponent::BroadcastGauge() const
//{
//	OnAwakenGaugeChanged.Broadcast(Runtime.AwakenTag, Runtime.GaugeValue, Runtime.GaugeMax);
//}
//
//bool UAwakenComponent::IsAwakenReady() const
//{
//	if (!CurrentRow)
//	{
//		return false;
//	}
//
//	if (Runtime.bAwakened)
//	{
//		return false;
//	}
//
//	if (Runtime.GaugeMax <= 0.0f)
//	{
//		return false;
//	}
//
//	return (Runtime.GaugeValue >= Runtime.GaugeMax);
//}
//
//void UAwakenComponent::BroadcastReadyIfChanged()
//{
//	const bool ready = IsAwakenReady();
//	if (ready != bLastReadyState)
//	{
//		bLastReadyState = ready;
//		OnAwakenReadyChanged.Broadcast(ready);
//	}
//}
//
//void UAwakenComponent::AddGaugeInternal(float InDelta)
//{
//	if (!CurrentRow)
//	{
//		return;
//	}
//
//	float newValue = Runtime.GaugeValue + InDelta;
//
//	if (newValue < 0.0f)
//	{
//		newValue = 0.0f;
//	}
//
//	if (newValue > Runtime.GaugeMax)
//	{
//		newValue = Runtime.GaugeMax;
//	}
//
//	if (newValue != Runtime.GaugeValue)
//	{
//		Runtime.GaugeValue = newValue;
//		BroadcastGauge();
//		BroadcastReadyIfChanged();
//	}
//}
//
//void UAwakenComponent::ResetGaugeInternal()
//{
//	if (!CurrentRow)
//	{
//		return;
//	}
//
//	if (Runtime.GaugeValue != 0.0f)
//	{
//		Runtime.GaugeValue = 0.0f;
//		BroadcastGauge();
//		BroadcastReadyIfChanged();
//	}
//}
//
///*
//bool UAwakenComponent::GainGaugeOnHit()
//{
//	if (!CurrentRow)
//	{
//		return false;
//	}
//
//	if (CurrentRow->GaugeGainHit == 0.0f)
//	{
//		return true;
//	}
//
//	AddGaugeInternal(CurrentRow->GaugeGainHit);
//	return true;
//}
//
//bool UAwakenComponent::AddGauge(float InDelta)
//{
//	if (!CurrentRow)
//	{
//		return false;
//	}
//
//	AddGaugeInternal(InDelta);
//	return true;
//}
//
//void UAwakenComponent::ResetGauge()
//{
//	ResetGaugeInternal();
//}
//
//bool UAwakenComponent::RequestAwaken()
//{
//	if (!IsAwakenReady())
//	{
//		return false;
//	}
//
//	Runtime.bAwakened = true;
//	OnAwakenedChanged.Broadcast(true);
//
//	// 각성 버튼 눌러서 각성 진입하면 게이지는 0으로 리셋(원하면 DT/룰로 분리)
//	ResetGaugeInternal();
//
//	return true;
//}
//
//*/
//
//void UAwakenComponent::ForceSetAwakened(bool bInAwakened)
//{
//	if (Runtime.bAwakened != bInAwakened)
//	{
//		Runtime.bAwakened = bInAwakened;
//		OnAwakenedChanged.Broadcast(Runtime.bAwakened);
//		BroadcastReadyIfChanged();
//	}
//}
//
//FGameplayTag UAwakenComponent::GetCurrentStackDebuffTag() const
//{
//	if (!CurrentRow)
//	{
//		return FGameplayTag();
//	}
//
//	return CurrentRow->StackDebuffTag;
//}
//
//const TArray<FName>& UAwakenComponent::GetPassiveEffectIDs() const
//{
//	static const TArray<FName> empty;
//
//	if (!CurrentRow)
//	{
//		return empty;
//	}
//
//	return CurrentRow->PassiveEffectIDs;
//}
//
//const TArray<FName>& UAwakenComponent::GetAuraEffectIDs() const
//{
//	static const TArray<FName> empty;
//
//	if (!CurrentRow)
//	{
//		return empty;
//	}
//
//	return CurrentRow->AuraEffectIDs;
//}
//
//const TArray<FName>& UAwakenComponent::GetTriggerIDs() const
//{
//	static const TArray<FName> empty;
//
//	if (!CurrentRow)
//	{
//		return empty;
//	}
//
//	return CurrentRow->TriggerIDs;
//}
//
//void UAwakenComponent::NotifyHit(const FSkillContext& InContext, AActor* InVictim, bool bInInstigatorIsPlayer, bool bInVictimIsAlly)
//{
//	if (!CurrentRow)
//	{
//		return;
//	}
//
//	// 각성 전: 플레이어만 패시브 발동(아군 제외)
//	// 각성 후: 오라 상태라서 아군도 디버프 검사/적용 가능(실제 적용은 Effect 실행부가 함)
//	if (!Runtime.bAwakened)
//	{
//		if (!bInInstigatorIsPlayer)
//		{
//			return;
//		}
//	}
//
//	// 적중 게이지 획득
//	// GainGaugeOnHit();
//
//	// TODO: EffectSubsystem/StatsComponent로 ApplyDebuff(8초 Refresh) 실행 호출
//	//InContext;
//}
//
//void UAwakenComponent::NotifyAllyDamaged(AActor* InAlly, AActor* InAttacker, bool bInAllyIsPlayer)
//{
//	if (!CurrentRow)
//	{
//		return;
//	}
//
//	// 결의(가시)는 "아군이 맞을 때 공격자에게 스택 디버프" 케이스
//	// 각성 전: 플레이어만 패시브 발동 -> "플레이어가 맞을 때만" 처리
//	// 각성 후: 오라 -> 아군도 포함 가능
//	if (!Runtime.bAwakened)
//	{
//		if (!bInAllyIsPlayer)
//		{
//			return;
//		}
//	}
//
//	// TODO: 공격자(InAttacker)에게 StackDebuffTag 적용(8초 Refresh, MaxStack)
//	(void)InAlly;
//	(void)InAttacker;
//}
//
//void UAwakenComponent::NotifyWeaponSkillUsed(const FSkillContext& InContext)
//{
//	if (!CurrentRow)
//	{
//		return;
//	}
//
//	// 무기 스킬 사용 시 트리거 검사(혈폭/응보/처단/정화)
//	TryTriggerByWeaponSkill(InContext);
//}
//
//void UAwakenComponent::TryTriggerByWeaponSkill(const FSkillContext& InContext)
//{
//	if (!TriggerTable)
//	{
//		return;
//	}
//
//	if (!CurrentRow)
//	{
//		return;
//	}
//
//	// TriggerIDs에 있는 트리거를 순회하면서
//	// ConditionDebuffTag가 "최대 스택"이면 발동
//
//	for (const FName& triggerId : CurrentRow->TriggerIDs)
//	{
//		const FTriggerEffectRow* triggerRow = TriggerTable->FindRow<FTriggerEffectRow>(triggerId, TEXT("TryTriggerByWeaponSkill"));
//		if (!triggerRow)
//		{
//			continue;
//		}
//
//		// TODO: 아래 2개는 외부 시스템(디버프/스탯) 연동이 필요
//		// 1) triggerRow->ConditionDebuffTag 가 현재 대상(또는 자기)에 "MaxStack"인지 검사
//		// 2) 맞으면 triggerRow->EffectIDs 를 EffectSubsystem로 실행
//		// (void)InContext;
//	}
//}
//
//void UAwakenComponent::ResetConditionDebuffStacks(const FGameplayTag& InDebuffTag)
//{
//	if (!InDebuffTag.IsValid())
//	{
//		return;
//	}
//
//	// 트리거 발동 직후 조건 디버프 스택을 0으로 만드는 처리.
//	// TODO: Debuff/Stats 시스템(예: UStatsComponent)에 "Remove/ResetStack" API 붙여서 여기서 호출
//	// 예시(가상의 함수):
//	// StatsComp->ResetDebuffStacks(InDebuffTag);
//
//	(void)InDebuffTag;
//}
