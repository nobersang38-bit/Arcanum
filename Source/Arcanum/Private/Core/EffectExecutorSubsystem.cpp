/* TODO : GPT 일단 오류 확인 */

#include "Core/EffectExecutorSubsystem.h"
//#include "Engine/DataTable.h"
//#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
//#include "GameFramework/Pawn.h"
//#include "GameFramework/Controller.h"
#include "GameplayTags/ArcanumTags.h"
#include "Component/PoolingComponent.h"
#include "Data/Skill/SkillContext.h"
#include "Data/Skill/EffectRow.h"
#include "Data/VFX/VfxRow.h"

DEFINE_LOG_CATEGORY_STATIC(LogEffectExecutorSubsystem, Log, All);

namespace
{
	static bool IsPlayerActor(AActor* InActor)
	{
		if (!InActor)
		{
			return false;
		}

		const APawn* pawn = Cast<APawn>(InActor);
		if (!pawn)
		{
			return false;
		}

		return pawn->IsPlayerControlled();
	}

	static bool HasTagName(AActor* InActor, const FName& InTagName)
	{
		if (!InActor)
		{
			return false;
		}

		return InActor->ActorHasTag(InTagName);
	}

	static FVector GetCenterLocation(const FSkillContext& InContext, AActor* InFallbackActor)
	{
		// TargetLocation이 (0,0,0)일 수 있으면 여기 정책을 너 규칙대로 바꿔라.
		if (!InContext.TargetLocation.IsNearlyZero())
		{
			return InContext.TargetLocation;
		}

		if (InFallbackActor)
		{
			return InFallbackActor->GetActorLocation();
		}

		if (InContext.Instigator.IsValid())
		{
			return InContext.Instigator->GetActorLocation();
		}

		return FVector::ZeroVector;
	}
}

void UEffectExecutorSubsystem::Initialize(FSubsystemCollectionBase& InCollection)
{
	Super::Initialize(InCollection);

	EffectTable = nullptr;
	VfxTable = nullptr;
	AreaPool = nullptr;
	ProjectilePool = nullptr;

	UE_LOG(LogEffectExecutorSubsystem, Log, TEXT("EffectExecutorSubsystem Initialize"));
}

void UEffectExecutorSubsystem::Deinitialize()
{
	UE_LOG(LogEffectExecutorSubsystem, Log, TEXT("EffectExecutorSubsystem Deinitialize"));

	EffectTable = nullptr;
	VfxTable = nullptr;
	AreaPool = nullptr;
	ProjectilePool = nullptr;

	Super::Deinitialize();
}

void UEffectExecutorSubsystem::BindTables(UDataTable* InEffectTable, UDataTable* InVfxTable)
{
	EffectTable = InEffectTable;
	VfxTable = InVfxTable;

	if (!EffectTable)
	{
		UE_LOG(LogEffectExecutorSubsystem, Error, TEXT("BindTables: EffectTable is null"));
	}
	if (!VfxTable)
	{
		UE_LOG(LogEffectExecutorSubsystem, Warning, TEXT("BindTables: VfxTable is null (VFX will be skipped)"));
	}
}

void UEffectExecutorSubsystem::SetPools(UPoolingComponent* InAreaPool, UPoolingComponent* InProjectilePool)
{
	AreaPool = InAreaPool;
	ProjectilePool = InProjectilePool;

	if (!AreaPool)
	{
		UE_LOG(LogEffectExecutorSubsystem, Warning, TEXT("SetPools: AreaPool is null (SpawnArea will be skipped)"));
	}
	if (!ProjectilePool)
	{
		UE_LOG(LogEffectExecutorSubsystem, Warning, TEXT("SetPools: ProjectilePool is null (Projectile will be skipped)"));
	}
}

bool UEffectExecutorSubsystem::ExecuteEffectByID(const FName& InEffectID, const FSkillContext& InContext, AActor* InTargetActor)
{
	const FEffectRow* row = FindEffectRow(InEffectID);
	if (!row)
	{
		return false;
	}

	return ExecuteEffectRow(InEffectID, *row, InContext, InTargetActor);
}

