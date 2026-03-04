// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTags/ArcanumTags.h"
#include "Interface/TeamInterface.h"
#include "PlayerCharacter.generated.h"

/*
	김유진 - 기본 플레이어 캐릭터
*/

UCLASS()
class ARCANUM_API APlayerCharacter : public ACharacter, public ITeamInterface
	//, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

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

protected:

	// 스프링암
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<class USpringArmComponent> SpringArm = nullptr;

	// 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<class UCameraComponent> Camera = nullptr;

	// 캐릭터 태그
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	FGameplayTagContainer GameplayTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tags")
	FGameplayTag TeamTag = Arcanum::Unit::Faction::Ally::Root;

protected:
	// Todo 데이터 구조체로 변경해야함
	float CurrentHealth = 300.0f;
	float MaxHealth = 300.0f;
};
