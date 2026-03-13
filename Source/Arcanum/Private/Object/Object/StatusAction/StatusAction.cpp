// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Object/StatusAction/StatusAction.h"

void UStatusAction::StartAction_Implementation(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat)
{
	// 블루프린트나 여기에서 로직 작성
}

void UStatusAction::StopAction_Implementation(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat)
{
	// 블루프린트나 여기에서 로직 작성
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
