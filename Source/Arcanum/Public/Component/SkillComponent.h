#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "Component/EquipmentComponent.h"
#include "SkillComponent.generated.h"

/*
 * 추영호
 * - 무기 태그 -> SkillID 선택
 * - SkillRow/EffectRow 실행
 * - 월드 스폰/풀링은 여기서
 * - 각성 트리거용 OnWeaponSkillExecuted 브로드캐스트
 */

class UDataTable;
struct FSkillRow;

/* 무기 스킬 실행됨 (각성 트리거 발동) */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponSkillExecuted, AActor*, InInstigator, FGameplayTag, InSkillIDTag);

UCLASS(ClassGroup = (Arcanum), meta = (BlueprintSpawnableComponent))
class ARCANUM_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USkillComponent();

protected:
	virtual void BeginPlay() override;

public:
	/* 일반/현재 무기 스킬 실행 (현재 장착 WeaponTag 기준) */
	UFUNCTION(BlueprintCallable, Category = "Arcanum|Skill")
	bool ActivateWeaponSkill();

	/* 전설 버튼: 장비(무기/투구) 전설로 바꾸고 전설 스킬 실행 */
	UFUNCTION(BlueprintCallable, Category = "Arcanum|Skill")
	bool ActivateLegendaryWeaponSkill();

	/* 전설 스킬 종료 시점에 호출(애님 노티파이/이펙트 종료 콜백 등) -> 원래 일반 무기/투구로 원복 */
	UFUNCTION(BlueprintCallable, Category = "Arcanum|Skill")
	void EndLegendaryWeaponSkill();

	/* 외부에서 장비컴포 주입 */
	UFUNCTION(BlueprintCallable, Category = "Arcanum|Skill")
	void SetEquipmentComponent(UEquipmentComponent* InEquipmentComponent);

public:
	UPROPERTY(BlueprintAssignable, Category = "Arcanum|Skill")
	FOnWeaponSkillExecuted OnWeaponSkillExecuted;

private:
	/* 장비 교체 콜백 */
	UFUNCTION()
	void HandleEquipChanged(EEquipSlot InSlot, FGameplayTag InNewEquipTag);

private:
	/* 바인딩/캐시 */
	void BindEquipment();
	void BuildCaches();

	/* WeaponTag -> SkillID */
	bool ResolveSkillIDByWeaponTag(const FGameplayTag& InWeaponTag, FGameplayTag& OutSkillIDTag) const;

	/* SkillIDTag -> SkillRow */
	const FSkillRow* FindSkillRowBySkillID(const FGameplayTag& InSkillIDTag) const;

	/* SkillRow 실행 (EffectIDs 순회) */
	bool ExecuteSkillRow(const FSkillRow& InSkillRow);

private:
	UPROPERTY(Transient)
	TObjectPtr<UEquipmentComponent> EquipmentComponent;

	/* 무기->스킬 매핑 테이블 (WeaponTag -> CommonSkillID/LegendarySkillID) */
	UPROPERTY(EditDefaultsOnly, Category = "Arcanum|Skill|Data")
	TObjectPtr<UDataTable> WeaponSkillMapTable;

	/* 스킬 마스터 테이블 */
	UPROPERTY(EditDefaultsOnly, Category = "Arcanum|Skill|Data")
	TObjectPtr<UDataTable> SkillTable;

	/* 이펙트 마스터 테이블 (EffectID = RowName) */
	UPROPERTY(EditDefaultsOnly, Category = "Arcanum|Skill|Data")
	TObjectPtr<UDataTable> EffectTable;

	/* 현재 무기 기준 스킬ID */
	UPROPERTY(VisibleAnywhere, Category = "Arcanum|Skill")
	FGameplayTag CurrentWeaponSkillIDTag;

	/* 전설 스킬 사용중 */
	UPROPERTY(VisibleAnywhere, Category = "Arcanum|Skill")
	bool bLegendaryActive = false;

private:
	/* 런타임 캐시 (UPROPERTY 없음) */
	TMap<FGameplayTag, FGameplayTag> WeaponTagToSkillIDCache;
	TMap<FGameplayTag, FName> SkillIDToRowNameCache;

	bool bCachesBuilt = false;
};
