// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NativeGameplayTags.h"
#include "BasementTeamComponent.generated.h"

//김도현
// 배틀필드매니저 서브시스템에 기지가 자신의 소속을 등록
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANUM_API UBasementTeamComponent : public UActorComponent
{
	GENERATED_BODY()
#pragma region 언리얼 기본설정
public:
	// Sets default values for this component's properties
	UBasementTeamComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
#pragma endregion

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting")
	FGameplayTag TeamTag;
};