bool UEffectExecutorSubsystem::ExecuteEffectsByIDs(const TArray<FName>& InEffectIDs, const FSkillContext& InContext, AActor* InTargetActor)
{
	bool allOk = true;

	for (const FName& effectId : InEffectIDs)
	{
		if (effectId.IsNone())
		{
			continue;
		}

		const bool ok = ExecuteEffectByID(effectId, InContext, InTargetActor);
		if (!ok)
		{
			allOk = false;
		}
	}

	return allOk;
}

const FEffectRow* UEffectExecutorSubsystem::FindEffectRow(const FName& InEffectID) const
{
	if (!EffectTable)
	{
		UE_LOG(LogEffectExecutorSubsystem, Error, TEXT("FindEffectRow: EffectTable is null. EffectID=%s"), *InEffectID.ToString());
		return nullptr;
	}

	static const FString contextString(TEXT("UEffectExecutorSubsystem::FindEffectRow"));
	const FEffectRow* row = EffectTable->FindRow<FEffectRow>(InEffectID, contextString, /*bWarnIfRowMissing*/ false);
	if (!row)
	{
		UE_LOG(LogEffectExecutorSubsystem, Error, TEXT("FindEffectRow: missing row. EffectID=%s, Table=%s"),
			*InEffectID.ToString(), *EffectTable->GetName());
	}

	return row;
}

const FVfxRow* UEffectExecutorSubsystem::FindVfxRow(const FName& InVfxID) const
{
	if (!VfxTable)
	{
		return nullptr;
	}

	static const FString contextString(TEXT("UEffectExecutorSubsystem::FindVfxRow"));
	const FVfxRow* row = VfxTable->FindRow<FVfxRow>(InVfxID, contextString, /*bWarnIfRowMissing*/ false);
	if (!row)
	{
		UE_LOG(LogEffectExecutorSubsystem, Warning, TEXT("FindVfxRow: missing row. VfxID=%s, Table=%s"),
			*InVfxID.ToString(), *VfxTable->GetName());
	}

	return row;
}

bool UEffectExecutorSubsystem::ExecuteEffectRow(const FName& InEffectID, const FEffectRow& InRow, const FSkillContext& InContext, AActor* InTargetActor)
{
	TArray<TWeakObjectPtr<AActor>> targets;
	const bool hasTargets = ResolveTargets(InRow, InContext, InTargetActor, targets);

	// 타겟이 없으면 “타겟 없이 실행 가능한 타입”만 통과
	if (!hasTargets)
	{
		if (!IsTargetOptional(InRow))
		{
			UE_LOG(LogEffectExecutorSubsystem, Warning, TEXT("ExecuteEffectRow: no targets. EffectID=%s"), *InEffectID.ToString());
			return false;
		}
	}

	ApplyTargetFilter(InRow.TargetFilterTag, targets);

	ApplyEffectToTargets(InRow, InContext, targets);

	// VFX는 타겟이 없어도 실행 가능(예: 장판 시작 VFX)
	ExecuteVfxByIDs(InRow.VfxIDs, InContext, InTargetActor, targets);

	return true;
}

bool UEffectExecutorSubsystem::IsTargetOptional(const FEffectRow& InRow) const
{
	// SpawnArea는 타겟 없이도 가능
	if (InRow.EffectTypeTag.MatchesTag(Arcanum::EffectType::SpawnArea))
	{
		return true;
	}
	// 나머지는 기본적으로 타겟 필요
	return false;
}

