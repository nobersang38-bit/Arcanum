// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/Types/BaseUnitData.h"
#include "UnitCombatComponent.generated.h"


// 김도현
// 전투 컴포넌트(AI컴포넌트)
class USphereComponent;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANUM_API UUnitCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UUnitCombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	void DeferredBeginPlay();

	UFUNCTION()
	void TickUpdate();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TWeakObjectPtr<USphereComponent> DetectComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FUnitAISetting UnitAISetting;

private:
	FTimerHandle TickTimerHandle;
};
