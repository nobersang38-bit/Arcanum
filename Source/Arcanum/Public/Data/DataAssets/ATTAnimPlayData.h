// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ATTAnimPlayData.generated.h"

UENUM(BlueprintType)
enum class ESelectNotifyType : uint8
{
	AnimNotify		UMETA(DisplayName = "AnimNotify"),
	AnimNotifyState	UMETA(DisplayName = "AnimNotifyState"),
};

USTRUCT(BlueprintType)
struct FATTNotifySetting
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float NotifyPlayTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ESelectNotifyType SelectNotifyType = ESelectNotifyType::AnimNotify;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "SelectNotifyType == ESelectNotifyType::AnimNotify", EditConditionHides))
	TSubclassOf<UAnimNotify> AnimNotify = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "SelectNotifyType == ESelectNotifyType::AnimNotifyState", EditConditionHides))
	TSubclassOf<UAnimNotifyState> AnimNotifyState = nullptr;
};
/**
 * 
 */
UCLASS()
class ARCANUM_API UATTAnimPlayData : public UDataAsset
{
	GENERATED_BODY()
public:
	void PlayAnim(TArray<UMaterialInstanceDynamic*> SetMaterials);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName StartFrameParameterName = FName("StartFrame");

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName EndFrameParameterName = FName("EndFrame");

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FIntPoint FrameRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FATTNotifySetting> SetNotify;
};
