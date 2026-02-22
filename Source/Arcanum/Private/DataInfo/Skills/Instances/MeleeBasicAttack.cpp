#include "DataInfo/Skills/Instances/MeleeBasicAttack.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Component/SkillComponent.h"
#include "Component/Stats/CharacterBattleStatsComponent.h"
#include "Character/BaseUnitCharacter.h"

void UMeleeBasicAttack::ActivateSkill(AActor* Instigator)
{
	Super::ActivateSkill(Instigator);

	UE_LOG(LogTemp, Warning, TEXT("UMeleeBasicAttack::ActivateSkill"));

	if (Instigator)
	{
		const FLevelModifierEntry* entry = GetCurrentLevelEntry();

		if (entry)
		{
			AActor* nearestTarget = FindNearestTarget(Instigator, SkillData.Targeting.MaxRange);

			UE_LOG(LogTemp, Warning, TEXT("NearestTarget = %s"), nearestTarget ? *nearestTarget->GetName() : TEXT("None"));

			if (nearestTarget)
			{
				const bool bHitApplied = ApplyBasicAttackDamage(Instigator, nearestTarget, *entry);

				UE_LOG(LogTemp, Warning, TEXT("ApplyBasicAttackDamage = %s"), bHitApplied ? TEXT("true") : TEXT("false"));

				if (bHitApplied)
				{
					NotifyBasicAttackHitToSkillComponent(Instigator, nearestTarget);
				}
			}
		}
	}
}

AActor* UMeleeBasicAttack::FindNearestTarget(AActor* InInstigator, float InRange) const
{
	AActor* nearestTarget = nullptr;

	if (InInstigator)
	{
		if (UWorld* world = InInstigator->GetWorld())
		{
			TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
			objectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

			TArray<AActor*> actorsToIgnore;
			actorsToIgnore.Add(InInstigator);

			TArray<AActor*> overlappedActors;

			const bool bHit = UKismetSystemLibrary::SphereOverlapActors(
				world,
				InInstigator->GetActorLocation(),
				InRange,
				objectTypes,
				ABaseUnitCharacter::StaticClass(),
				actorsToIgnore,
				overlappedActors
			);

			if (bHit)
			{
				float nearestDistanceSquared = MAX_flt;

				for (AActor* actor : overlappedActors)
				{
					UCharacterBattleStatsComponent* targetStatsComponent = actor->FindComponentByClass<UCharacterBattleStatsComponent>();

					if (targetStatsComponent)
					{
						// RegenStat에서 Health 루트 태그를 찾아서 현재 체력을 꺼냄
						float currentHealthValue = -1.0f;

						for (const FRegenStat& regenStat : targetStatsComponent->GetRegenStats())
						{
							if (regenStat.ParentTag == Arcanum::BattleStat::Character::Regen::Health::Root)
							{
								currentHealthValue = regenStat.Current;

								break;
							}
						}

						// 죽은 대상 제외
						if (currentHealthValue <= 0.0f)
						{
							continue;
						}

						// TODO : 팀 판정 로직
						// if (Team(InInstigator, actor))
						// {
						//    continue;
						// }

						const float distanceSquared = FVector::DistSquared2D(InInstigator->GetActorLocation(), actor->GetActorLocation());

						if (distanceSquared < nearestDistanceSquared)
						{
							nearestDistanceSquared = distanceSquared;
							nearestTarget = actor;
						}
					}
				}
			}
		}
	}

	return nearestTarget;
}

bool UMeleeBasicAttack::ApplyBasicAttackDamage(AActor* InInstigator, AActor* InTarget, const FLevelModifierEntry& InEntry)
{
	if (InTarget)
	{
		return ApplyModifiersToTarget(InInstigator, InTarget, InEntry.OtherCharacterModifiers);
	}

	return false;
}

void UMeleeBasicAttack::NotifyBasicAttackHitToSkillComponent(AActor* InInstigator, AActor* InTarget)
{
	if (InInstigator)
	{
		if (InTarget)
		{
			USkillComponent* skillComponent = InInstigator->FindComponentByClass<USkillComponent>();

			if (skillComponent)
			{
				// 기본공격 적중 알림
				skillComponent->NotifyBasicAttackHit(InTarget);
			}
		}
	}
}