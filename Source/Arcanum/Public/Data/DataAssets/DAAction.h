// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataInfo/CommonData/Stats/FBattleStats.h"
#include "NativeGameplayTags.h"
#include "DAAction.generated.h"

/**
 * 김도현
 */
UCLASS()
class ARCANUM_API UDAAction : public UDataAsset
{
	GENERATED_BODY()
public:
	virtual void StartAction(const FRegenStat& Stat);
	virtual void StartAction(const FNonRegenStat& Stat);

	FORCEINLINE void AddEnableTags(const TSet<FGameplayTag>& InTags) { EnableTags = InTags; }
	FORCEINLINE void AddEnableTag(const FGameplayTag& InTag) { EnableTags.Add(InTag); }

protected:
	FORCEINLINE bool IsEnable(const FRegenStat& Stat) { return EnableTags.Contains(Stat.ParentTag); }
	FORCEINLINE bool IsEnable(const FNonRegenStat& Stat) { return EnableTags.Contains(Stat.TagName); }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSet<FGameplayTag> EnableTags;
};
