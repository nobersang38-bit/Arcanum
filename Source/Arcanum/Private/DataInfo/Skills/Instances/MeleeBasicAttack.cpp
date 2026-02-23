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
		if (const FLevelModifierEntry* entry = GetCurrentLevelEntry())
		{
			/*
			// 기본공격 범위
			DrawDebugSphere(
				GetWorld(),
				Instigator->GetActorLocation(),
				SkillData.Targeting.MaxRange,
				24,
				FColor::Green,
				false,
				0.5f,
				0,
				2.0f
			);
			*/

			AActor* frontTarget = FindFrontTarget(Instigator, SkillData.Targeting.MaxRange);

			UE_LOG(LogTemp, Warning, TEXT("FrontTarget = %s"), frontTarget ? *frontTarget->GetName() : TEXT("None"));

			if (frontTarget)
			{
				const bool bHitApplied = ApplyBasicAttackDamage(Instigator, frontTarget, *entry);

				UE_LOG(LogTemp, Warning, TEXT("ApplyBasicAttackDamage = %s"), bHitApplied ? TEXT("true") : TEXT("false"));

				if (bHitApplied)
				{
					NotifyBasicAttackHitToSkillComponent(Instigator, frontTarget);
				}
			}
		}
	}
}

AActor* UMeleeBasicAttack::FindFrontTarget(AActor* InInstigator, float InRange) const
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
				ABaseUnitCharacter::StaticClass(),  // 테스트 캐릭터 (nullptr)
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

						// 적/아군 판정 추가 (기본공격은 Enemy만 타겟)
						if (!CanAffectTarget(InInstigator, actor, Arcanum::Skills::TargetFilter::Enemy))
						{
							continue;
						}

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
		// 기본공격 데미지 + 현재 각성 디버프를 합쳐서 적용
		TArray<FDerivedStatModifier> finalModifiers;

		if (BuildHitModifiersWithAwaken(InInstigator, InEntry.OtherCharacterModifiers, finalModifiers))
		{
			return ApplyModifiersToTarget(InInstigator, InTarget, finalModifiers);
		}
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