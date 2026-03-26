// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Object/StatusAction/StatusAction_PlayerHealthBonus.h"

UStatusAction_PlayerHealthBonus::UStatusAction_PlayerHealthBonus()
{
	EnableTagMergeType = EStatusActionTagType::OnlyStatusActionCompTags;
}

void UStatusAction_PlayerHealthBonus::StartAction_Implementation(const FRegenStat& RegenStat, const FNonRegenStat& NonRegenStat)
{
	
}
