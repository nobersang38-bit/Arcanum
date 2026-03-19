// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "NativeGameplayTags.h"
#include "UnitDragDropOperation.generated.h"

/**
 * 김도현
 */
UCLASS()
class ARCANUM_API UUnitDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	FGameplayTag UnitTag;

	UPROPERTY()
	TWeakObjectPtr<class UBattleAllyUnitSlotWidget> Slot = nullptr;
};
