// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/SkeletalMesh.h"
#include "Animation/AnimBlueprint.h"
#include "CharacterData.generated.h"

/**
 * 김유진 - 캐릭터 데이터 테이블용 구조체
 */
USTRUCT(BlueprintType)
struct FCharacterData : public FTableRowBase{
public:
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Character")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	float Mana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Mesh")
	USkeletalMesh* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Mesh")
	UAnimBlueprint* AnimBlueprint;
};