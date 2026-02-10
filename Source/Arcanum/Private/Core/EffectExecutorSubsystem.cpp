//#include "Core/EffectExecutorSubsystem.h"
//#include "Engine/DataTable.h"
//#include "Kismet/GameplayStatics.h"
//#include "Data/Rows/WeaponSkillMapRow.h"
//#include "Data/Skill/SkillRow.h"
//#include "Data/Skill/EffectRow.h"
//#include "Data/VFX/VfxRow.h"
//#include "Engine/OverlapResult.h"
//#include "NiagaraFunctionLibrary.h"
//#include "NiagaraSystem.h"
//
//void UEffectExecutorSubsystem::Initialize(FSubsystemCollectionBase& InCollection)
//{
//	Super::Initialize(InCollection);
//}
//
//void UEffectExecutorSubsystem::Deinitialize()
//{
//	WeaponSkillMapTable = nullptr;
//	SkillTable = nullptr;
//	EffectTable = nullptr;
//	VfxTable = nullptr;
//
//	AreaPool = nullptr;
//	ProjectilePool = nullptr;
//
//	Super::Deinitialize();
//}
//
//void UEffectExecutorSubsystem::BindTables(UDataTable* InWeaponSkillMapTable, UDataTable* InSkillTable, UDataTable* InEffectTable, UDataTable* InVfxTable)
//{
//	WeaponSkillMapTable = InWeaponSkillMapTable;
//	SkillTable = InSkillTable;
//	EffectTable = InEffectTable;
//	VfxTable = InVfxTable;
//}
//
//void UEffectExecutorSubsystem::SetPools(UPoolingComponent* InAreaPool, UPoolingComponent* InProjectilePool)
//{
//	AreaPool = InAreaPool;
//	ProjectilePool = InProjectilePool;
//}
//
//bool UEffectExecutorSubsystem::FindSkillIDByWeaponTag(const FGameplayTag& InWeaponTag, FGameplayTag& OutSkillID) const
//{
//	OutSkillID = FGameplayTag();
//
//	if (const FWeaponSkillMapRow* row = FindWeaponSkillMapRow(InWeaponTag))
//	{
//		if (row->CommonSkillID.IsValid())
//		{
//			OutSkillID = row->CommonSkillID;
//		}
//		else if (row->LegendarySkillID.IsValid())
//		{
//			OutSkillID = row->LegendarySkillID;
//		}
//	}
//
//	return OutSkillID.IsValid();
//}
//
//bool UEffectExecutorSubsystem::ExecuteSkillByID(const FGameplayTag& InSkillID, const FSkillContext& InContext, AActor* InTargetActor)
//{
//	if (const FSkillRow* row = FindSkillRow(InSkillID))
//	{
//		return ExecuteSkillRow(*row, InContext, InTargetActor);
//	}
//
//	return false;
//}
//
//bool UEffectExecutorSubsystem::ExecuteEffectByID(const FName& InEffectID, const FSkillContext& InContext, AActor* InTargetActor)
//{
//	if (const FEffectRow* row = FindEffectRow(InEffectID))
//	{
//		return ExecuteEffectRow(InEffectID, *row, InContext, InTargetActor);
//	}
//
//	return false;
//}
//
//// TODO : 나중에 Initialize() 에서 한 번만 전부 읽어서 WeaponTag -> Row 포인터 캐시(TMap)
//FWeaponSkillMapRow* UEffectExecutorSubsystem::FindWeaponSkillMapRow(const FGameplayTag& InWeaponTag) const
//{
//	if (WeaponSkillMapTable && InWeaponTag.IsValid())
//	{
//		TArray<FWeaponSkillMapRow*> rows;
//		WeaponSkillMapTable->GetAllRows<FWeaponSkillMapRow>(TEXT("FindWeaponSkillMapRow"), rows);
//
//		for (FWeaponSkillMapRow* row : rows)
//		{
//			if (row)
//			{
//				if (row->WeaponTag == InWeaponTag)
//				{
//					return row;
//				}
//			}
//		}
//	}
//
//	return nullptr;
//}
//
//// TODO : 나중에 Initialize() 에서 한 번만 전부 읽어서 캐시(TMap)
//FSkillRow* UEffectExecutorSubsystem::FindSkillRow(const FGameplayTag& InSkillID) const
//{
//	if (SkillTable && InSkillID.IsValid())
//	{
//		TArray<FSkillRow*> rows;
//		SkillTable->GetAllRows<FSkillRow>(TEXT("FindSkillRow"), rows);
//
//		for (FSkillRow* row : rows)
//		{
//			if (row)
//			{
//				// TODO : FSkillRow SkillID
//				if (row->SkillID == InSkillID)
//				{
//					return row;
//				}
//			}
//		}
//
//		UE_LOG(LogTemp, Warning, TEXT("FindSkillRow : 못찾음 SkillID=%s"), *InSkillID.GetTagName().ToString());
//	}
//
//	return nullptr;
//}
//
//FEffectRow* UEffectExecutorSubsystem::FindEffectRow(const FName& InEffectID) const
//{
//	if (EffectTable && !InEffectID.IsNone())
//	{
//		return EffectTable->FindRow<FEffectRow>(InEffectID, TEXT("FindEffectRow"));
//	}
//
//	return nullptr;
//}
//
//FVfxRow* UEffectExecutorSubsystem::FindVfxRow(const FName& InVfxID) const
//{
//	if (VfxTable && !InVfxID.IsNone())
//	{
//		return VfxTable->FindRow<FVfxRow>(InVfxID, TEXT("FindVfxRow"));
//	}
//
//	return nullptr;
//}
//
//bool UEffectExecutorSubsystem::ExecuteSkillRow(const FSkillRow& InRow, const FSkillContext& InContext, AActor* InTargetActor)
//{
//	for (const FName& effectId : InRow.EffectIDs)
//	{
//		if (!ExecuteEffectByID(effectId, InContext, InTargetActor))
//		{
//			return false;
//		}
//	}
//
//	return true;
//}
//
//bool UEffectExecutorSubsystem::ExecuteEffectRow(const FName& InEffectID, const FEffectRow& InRow, const FSkillContext& InContext, AActor* InTargetActor)
//{
//	TArray<TWeakObjectPtr<AActor>> targets;
//	const bool hasTargets = BuildTargets(InRow, InContext, InTargetActor, targets);
//
//	if (!hasTargets && !CanExecuteWithoutTargetActor(InRow))
//	{
//		return false;
//	}
//
//	ApplyEffectToTargets(InRow, InContext, targets);
//
//	if (InRow.VfxIDs.Num() > 0)
//	{
//		ExecuteVfxByIDs(InRow.VfxIDs, InContext, InTargetActor, targets);
//	}
//
//	return true;
//}
//
//bool UEffectExecutorSubsystem::CanExecuteWithoutTargetActor(const FEffectRow& InRow) const
//{
//	// TargetType 기준
//	// TargetActor가 필요 없는 케이스는 전부 true
//	if (InRow.TargetTypeTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(TEXT("Arcanum.TargetType.Self"))))
//	{
//		return true;
//	}
//
//	if (InRow.TargetTypeTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(TEXT("Arcanum.TargetType.AlliesInRadius"))))
//	{
//		return true;
//	}
//
//	if (InRow.TargetTypeTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(TEXT("Arcanum.TargetType.EnemiesInRadius"))))
//	{
//		return true;
//	}
//
//	if (InRow.TargetTypeTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(TEXT("Arcanum.TargetType.BothInRadius"))))
//	{
//		return true;
//	}
//
//	// EffectType 범위/장판
//	if (InRow.EffectTypeTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(TEXT("Arcanum.EffectType.SpawnArea"))))
//	{
//		return true;
//	}
//
//	return false;
//}
//
//// TODO : 나중에 수정
//bool UEffectExecutorSubsystem::BuildTargets(const FEffectRow& InRow, const FSkillContext& InContext, AActor* InTargetActor, TArray<TWeakObjectPtr<AActor>>& OutTargets) const
//{
//	return false;
//}
//
//void UEffectExecutorSubsystem::ApplyTargetFilter(const FGameplayTag& InFilterTag, TArray<TWeakObjectPtr<AActor>>& InOutTargets) const
//{
//}
//
//void UEffectExecutorSubsystem::ApplyEffectToTargets(const FEffectRow& InRow, const FSkillContext& InContext, const TArray<TWeakObjectPtr<AActor>>& InTargets)
//{
//}
//
//void UEffectExecutorSubsystem::ExecuteVfxByIDs(const TArray<FName>& InVfxIDs, const FSkillContext& InContext, AActor* InTargetActor, const TArray<TWeakObjectPtr<AActor>>& InTargets)
//{
//}
//
//void UEffectExecutorSubsystem::SpawnAreaFromPool(const FEffectRow& InRow, const FSkillContext& InContext, const TArray<TWeakObjectPtr<AActor>>& InTargets)
//{
//}
//
//void UEffectExecutorSubsystem::SpawnProjectileFromPool(const FEffectRow& InRow, const FSkillContext& InContext, AActor* InTargetActor, const TArray<TWeakObjectPtr<AActor>>& InTargets)
//{
//}
//
//
//
