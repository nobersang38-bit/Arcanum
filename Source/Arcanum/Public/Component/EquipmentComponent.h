#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "EquipmentComponent.generated.h"

/*
 * 추영호
 * 장비 컴포넌트 
 * - 현재 장착 상태 관리
 * - EquipByTag로 꽂는다
 * - 일반무기/투구 2세트 스왑
 * - 전설무기/투구 1세트 임시 적용 + 원복
 */

UENUM(BlueprintType)
enum class EEquipSlot : uint8
{
	Weapon UMETA(DisplayName = "Weapon"),
	Helmet UMETA(DisplayName = "Helmet"),
	Armor  UMETA(DisplayName = "Armor"),
	Gloves UMETA(DisplayName = "Gloves"),
	Shoes  UMETA(DisplayName = "Shoes"),
};

/* 무기/투구 세트 */
USTRUCT(BlueprintType)
struct FWeaponHelmetSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag WeaponTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag HelmetTag;
};

/* 일반/전설 */
UENUM(BlueprintType)
enum class EWeaponSetMode : uint8
{
	Normal    UMETA(DisplayName = "Normal"),
	Legendary UMETA(DisplayName = "Legendary"),
};

/* 무기 슬롯이 바뀌면 WeaponTag로 WeaponSkillMapTable에서 SkillID를 다시 뽑아야 함
   FindSkillIDByWeaponTag(WeaponTag) -> SkillID */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquipChanged, EEquipSlot, InSlot, FGameplayTag, InNewEquipTag);

UCLASS(ClassGroup = (Arcanum), meta = (BlueprintSpawnableComponent))
class ARCANUM_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEquipmentComponent();

	/* 장비 장착 : InSlot에 InEquipTag를 저장하고 OnEquipChanged를 브로드캐스트  */
	UFUNCTION(BlueprintCallable, Category = "Arcanum|Equipment")
	bool EquipByTag(EEquipSlot InSlot, const FGameplayTag& InEquipTag);

	/* 장비 해제 : InSlot의 태그를 비우고 OnEquipChanged를 브로드캐스트 */
	UFUNCTION(BlueprintCallable, Category = "Arcanum|Equipment")
	bool UnEquip(EEquipSlot InSlot);

	/* 현재 장착 태그 조회 */
	UFUNCTION(BlueprintCallable, Category = "Arcanum|Equipment")
	FGameplayTag GetEquippedTag(EEquipSlot InSlot) const;

	/* ===============================
	 * 무기/투구 Set (일반+전설)
	 * =============================== */

	 /* 일반 세트(0/1) 설정 */
	UFUNCTION(BlueprintCallable, Category = "Arcanum|Equipment|Set")
	void SetNormalWeaponHelmetSet(int32 InIndex, const FWeaponHelmetSet& InSet);

	/* 전설 세트 설정 */
	UFUNCTION(BlueprintCallable, Category = "Arcanum|Equipment|Set")
	void SetLegendaryWeaponHelmetSet(const FWeaponHelmetSet& InSet);

	/* 일반 세트 스왑(0<->1), 무기+투구만 교체 */
	UFUNCTION(BlueprintCallable, Category = "Arcanum|Equipment|Set")
	bool SwapNormalWeaponSet();

	/* 전설 발동: 무기+투구만 전설로 교체(원복용으로 현재 일반세트 인덱스 저장) */
	UFUNCTION(BlueprintCallable, Category = "Arcanum|Equipment|Set")
	bool ActivateLegendaryWeaponSet();

	/* 전설 종료: 전설 발동 전 일반 무기+투구로 원복 */
	UFUNCTION(BlueprintCallable, Category = "Arcanum|Equipment|Set")
	bool DeactivateLegendaryWeaponSet();

	// Getter
	/* 현재 선택된 일반 무기/투구 세트 인덱스 */
	UFUNCTION(BlueprintCallable, Category = "Arcanum|Equipment|Set")
	int32 GetCurrentNormalSetIndex() const	{ return CurrentNormalSetIndex; }

	/* 전설 무기/투구 세트가 적용 중인지(전설 모드인지) */
	UFUNCTION(BlueprintCallable, Category = "Arcanum|Equipment|Set")
	bool IsLegendaryActive() const {return (CurrentSetMode == EWeaponSetMode::Legendary); }
	
	/* 스킬 : 현재 무기 태그(Weapon 슬롯) */
	UFUNCTION(BlueprintCallable, Category = "Arcanum|Equipment")
	FGameplayTag GetCurrentWeaponTag() const { return WeaponTag; }

public:
	/* 장비 교체 델리게이트 */
	UPROPERTY(BlueprintAssignable, Category = "Arcanum|Equipment")
	FOnEquipChanged OnEquipChanged;

protected:
	virtual void BeginPlay() override;

private:
	/* 현재 장착 무기 태그 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arcanum|Equipment", meta = (AllowPrivateAccess = "true"))
	FGameplayTag WeaponTag;

	/* 현재 장착 투구 태그 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arcanum|Equipment", meta = (AllowPrivateAccess = "true"))
	FGameplayTag HelmetTag;

	/* 현재 장착 갑옷 태그 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arcanum|Equipment", meta = (AllowPrivateAccess = "true"))
	FGameplayTag ArmorTag;

	/* 현재 장착 장갑 태그 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arcanum|Equipment", meta = (AllowPrivateAccess = "true"))
	FGameplayTag GlovesTag;

	/* 현재 장착 신발 태그 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arcanum|Equipment", meta = (AllowPrivateAccess = "true"))
	FGameplayTag ShoesTag;

	/* 일반 무기+투구 세트 2개 */
	UPROPERTY(EditDefaultsOnly, Category = "Arcanum|Equipment|Set", meta = (AllowPrivateAccess = "true"))
	FWeaponHelmetSet NormalSets[2];

	/* 전설 무기+투구 세트 1개 */
	UPROPERTY(EditDefaultsOnly, Category = "Arcanum|Equipment|Set", meta = (AllowPrivateAccess = "true"))
	FWeaponHelmetSet LegendarySet;

	/* 현재 일반 세트 인덱스(0/1) */
	UPROPERTY(VisibleAnywhere, Category = "Arcanum|Equipment|Set", meta = (AllowPrivateAccess = "true"))
	int32 CurrentNormalSetIndex = 0;

	/* 전설 발동 전 일반 세트 인덱스(원복용) */
	UPROPERTY(VisibleAnywhere, Category = "Arcanum|Equipment|Set", meta = (AllowPrivateAccess = "true"))
	int32 SavedNormalSetIndex = 0;

	/* 현재 세트 모드 */
	UPROPERTY(VisibleAnywhere, Category = "Arcanum|Equipment|Set", meta = (AllowPrivateAccess = "true"))
	EWeaponSetMode CurrentSetMode = EWeaponSetMode::Normal;
};

