#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "GameplayTagContainer.h"
#include "Data/Skill/SkillContext.h"
#include "EffectExecutorSubsystem.generated.h"

class UDataTable;
class AActor;
class UPoolingComponent;

struct FSkillContext;
struct FEffectRow;
struct FVfxRow;

/**
 * 추영호
 * FEffectRow 1줄(또는 EffectIDs 배열)을 실제 게임 결과로 적용하는 공용 실행기
 * Skill/Awaken/Trigger 쪽은 '무슨 EffectID를 실행할지'만 결정
 * 실행 적용(데미지/힐/디버프/장판/VFX)은 여기서 통일
 */
UCLASS()
class ARCANUM_API UEffectExecutorSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UEffectExecutorSubsystem() = default;

	/* 서브 시스템 초기화(태그 캐시 등) */
	virtual void Initialize(FSubsystemCollectionBase& InCollection) override;
	virtual void Deinitialize() override;

	/* DT 바인딩 */
	UFUNCTION(BlueprintCallable, Category = "Arcanum|Execute")
	void BindTables(UDataTable* InEffectTable, UDataTable* InVfxTable);

	/* 풀링 바인딩 */
	UFUNCTION(BlueprintCallable, Category = "Arcanum|Execute")
	void SetPools(UPoolingComponent* InAreaPool, UPoolingComponent* InProjectilePool);

	/* EffectID(RowName) 1개 실행 */
	UFUNCTION(BlueprintCallable, Category = "Arcanum|Execute")
	bool ExecuteEffectByID(const FName& InEffectID, const FSkillContext& InContext, AActor* InTargetActor);

	/* EffectID(RowName) 여러 개 실행 */
	UFUNCTION(BlueprintCallable, Category = "Arcanum|Execute")
	bool ExecuteEffectsByIDs(const TArray<FName>& InEffectIDs, const FSkillContext& InContext, AActor* InTargetActor);

private:
	/* DT 조회 */
	const FEffectRow* FindEffectRow(const FName& InEffectID) const;
	const FVfxRow* FindVfxRow(const FName& InVfxID) const;

	/* EffectRow 1줄 실행 */
	bool ExecuteEffectRow(const FName& InEffectID, const FEffectRow& InRow, const FSkillContext& InContext, AActor* InTargetActor);

	/* 이 EffectType은 타겟 없이도 실행 가능한가(SpawnArea/일부 VFX 등) */
	bool IsTargetOptional(const FEffectRow& InRow) const;

	/* 타겟 리스트 구성 */
	bool ResolveTargets(const FEffectRow& InRow, const FSkillContext& InContext, AActor* InTargetActor, TArray<TWeakObjectPtr<AActor>>& OutTargets) const;

	/* 타겟 필터 적용 */
	void ApplyTargetFilter(const FGameplayTag InFilterTag, TArray<TWeakObjectPtr<AActor>>& InOutTargets) const;

	/* EffectType 분기 실제 적용 */
	void ApplyEffectToTargets(const FEffectRow& InRow, const FSkillContext& InContext, const TArray<TWeakObjectPtr<AActor>>& InTargets);

	/* VFX 실행 */
	void ExecuteVfxByIDs(const TArray<FName>& InVfxIDs, const FSkillContext& InContext, AActor* InTargetActor, const TArray<TWeakObjectPtr<AActor>>& InTargets);

	/* 풀링: '사용'만 */
	void SpawnAreaFromPool(const FEffectRow& InRow, const FSkillContext& InContext);
	void SpawnProjectileFromPool(const FEffectRow& InRow, const FSkillContext& InContext, AActor* InTargetActor);

private:
	UPROPERTY(Transient)
	TObjectPtr<UDataTable> EffectTable = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UDataTable> VfxTable = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UPoolingComponent> AreaPool = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UPoolingComponent> ProjectilePool = nullptr;
};

