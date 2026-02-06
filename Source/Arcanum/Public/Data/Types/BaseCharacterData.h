// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseCharacterData.generated.h"

// 김도현
// 아군 유닛, 적군 유닛이 가져야할 공용 정보
USTRUCT(BlueprintType)
struct FBaseCharacterData
{
	GENERATED_BODY()
protected:
	// 해당 객체의 이름
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Name = FText::FromString(TEXT("이름"));

	// 해당 객체의 설명
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Description = FText::FromString(TEXT("원거리 공격이 가능하나, 체력이 낮다"));

	// 인게임의 유닛을 선택하거나 작게 보여줄 아이콘
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> Icon = nullptr;

	// 대량 1:2비율의 인물화같은 느낌의 텍스처, 캐릭터 선택창의 캐릭터 선택 패널등에 사용
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> Portrait = nullptr;

	// Todo : 나중에 베이스 클래스로 변경해야 함
	// 해당 유닛의 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> ActorType = nullptr;
};
