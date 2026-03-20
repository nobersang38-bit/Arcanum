// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GachaHighgradeActor.generated.h"

class UWidgetComponent;
class UGachaHighgradeWidget;

struct FGachaItemResult;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSilhouetteStartActor);

UCLASS()
class ARCANUM_API AGachaHighgradeActor : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable) FOnSilhouetteStartActor OnSilhouetteStart;
	
	AGachaHighgradeActor();
	UGachaHighgradeWidget* GetWidget() const { return CachedWidget; }

protected:
	UFUNCTION() void HandleSilhouetteStart();
	
	UPROPERTY(VisibleAnywhere) UWidgetComponent* WidgetComp;
	UPROPERTY() UGachaHighgradeWidget* CachedWidget;
public:
	void InitializeWidget(const FGachaItemResult& ItemData, const FText& Dialog);

};
