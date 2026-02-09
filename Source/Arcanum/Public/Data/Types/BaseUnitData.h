// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseUnitData.generated.h"

// 김도현
// AI 틱 최적화 구조체
USTRUCT(BlueprintType)
struct FAITickParams
{
	GENERATED_BODY()
public:
	// 타겟을 찾을때 틱
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Test")
	float UpdateInterval = 2.0f;

	// 서로 계산할때 같은시간에 연산 안하게 오차범위
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Test")
	float UpdateIntervalMargin = 0.2f;

	// 스폰되었을때 계산 시작할 범위
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Test")
	FVector2D InitialDelayRange = FVector2D(0.0f, 0.2f);
};

USTRUCT(BlueprintType)
struct FUnitInfoSetting
{
	GENERATED_BODY()
public:
	// 해당 유닛의 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
	TSubclassOf<class ABaseUnitCharacter> CharacterType = nullptr;

	// 해당 객체의 이름
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
	FText Name = FText::FromString(TEXT("이름"));

	// 해당 객체의 설명
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
	FText Description = FText::FromString(TEXT("원거리 공격이 가능하나, 체력이 낮다"));

	// 인게임의 유닛을 선택하거나 작게 보여줄 아이콘
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
	TSoftObjectPtr<UTexture2D> Icon = nullptr;

	// 대량 1:2비율의 인물화같은 느낌의 텍스처, 캐릭터 선택창의 캐릭터 선택 패널등에 사용
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
	TSoftObjectPtr<UTexture2D> Portrait = nullptr;
};

USTRUCT(BlueprintType)
struct FUnitBodySetting
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Body")
	TSoftObjectPtr<class USkeletalMesh> SkeletalMesh = nullptr;
};

USTRUCT(BlueprintType)
struct FUnitAnimSetting
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	TSubclassOf<class UAnimInstance> AnimInstance = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	TArray<TObjectPtr<class UAnimMontage>> Attacks;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	TArray<TObjectPtr<class UAnimMontage>> Hits;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	TArray<TObjectPtr<class UAnimMontage>> Deaths;
};

USTRUCT(BlueprintType)
struct FUnitAISetting
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	TSoftObjectPtr<class UBehaviorTree> BehaviorTree = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	TSoftObjectPtr<class UDATargetPriorityWeight> TargetPriorityWeight = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	FName BBTargetActorName = FName(TEXT("TargetActor"));

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	FAITickParams AITickParams;
};

// 김도현
// 아군 유닛, 적군 유닛이 가져야할 공용 정보
USTRUCT(BlueprintType)
struct FBaseUnitData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
	FUnitInfoSetting InfoSetting;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Body")
	FUnitBodySetting BodySetting;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	FUnitAnimSetting AnimSetting;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	FUnitAISetting AISetting;
};
