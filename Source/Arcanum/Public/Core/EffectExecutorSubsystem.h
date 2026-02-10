//#pragma once
//
//#include "CoreMinimal.h"
//#include "Subsystems/WorldSubsystem.h"
//#include "GameplayTagContainer.h"
//#include "Data/Skill/SkillContext.h"
//#include "EffectExecutorSubsystem.generated.h"
//
///**
// * 추영호
// * FEffectRow 1줄(또는 EffectIDs 배열)을 실제 게임 결과로 적용하는 공용 실행기
// * Skill/Awaken/Trigger 쪽은 '무슨 EffectID를 실행할지'만 결정
// * 실행 적용(데미지/힐/디버프/장판/VFX)은 여기서 통일
// */
//
//class UDataTable;
//class AActor;
//class UPoolingComponent;
//
//struct FWeaponSkillMapRow;
//struct FSkillRow;
//struct FEffectRow;
//struct FVfxRow;
//
//UCLASS()
//class ARCANUM_API UEffectExecutorSubsystem : public UWorldSubsystem
//{
//	GENERATED_BODY()
//
//public:
//	UEffectExecutorSubsystem() = default;
//
//	/* 초기화 */
//	virtual void Initialize(FSubsystemCollectionBase& InCollection) override;
//	virtual void Deinitialize() override;
//
//	/* DT 바인딩 */
//	UFUNCTION(BlueprintCallable, Category = "Arcanum|Execute")
//	void BindTables(UDataTable* InWeaponSkillMapTable, UDataTable* InSkillTable, UDataTable* InEffectTable, UDataTable* InVfxTable);
//
//	/* 풀링 바인딩(장판/투사체만) */
//	UFUNCTION(BlueprintCallable, Category = "Arcanum|Execute")
//	void SetPools(UPoolingComponent* InAreaPool, UPoolingComponent* InProjectilePool);
//
//	/* 무기 태그로 스킬ID (WeaponTag -> SkillID Row에서 Common/Legendary 중 1개 선택) */
//	UFUNCTION(BlueprintCallable, Category = "Arcanum|Execute")
//	bool FindSkillIDByWeaponTag(const FGameplayTag& InWeaponTag, FGameplayTag& OutSkillID) const;
//
//
//	/* 스킬 실행: SkillID로 SkillTable(SkillRow)를 찾고, 그 안의 EffectIDs(여러 개)를 순서대로 실행 */
//	UFUNCTION(BlueprintCallable, Category = "Arcanum|Execute")
//	bool ExecuteSkillByID(const FGameplayTag& InSkillID, const FSkillContext& InContext, AActor* InTargetActor);
//
//	/* 단일 효과 실행: EffectID(RowName) 1개를 EffectTable(EffectRow)에서 찾아 즉시 실행 (트리거/각성/세트효과) */
//	UFUNCTION(BlueprintCallable, Category = "Arcanum|Execute")
//	bool ExecuteEffectByID(const FName& InEffectID, const FSkillContext& InContext, AActor* InTargetActor);
//
//
//private:
//	/* DT 조회 (ID로 해당 DataTable Row를 찾기) */
//	FWeaponSkillMapRow* FindWeaponSkillMapRow(const FGameplayTag& InWeaponTag) const;
//	FSkillRow* FindSkillRow(const FGameplayTag& InSkillID) const;
//	FEffectRow* FindEffectRow(const FName& InEffectID) const;
//	FVfxRow* FindVfxRow(const FName& InVfxID) const;
//
//	/* 실행 단계 : SkillRow를 읽어 EffectID 목록을 얻고, 각 EffectRow를 순서대로 실행 */
//	bool ExecuteSkillRow(const FSkillRow& InRow, const FSkillContext& InContext, AActor* InTargetActor);
//	bool ExecuteEffectRow(const FName& InEffectID, const FEffectRow& InRow, const FSkillContext& InContext, AActor* InTargetActor);
//
//	/* 타겟 처리 : 이 EffectType은 TargetActor 없이도 실행 가능한가 (SpawnArea/범위지정/자기중심) */
//	bool CanExecuteWithoutTargetActor(const FEffectRow& InRow) const;
//
//	/* 타겟 구성 : Row의 TargetType/Radius/Filter 기준으로 적용 대상 목록 */
//	bool BuildTargets(const FEffectRow& InRow, const FSkillContext& InContext, AActor* InTargetActor, TArray<TWeakObjectPtr<AActor>>& OutTargets) const;
//
//	/* 타겟 필터 : FilterTag로 대상 목록을 걸러냄 */
//	void ApplyTargetFilter(const FGameplayTag& InFilterTag, TArray<TWeakObjectPtr<AActor>>& InOutTargets) const;
//
//	/* 적용 : EffectType 분기(Damage/Heal/Debuff/SpawnArea/Projectile 등)로 실제 게임 결과를 적용 */
//	void ApplyEffectToTargets(const FEffectRow& InRow, const FSkillContext& InContext, const TArray<TWeakObjectPtr<AActor>>& InTargets);
//
//	/* 연출 : VfxIDs를 실행 */
//	void ExecuteVfxByIDs(const TArray<FName>& InVfxIDs, const FSkillContext& InContext, AActor* InTargetActor, const TArray<TWeakObjectPtr<AActor>>& InTargets);
//
//	/* 풀링 : 액터를 풀에서 꺼냄 (장판/투사체) */
//	void SpawnAreaFromPool(const FEffectRow& InRow, const FSkillContext& InContext, const TArray<TWeakObjectPtr<AActor>>& InTargets);
//	void SpawnProjectileFromPool(const FEffectRow& InRow, const FSkillContext& InContext, AActor* InTargetActor, const TArray<TWeakObjectPtr<AActor>>& InTargets);
//
//private:
//	UPROPERTY(Transient)
//	TObjectPtr<UDataTable> WeaponSkillMapTable = nullptr;
//
//	UPROPERTY(Transient)
//	TObjectPtr<UDataTable> SkillTable = nullptr;
//
//	UPROPERTY(Transient)
//	TObjectPtr<UDataTable> EffectTable = nullptr;
//
//	UPROPERTY(Transient)
//	TObjectPtr<UDataTable> VfxTable = nullptr;
//
//	UPROPERTY(Transient)
//	TObjectPtr<UPoolingComponent> AreaPool = nullptr;
//
//	UPROPERTY(Transient)
//	TObjectPtr<UPoolingComponent> ProjectilePool = nullptr;
//};