bool UEffectExecutorSubsystem::ResolveTargets(const FEffectRow& InRow, const FSkillContext& InContext, AActor* InTargetActor, TArray<TWeakObjectPtr<AActor>>& OutTargets) const
{
	OutTargets.Reset();

	// Self
	if (InRow.TargetTypeTag.MatchesTag(Arcanum::TargetType::Self))
	{
		if (InContext.Instigator.IsValid())
		{
			OutTargets.Add(InContext.Instigator.Get());
			return true;
		}
		return false;
	}

	// Target
	if (InRow.TargetTypeTag.MatchesTag(Arcanum::TargetType::Target))
	{
		if (InTargetActor)
		{
			OutTargets.Add(InTargetActor);
			return true;
		}
		return false;
	}

	if (InRow.Radius <= 0.0f)
	{
		return false;
	}

	UWorld* world = GetWorld();
	if (!world)
	{
		return false;
	}

	const FVector center = GetCenterLocation(InContext, InTargetActor);

	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	objectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	TArray<AActor*> ignoreActors;
	if (InContext.Instigator.IsValid())
	{
		ignoreActors.Add(InContext.Instigator.Get());
	}

	TArray<AActor*> hitActors;
	const bool overlapped = UKismetSystemLibrary::SphereOverlapActors(
		world,
		center,
		InRow.Radius,
		objectTypes,
		nullptr,
		ignoreActors,
		hitActors
	);

	if (!overlapped || hitActors.Num() <= 0)
	{
		return false;
	}

	const bool wantAllies = InRow.TargetTypeTag.MatchesTag(Arcanum::TargetType::AlliesInRadius);
	const bool wantEnemies = InRow.TargetTypeTag.MatchesTag(Arcanum::TargetType::EnemiesInRadius);
	const bool wantBoth = InRow.TargetTypeTag.MatchesTag(Arcanum::TargetType::BothInRadius);

	for (AActor* actor : hitActors)
	{
		if (!actor)
		{
			continue;
		}

		if (wantBoth)
		{
			OutTargets.Add(actor);
			continue;
		}

		// 임시 팀 판정 : ActorTag "Ally"/"Enemy"
		const bool isAlly = HasTagName(actor, TEXT("Ally")) || IsPlayerActor(actor);
		const bool isEnemy = HasTagName(actor, TEXT("Enemy"));

		if (wantAllies)
		{
			if (isAlly)
			{
				OutTargets.Add(actor);
			}
		}
		else if (wantEnemies)
		{
			if (isEnemy)
			{
				OutTargets.Add(actor);
			}
		}
		else
		{
			return false;
		}
	}

	return (OutTargets.Num() > 0);
}

void UEffectExecutorSubsystem::ApplyTargetFilter(const FGameplayTag InFilterTag, TArray<TWeakObjectPtr<AActor>>& InOutTargets) const
{
	if (!InFilterTag.IsValid())
	{
		return;
	}

	if (InFilterTag.MatchesTag(Arcanum::TargetFilter::PlayersOnly))
	{
		InOutTargets.RemoveAll([](const TWeakObjectPtr<AActor>& actor)
			{
				if (!actor.IsValid())
				{
					return true;
				}
				return !IsPlayerActor(actor.Get());
			});
		return;
	}

	if (InFilterTag.MatchesTag(Arcanum::TargetFilter::ExcludePlayers))
	{
		InOutTargets.RemoveAll([](const TWeakObjectPtr<AActor>& actor)
			{
				if (!actor.IsValid())
				{
					return true;
				}
				return IsPlayerActor(actor.Get());
			});
		return;
	}

	if (InFilterTag.MatchesTag(Arcanum::TargetFilter::ExcludeAllies))
	{
		InOutTargets.RemoveAll([](const TWeakObjectPtr<AActor>& actor)
			{
				if (!actor.IsValid())
				{
					return true;
				}
				return HasTagName(actor.Get(), TEXT("Ally")) || IsPlayerActor(actor.Get());
			});
		return;
	}
}

