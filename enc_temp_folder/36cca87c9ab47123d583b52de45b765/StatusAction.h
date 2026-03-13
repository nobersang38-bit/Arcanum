// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DataInfo/CommonData/Stats/FBattleStats.h"
#include "StatusAction.generated.h"

/**
 * 김도현
 */
UCLASS(Blueprintable)
class ARCANUM_API UStatusAction : public UObject
{
	GENERATED_BODY()
public:
	// 실행할때의 로직
	UFUNCTION(BlueprintNativeEvent, Category = "StatusAction")
	void StartAction(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat);

	// 중단될때의 로직, 지속적인 액션인 경우에 끝날때 호출 해줘야함
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "StatusAction")
	void StopAction(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat);

	// 활성화 태그들 추가
	UFUNCTION(BlueprintCallable, Category = "StatusAction")
	FORCEINLINE void AddEnableTags(const FGameplayTagContainer& InTags) { EnableTags = InTags; }

	// 활성화 태그 추가
	UFUNCTION(BlueprintCallable, Category = "StatusAction")
	FORCEINLINE void AddEnableTag(const FGameplayTag& InTag) { EnableTags.AddTag(InTag); }

protected:
	// 이걸 우선 호출 후 검사 완료후에 StartAction을 실행 시켜야함
	UFUNCTION()
	void Internal_StartAction(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat);

	// 이걸 우선 호출 후 검사 완료후에 StopAction을 실행 시켜야함
	UFUNCTION()
	void Internal_StopAction(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat);

	// 지금 이 액션이 실행 될 수 있는가
	UFUNCTION()
	FORCEINLINE bool IsEnableAction(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat) { return EnableTags.HasTag(RegenStat.ParentTag) || EnableTags.HasTag(NonRegenStat.TagName); }
	
	// 지금 이 액션을 정지해야하는가
	UFUNCTION()
	FORCEINLINE bool IsDisableAction(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat) { return DisableTags.HasTag(RegenStat.ParentTag) || DisableTags.HasTag(NonRegenStat.TagName); }

protected:
	// 인스턴스로 만들 것 인가, 지속적인 액셔인 경우에 인스턴스로 해야 합니다
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	bool bUseInstancing = true;

	// 이 안의 태그중 하나라도 들어오면 액션 발동
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	FGameplayTagContainer EnableTags;

	// 이 안의 태그중 하나라도 들어오면 액션 취소ko
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	FGameplayTagContainer DisableTags;
};
