#include "Object/Object/StatusAction/StatusAction_SetBonus.h"
#include "Core/SubSystem/BattlefieldManagerSubsystem.h"
#include "Component/Stats/CharacterBattleStatsComponent.h"
#include "GameFramework/Character.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "GameplayTags/ArcanumTags.h"

UStatusAction_SetBonus::UStatusAction_SetBonus()
{
	EnableTagMergeType = EStatusActionTagType::OnlyThisTags;
}

void UStatusAction_SetBonus::InitializeAction_Implementation(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat)
{
	Super::InitializeAction_Implementation(RegenStat, NonRegenStat);

	if (UBattlefieldManagerSubsystem* battleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>())
	{
		SetBonusTag = battleSubsystem->GetEquippedSetBonusTag();
		UE_LOG(LogTemp, Warning, TEXT("SetBonusTag = %s"), *battleSubsystem->GetEquippedSetBonusTag().ToString());

		bIsBonusActive = false;
		bIsCooldown = false;

		ChangeEnableTags(Arcanum::BattleStat::Character::Regen::Health::Root);
		ChangeDisableTags(Arcanum::BattleStat::Character::Regen::Health::Root);
		
		if (SetBonusTag == Arcanum::Items::SetBonus::Surge)
		{
			SetBonusModifier = FDerivedStatModifier();
			SetBonusModifier.SourceTag = SetBonusTag;
			SetBonusModifier.StatTag = Arcanum::BattleStat::Character::Regen::Health::Regen;
			SetBonusModifier.Value.Flat = 7.0f;
			SetBonusModifier.Value.Mul = 0.0f;
			SetBonusModifier.bIsPermanent = false;
			SetBonusModifier.Duration = BonusDuration;
			SetBonusModifier.ModifierPolicy = EModifierStackPolicy::Refresh;
			SetBonusModifier.MaxStack = 1;
		}
	}
}

void UStatusAction_SetBonus::StartAction_Implementation(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat)
{
	Super::StartAction_Implementation(RegenStat, NonRegenStat);

	if (!SetBonusTag.IsValid()) return;
	if (RegenStat.ParentTag != Arcanum::BattleStat::Character::Regen::Health::Root) return;
	if (bIsBonusActive || bIsCooldown) return;

	const float totalMax = RegenStat.GetTotalMax();
	if (totalMax <= 0.0f) return;

	const float currentHealthPercent = RegenStat.Current / totalMax;
	if (currentHealthPercent > TriggerHealthPercent) return;

	AActor* ownerActor = Cast<AActor>(GetOuter());
	if (!ownerActor) return;

	UCharacterBattleStatsComponent* statComponent = ownerActor->FindComponentByClass<UCharacterBattleStatsComponent>();
	if (!statComponent) return;

	UE_LOG(LogTemp, Warning, TEXT("%s SetBonus Triggered"), *ownerActor->GetName());
	statComponent->ApplyDurationModifier(SetBonusModifier);

	bIsBonusActive = true;
	StartBonusTimer();
	StartCooldown();

	ACharacter* ownerCharacter = Cast<ACharacter>(ownerActor);
	if (ownerCharacter && SetBonusNiagara)
	{
		if (!ActiveNiagaraComponent)
		{
			ActiveNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
				SetBonusNiagara,
				ownerCharacter->GetMesh(),
				AttachSocketName,
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				EAttachLocation::SnapToTarget,
				false,
				true,
				ENCPoolMethod::AutoRelease,
				true
			);
		}
	}
}

void UStatusAction_SetBonus::StartBonusTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(BonusTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(
		BonusTimerHandle,
		this,
		&UStatusAction_SetBonus::EndBonus,
		BonusDuration,
		false);
}

void UStatusAction_SetBonus::EndBonus()
{
	bIsBonusActive = false;

	if (ActiveNiagaraComponent)
	{
		ActiveNiagaraComponent->Deactivate();
		ActiveNiagaraComponent = nullptr;
	}
}

void UStatusAction_SetBonus::StartCooldown()
{
	bIsCooldown = true;

	GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(
		CooldownTimerHandle,
		this,
		&UStatusAction_SetBonus::EndCooldown,
		CooldownDuration,
		false);
}

void UStatusAction_SetBonus::EndCooldown()
{
	bIsCooldown = false;
}
