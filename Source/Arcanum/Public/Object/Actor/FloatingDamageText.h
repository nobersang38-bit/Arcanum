// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/PoolingInterface.h"
#include "FloatingDamageText.generated.h"

UCLASS()
class ARCANUM_API AFloatingDamageText : public AActor, public IPoolingInterface
{
	GENERATED_BODY()

#pragma region 언리얼 기본 생성
public:
	// Sets default values for this actor's properties
	AFloatingDamageText();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
#pragma endregion
public:
	void SetText(float InValue);

	void SetText(FText InText);

protected:
	void ActivateItem() override;

	void DeactiveItem() override;

	UFUNCTION()
	void Internal_Deactive();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UWidgetComponent> FloatingWidgetComponent = nullptr;

	UPROPERTY()
	TWeakObjectPtr<class UFloatingDamageTextWidget> CashedFloatingDamageTextWidget = nullptr;

};
