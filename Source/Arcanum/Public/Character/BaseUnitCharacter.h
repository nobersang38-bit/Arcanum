// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/TeamInterface.h"
#include "BaseUnitCharacter.generated.h"

UCLASS()
class ARCANUM_API ABaseUnitCharacter : public ACharacter, public ITeamInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseUnitCharacter();

public:
	virtual FGameplayTag GetTeamTag_Implementation() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	// 태그가 설정되어 있으면 AIC에 있는 태그보다 우선으로 처리합니다, 테스트용
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "Arcanum.TeamID"))
	FGameplayTag TeamID;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
