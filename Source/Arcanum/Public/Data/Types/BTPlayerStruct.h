// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NativeGameplayTags.h"
#include "BTPlayerStruct.generated.h"

UENUM(BlueprintType)
enum class EBSkillType : uint8
{
	None			UMETA(DisplayName = "None"),
	BasicAttack		UMETA(DisplayName = "BasicAttack"),
	BasicSkill		UMETA(DisplayName = "BasicSkill"),
	UltimateSkill	UMETA(DisplayName = "UltimateSkill"),
	Item01			UMETA(DisplayName = "Item01"),
	Item02			UMETA(DisplayName = "Item02"),
	Swap			UMETA(DisplayName = "Swap"),
};
/**
 * 김도현
 */
UCLASS()
class UBTPlayerDataObject : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Runtime")
	EBSkillType SkillType = EBSkillType::None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Runtime")
	TWeakObjectPtr<class ABattlePlayerController> PlayerController = nullptr;

	bool CostCheck();
	void UseSkill();
};
