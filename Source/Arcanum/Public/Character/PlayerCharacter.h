// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTags/ArcanumTags.h"
#include "Interface/TeamInterface.h"
#include "DataInfo/BattleCharacter/BattleStats/Data/FGradeStatData.h"
#include "Interface/StatModifierInterface.h"
#include "Interface/StatInterface.h"
#include "Data/Types/BTPlayerStruct.h"
#include "PlayerCharacter.generated.h"

/*
	김유진 - 기본 플레이어 캐릭터
*/

UCLASS()
class ARCANUM_API APlayerCharacter : public ACharacter, public ITeamInterface, public IStatModifierInterface, public IStatInterface
	//, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	void SetAutoMode(class ABattlePlayerController* MainController, bool bIsAuto);
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual FGameplayTag GetTeamTag() override;

	UFUNCTION()
	void RecievedDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	
public:
	// ID 태그 바꾸는 함수
	UFUNCTION(BlueprintCallable, Category = "Tags")
	void SetIDTag(FGameplayTag NewID);

	// 태그 출력하는 함수
	UFUNCTION(BlueprintCallable, Category = "Tags")
	void PrintIDTag();
	
#pragma region 디버그
	void PlayerBasicAttack();
#pragma endregion

#pragma region 스탯
	void AddCurrentStat(FGameplayTag InTag, float InValue);

#pragma endregion


protected:

	// 스프링암
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<class USpringArmComponent> SpringArm = nullptr;

	// 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<class UCameraComponent> Camera = nullptr;

	// 스탯 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	TObjectPtr<class UCharacterBattleStatsComponent> StatComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "StatusAction")
	TObjectPtr<class UStatusActionComponent> StatusActionComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting|AI")
	TObjectPtr<class UBehaviorTree> BehaviorTree = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting|AI")
	FName BlackboardBasicAttackName = FName("BasicAttack");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting|AI")
	FName BlackboardBasicSkillName = FName("BasicSkill");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting|AI")
	FName BlackboardUltimateSkillName = FName("UltimateSkill");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting|AI")
	FName BlackboardItem01Name = FName("Item01");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting|AI")
	FName BlackboardItem02Name = FName("Item02");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting|AI")
	FName BlackboardSwapName = FName("Swap");

	// 캐릭터 태그
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer GameplayTags;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tags")
	FGameplayTag TeamTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
	FGameplayTag HealthTag = Arcanum::BattleStat::Character::Regen::Health::Root;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
	FGameplayTag ManaTag = Arcanum::BattleStat::Character::Regen::Mana::Root;

protected:
	//UPROPERTY()
	//TMap<FGameplayTag, FRegenStat> RegenStats;

	//UPROPERTY()
	//TMap<FGameplayTag, FNonRegenStat> NonRegenStats;


	// IStatModifierInterface을(를) 통해 상속됨
	void AddLevelModifierEntry(const FLevelModifierEntry& LevelModifierEntry) override;

	void AddDerivedStatModifier(const FDerivedStatModifier& DerivedStatModifier) override;


	// IStatModifierInterface을(를) 통해 상속됨
	void ChangeStat(const FGameplayTag& InTag, float InValue) override;


	// IStatInterface을(를) 통해 상속됨
	const UCharacterBattleStatsComponent* GetStatComponent() const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class AAIController> CachedAIC = nullptr;

};
