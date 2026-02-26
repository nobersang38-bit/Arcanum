// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "Data/DataAssets/DATargetPriorityWeight.h"
#include "BaseUnitData.generated.h"

class UATTAnimPlayData;

// 김도현
// 런타임 중의 데이터
USTRUCT(BlueprintType)
struct FUnitRuntimeData
{
	GENERATED_BODY()
public:
	// 나를 때리려고하는 적은 누구누구인가
	UPROPERTY()
	TSet<TWeakObjectPtr<AActor>> AttackingTargets;

	// 내가 발견한 엘리트 적, 엘리트 적이 없다면 비어있음
	UPROPERTY()
	TSet<TWeakObjectPtr<AActor>> Elites;
};

// 김도현
// AI 틱 최적화 구조체
USTRUCT(BlueprintType)
struct FAITickParams
{
	GENERATED_BODY()
public:
	// 타겟을 찾을때 틱, 범위
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TickParam")
	FVector2D UpdateIntervalRange = FVector2D(1.9f, 2.1f);

	// 스폰되었을때 계산 시작할 시간 범위
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TickParam")
	FVector2D InitialDelayRange = FVector2D(0.0f, 0.2f);
};

USTRUCT(BlueprintType)
struct FUnitInfoSetting
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info", meta = (Categories = "Arcanum.Unit"))
	FGameplayTag Tag;

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
struct FUnitDeadMontageSetting
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	TObjectPtr<class UAnimMontage> DeadMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	float DeactiveTime = 3.0f;
};

USTRUCT(BlueprintType)
struct FUnitDeadFrameSetting
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	TObjectPtr<UATTAnimPlayData> DeadMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	float DeactiveTime = 3.0f;
};


UENUM(BlueprintType)
enum class EAnimMode : uint8
{
	AnimBlueprint    UMETA(DisplayName = "AnimBlueprint"),
	AnimToTexture    UMETA(DisplayName = "AnimToTexture"),
};

USTRUCT(BlueprintType)
struct FUnitAnimSetting
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	EAnimMode AnimMode = EAnimMode::AnimBlueprint;

#pragma region Mesh
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Body", meta = (EditCondition = "AnimMode == EAnimMode::AnimBlueprint", EditConditionHides))
	TSoftObjectPtr<class USkeletalMesh> SkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Body", meta = (EditCondition = "AnimMode == EAnimMode::AnimToTexture", EditConditionHides))
	TObjectPtr<class UStaticMesh> StaticMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Body", meta = (EditCondition = "AnimMode == EAnimMode::AnimToTexture", EditConditionHides))
	float MeshScale = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Body", meta = (EditCondition = "AnimMode == EAnimMode::AnimToTexture", EditConditionHides))
	float MeshAddHeight = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Material", meta = (EditCondition = "AnimMode == EAnimMode::AnimToTexture", EditConditionHides))
	int32 MaterialNum = 2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material", meta = (EditCondition = "AnimMode == EAnimMode::AnimToTexture", EditConditionHides))
	int32 StartFramePropertyIDX = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Material", meta = (EditCondition = "AnimMode == EAnimMode::AnimToTexture", EditConditionHides))
	int32 EndFramePropertyIDX = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Material", meta = (EditCondition = "AnimMode == EAnimMode::AnimToTexture", EditConditionHides))
	FVector2D DefaultAnimFrameRange;
#pragma endregion

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim", meta = (EditCondition = "AnimMode == EAnimMode::AnimBlueprint", EditConditionHides))
	TSubclassOf<class UAnimInstance> AnimInstance = nullptr;

#pragma region Loco
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim", meta = (EditCondition = "AnimMode == EAnimMode::AnimBlueprint", EditConditionHides))
	TObjectPtr<UAnimSequenceBase> Idle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim", meta = (EditCondition = "AnimMode == EAnimMode::AnimBlueprint", EditConditionHides))
	TObjectPtr<UAnimSequenceBase> Run;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim", meta = (EditCondition = "AnimMode == EAnimMode::AnimToTexture", EditConditionHides))
	TObjectPtr<UATTAnimPlayData> IdleATTData;
#pragma endregion


#pragma region Attacks
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim", meta = (EditCondition = "AnimMode == EAnimMode::AnimBlueprint", EditConditionHides))
	TArray<TObjectPtr<class UAnimMontage>> Attacks;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim", meta = (EditCondition = "AnimMode == EAnimMode::AnimToTexture", EditConditionHides))
	TArray<TObjectPtr<UATTAnimPlayData>> AttacksATTData;
#pragma endregion

#pragma region Hits
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim", meta = (EditCondition = "AnimMode == EAnimMode::AnimBlueprint", EditConditionHides))
	TArray<TObjectPtr<class UAnimMontage>> Hits;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim", meta = (EditCondition = "AnimMode == EAnimMode::AnimToTexture", EditConditionHides))
	TArray<TObjectPtr<UATTAnimPlayData>> HitsATTData;
#pragma endregion

#pragma region Deads
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim", meta = (EditCondition = "AnimMode == EAnimMode::AnimBlueprint", EditConditionHides))
	TArray<FUnitDeadMontageSetting> Deads;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim", meta = (EditCondition = "AnimMode == EAnimMode::AnimToTexture", EditConditionHides))
	TArray<FUnitDeadFrameSetting> DeadsFrameRange;
#pragma endregion
};

USTRUCT(BlueprintType)
struct FUnitAISetting
{
	GENERATED_BODY()
public:
	// 해당 유닛은 엘리트인가
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Info")
	bool bIsElite = false;

	// 한번 타겟을 공격시작하면 타겟을 변경할 수 있는가, false이면 틱마다 검사하니 AITickParams의 UpdataInterval여유있게 설정해야 함, true로 하는걸 추천
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	bool bUseLockTarget = true;

	// 최대 몇명을 감지할 것 인가
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	int32 MaxTargetCount = 10;

	// 때릴 수 있는 거리는 얼마인가, SquaredLength 기준 입니다
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float AttackRange = 10000.0f;

	// 공격 주기
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	float AttackRate = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	TSoftObjectPtr<class UBehaviorTree> BehaviorTree = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	FTargetPriorityWeightData TargetPriorityWeightData;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Anim")
	FUnitAnimSetting AnimSetting;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	FUnitAISetting AISetting;
};
