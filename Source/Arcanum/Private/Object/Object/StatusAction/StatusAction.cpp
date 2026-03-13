// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Object/StatusAction/StatusAction.h"

void UStatusAction::InitializeAction_Implementation(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat)
{
	// 블루프린트나 여기에서 로직 작성
}

void UStatusAction::StartAction_Implementation(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat)
{
	// 블루프린트나 여기에서 로직 작성
}

void UStatusAction::StopAction_Implementation(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat)
{
	// 블루프린트나 여기에서 로직 작성
}

void UStatusAction::SetupTags()
{
	if (OriginalEnableTags.IsEmpty())
	{
		OriginalEnableTags = EnableTags;
	}

	if (OriginalDisableTags.IsEmpty())
	{
		OriginalDisableTags = DisableTags;
	}
}

void UStatusAction::Internal_InitializeAction(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat)
{
	if (!bUseStartOnInitializeAction) return;
	InitializeAction(RegenStat, NonRegenStat);
}

void UStatusAction::Internal_StartAction(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat)
{
	if (!IsEnableAction(RegenStat, NonRegenStat)) return;
	StartAction(RegenStat, NonRegenStat);
}

void UStatusAction::Internal_StopAction(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat)
{
	if (!IsDisableAction(RegenStat, NonRegenStat)) return;
	StopAction(RegenStat, NonRegenStat);
}

void UStatusAction::AppendEnableTags(const FGameplayTagContainer& InMergeTagContainer)
{
	SetupTags();
	EnableTags = OriginalEnableTags;
	EnableTags.AppendTags(InMergeTagContainer);
}

void UStatusAction::AppendDisableTags(const FGameplayTagContainer& InMergeTagContainer)
{
	SetupTags();
	DisableTags = OriginalDisableTags;;
	DisableTags.AppendTags(InMergeTagContainer);
}

void UStatusAction::ChangeEnableTags(const FGameplayTagContainer& InChangeTagContainer)
{
	SetupTags();
	EnableTags = InChangeTagContainer;
}

void UStatusAction::ChangeDisableTags(const FGameplayTagContainer& InChangeTagContainer)
{
	SetupTags();
	DisableTags = InChangeTagContainer;
}

void UStatusAction::ChangeEnableTags(const FGameplayTag& InTag)
{
	SetupTags();
	EnableTags.Reset();
	EnableTags.AddTag(InTag);
}

void UStatusAction::ChangeDisableTags(const FGameplayTag& InTag)
{
	SetupTags();
	DisableTags.Reset();
	DisableTags.AddTag(InTag);
}

void UStatusAction::RestoreEnableTags()
{
	EnableTags = OriginalEnableTags;
}

void UStatusAction::RestoreDisableTags()
{
	DisableTags = OriginalDisableTags;
}
