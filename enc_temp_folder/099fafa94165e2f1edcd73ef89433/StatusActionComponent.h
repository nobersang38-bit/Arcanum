// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NativeGameplayTags.h"
#include "StatusActionComponent.generated.h"

//김도현
class UStatusAction;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANUM_API UStatusActionComponent : public UActorComponent
{
	GENERATED_BODY()
#pragma region 언리얼 엔진 기본 생성
public:
	// Sets default values for this component's properties
	UStatusActionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

#pragma endregion

public:
	UFUNCTION()
	void SetupAction();


protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag, TSubclassOf<UStatusAction>> ActionSet;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Runtime|Debug", meta = (AllowPrivateAccess = "true"))
	TMap<FGameplayTag, UStatusAction*> Actions;

	TMap<FGameplayTag, FDelegateHandle> ActionDelegates;

	UPROPERTY()
	TWeakObjectPtr<class UCharacterBattleStatsComponent> CachedCharacterBattleStatsComponent = nullptr;
};