void UEffectExecutorSubsystem::ApplyEffectToTargets(const FEffectRow& InRow, const FSkillContext& InContext, const TArray<TWeakObjectPtr<AActor>>& InTargets)
{
	AActor* instigatorActor = InContext.Instigator.Get();
	AController* instigatorController = nullptr;

	if (instigatorActor)
	{
		const APawn* pawn = Cast<APawn>(instigatorActor);
		if (pawn)
		{
			instigatorController = pawn->GetController();
		}
	}

	// Damage
	if (InRow.EffectTypeTag.MatchesTag(Arcanum::EffectType::Damage))
	{
		for (const TWeakObjectPtr<AActor>& target : InTargets)
		{
			if (!target.IsValid())
			{
				continue;
			}

			UGameplayStatics::ApplyDamage(
				target.Get(),
				InRow.Value,
				instigatorController,
				instigatorActor,
				nullptr
			);
		}
		return;
	}

	// Heal (임시; ApplyDamage 음수)
	if (InRow.EffectTypeTag.MatchesTag(Arcanum::EffectType::Heal))
	{
		for (const TWeakObjectPtr<AActor>& target : InTargets)
		{
			if (!target.IsValid())
			{
				continue;
			}

			UGameplayStatics::ApplyDamage(
				target.Get(),
				-InRow.Value,
				instigatorController,
				instigatorActor,
				nullptr
			);
		}
		return;
	}

	// SpawnArea (풀링 필수)
	if (InRow.EffectTypeTag.MatchesTag(Arcanum::EffectType::SpawnArea))
	{
		SpawnAreaFromPool(InRow, InContext);
		return;
	}

	// Projectile (풀링 필수) - 태그가 아직 없으면 무시
	// if (InRow.EffectTypeTag.MatchesTag(Arcanum::EffectType::Projectile))
	// {
	//     SpawnProjectileFromPool(InRow, InContext, InTargetActor);
	//     return;
	// }

	UE_LOG(LogEffectExecutorSubsystem, Warning,
		TEXT("ApplyEffectToTargets: TODO EffectType=%s Value=%f Duration=%f MaxStack=%d Interval=%f"),
		*InRow.EffectTypeTag.ToString(),
		InRow.Value,
		InRow.Duration,
		InRow.MaxStack,
		InRow.Interval
	);
}

void UEffectExecutorSubsystem::ExecuteVfxByIDs(const TArray<FName>& InVfxIDs, const FSkillContext& InContext, AActor* InTargetActor, const TArray<TWeakObjectPtr<AActor>>& InTargets)
{
	if (!VfxTable)
	{
		return;
	}

	for (const FName& vfxId : InVfxIDs)
	{
		if (vfxId.IsNone())
		{
			continue;
		}

		const FVfxRow* row = FindVfxRow(vfxId);
		if (!row)
		{
			continue;
		}

		// TODO: FVfxRow 확정되면 여기서 Niagara 스폰 연결
		// AutoRelease 쓸거임
		UE_LOG(LogEffectExecutorSubsystem, Verbose, TEXT("ExecuteVfxByIDs: VfxID=%s"), *vfxId.ToString());
	}
}

void UEffectExecutorSubsystem::SpawnAreaFromPool(const FEffectRow& InRow, const FSkillContext& InContext)
{
	if (!AreaPool)
	{
		UE_LOG(LogEffectExecutorSubsystem, Warning, TEXT("SpawnAreaFromPool: AreaPool is null"));
		return;
	}

	const FVector spawnLocation = GetCenterLocation(InContext, nullptr);

	UObject* pooled = AreaPool->ActivatePooledItem(EReferenceLocation::Absolute, spawnLocation);
	AActor* pooledActor = Cast<AActor>(pooled);
	if (pooledActor)
	{
		pooledActor->SetActorLocation(spawnLocation);
	}

	// IPoolable 확장
}

void UEffectExecutorSubsystem::SpawnProjectileFromPool(const FEffectRow& InRow, const FSkillContext& InContext, AActor* InTargetActor)
{
	if (!ProjectilePool)
	{
		UE_LOG(LogEffectExecutorSubsystem, Warning, TEXT("SpawnProjectileFromPool: ProjectilePool is null"));
		return;
	}

	const FVector spawnLocation = GetCenterLocation(InContext, InContext.Instigator.Get());

	UObject* pooled = ProjectilePool->ActivatePooledItem(EReferenceLocation::Absolute, spawnLocation);
	AActor* pooledActor = Cast<AActor>(pooled);
	if (pooledActor)
	{
		pooledActor->SetActorLocation(spawnLocation);
	}

	//  투사체에 파라미터 전달(IPoolable 확장)
}

