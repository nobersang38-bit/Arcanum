// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DataInfo/CommonData/Stats/FBattleStats.h"
#include "StatusAction.generated.h"

UENUM(BlueprintType)
enum class EStatusActionTagType : uint8
{
	OnlyThisTags				UMETA(DisplayName = "OnlyThisTags", ToolTip = "오로지 StatusAction의 태그만 사용합니다"),
	OnlyStatusActionCompTags	UMETA(DisplayName = "OnlyStatusActionCompTags", ToolTip = "오로지 StatusActionComponent의 태그만 사용합니다"),
	AddTags						UMETA(DisplayName = "AddTags", ToolTip = "StatusAction과 StatusActionComponent의 태그를 합칩니다"),
};

/**
 * 김도현
 */
UCLASS(Blueprintable)
class ARCANUM_API UStatusAction : public UObject
{
	GENERATED_BODY()

	friend class UStatusActionComponent;

public:
	// 처음 초기화시 실행될 로직
	UFUNCTION(BlueprintNativeEvent, Category = "StatusAction")
	void InitializeAction(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat);

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

	// 비활성화 태그들 추가
	UFUNCTION(BlueprintCallable, Category = "StatusAction")
	FORCEINLINE void AddDisableTags(const FGameplayTagContainer& InTags) { DisableTags = InTags; }

	// 비활성화 태그 추가
	UFUNCTION(BlueprintCallable, Category = "StatusAction")
	FORCEINLINE void AddDisableTag(const FGameplayTag& InTag) { DisableTags.AddTag(InTag); }

protected:
	UFUNCTION()
	void SetupTags();

	UFUNCTION()
	void Internal_InitializeAction(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat);

	// 이걸 우선 호출 후 검사 완료후에 StartAction을 실행 시켜야함
	UFUNCTION()
	void Internal_StartAction(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat);

	// 이걸 우선 호출 후 검사 완료후에 StopAction을 실행 시켜야함
	UFUNCTION()
	void Internal_StopAction(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat);

	// 지금 이 액션이 실행 될 수 있는가
	UFUNCTION()
	FORCEINLINE bool IsEnableAction(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat) 
	{ 
		return (RegenStat.ParentTag.IsValid() || NonRegenStat.TagName.IsValid()) && (EnableTags.HasTagExact(RegenStat.ParentTag) || EnableTags.HasTagExact(NonRegenStat.TagName));
	}
	
	// 지금 이 액션을 정지해야하는가
	UFUNCTION()
	FORCEINLINE bool IsDisableAction(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat) 
	{ 
		return (RegenStat.ParentTag.IsValid() || NonRegenStat.TagName.IsValid()) && (DisableTags.HasTagExact(RegenStat.ParentTag) || DisableTags.HasTagExact(NonRegenStat.TagName));
	}

	// EnableTags랑 다른 태그 컨테이너 합치기
	UFUNCTION()
	void AppendEnableTags(const FGameplayTagContainer& InMergeTagContainer);

	// DisableTags랑 다른 태그 컨테이너 합치기
	UFUNCTION()
	void AppendDisableTags(const FGameplayTagContainer& InMergeTagContainer);

	// EnableTags 교체
	void ChangeEnableTags(const FGameplayTagContainer& InChangeTagContainer);

	// DisableTags 교체
	void ChangeDisableTags(const FGameplayTagContainer& InChangeTagContainer);
	
	// EnableTags 교체
	void ChangeEnableTags(const FGameplayTag& InTag);

	// DisableTags 교체
	void ChangeDisableTags(const FGameplayTag& InTag);

	// 맨 처음에 설정한 EnableTags로 되돌리기
	UFUNCTION()
	void RestoreEnableTags();

	// 맨 처음에 설정한 DisableTags로 되돌리기
	UFUNCTION()
	void RestoreDisableTags();

protected:
	// 이 액션이 StatusActionComponent에 할당되면 StartAction을 한번 시작 할 것인가
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	bool bUseStartOnStartAction = true;

	// 이 액션이 StatusActionComponent에 할당되면 InitializeAction을 한번 시작 할 것인가
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	bool bUseStartOnInitializeAction = true;

	// StatusActionComponent에서 할당된 태그와의 병합 방식
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	EStatusActionTagType EnableTagMergeType = EStatusActionTagType::AddTags;

	// StatusActionComponent에서 할당된 태그와의 병합 방식
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	EStatusActionTagType DisableTagMergeType = EStatusActionTagType::OnlyThisTags;

	// 이 안의 태그중 하나라도 들어오면 액션 발동
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	FGameplayTagContainer EnableTags;

	// 이 안의 태그중 하나라도 들어오면 액션 취소
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	FGameplayTagContainer DisableTags;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Runtime|Debug", meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer OriginalEnableTags;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Runtime|Debug", meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer OriginalDisableTags;
};
