// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "NativeGameplayTags.h"
#include "PoolingSubsystem.generated.h"

USTRUCT()
struct FPoolItemsData
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TSet<AActor*> Items;
};

USTRUCT()
struct FPoolingSet
{
	GENERATED_BODY()
public:
	// 초기 생성 개수
	int32 DefaultPoolSize = 20;

	UPROPERTY()
	TMap<FGameplayTag, TSubclassOf<AActor>> PoolSet;
};
/**
 * 김도현
 */
UCLASS()
class ARCANUM_API UPoolingSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	// 풀링 초기설정
	void SetPoolSet(const FPoolingSet& SetData);

	// 비활성화인 액터가 있다면 활성화 시키고, 비활성화인 액터가 없다면 스폰 (세팅에 설정되어 있어야 사용가능)
	AActor* SpawnFromPool(const FGameplayTag& SpawnTag, const FTransform& InTransform);
	// 풀링에 해당 오브젝트 추가하는 설정도 포함 (세팅에 설정 안되어 있어도 사용 가능)
	AActor* SpawnFromPool(const FGameplayTag& SpawnTag, TSubclassOf<AActor> InClass, const FTransform& InTransform);
	// 풀링에 해당 오브젝트를 추가하는 설정도 포함 (세팅에 설정 안되어 있어도 사용 가능)
	AActor* SpawnFromPool(TSubclassOf<AActor> InClass, const FTransform& InTransform);

	// 아이템 비활성화 
	bool DeactiveItem(AActor* InActor);

	// 아이템 삭제
	void DestroyItem(AActor* InActor);


protected:
	void Internal_ActivateItem(AActor* InActor, const FTransform& InTransform);
	void Internal_DeactiveItem(AActor* InActor);

protected:
	UPROPERTY()
	TMap<FName, FPoolItemsData> ActivateItems;

	UPROPERTY()
	TMap<FName, FPoolItemsData> DeactivedItems;

	UPROPERTY()
	TMap<AActor*, FName> ActorToTagMap;

	UPROPERTY()
	TMap<UClass*, FName> ClassToTagMap;

	FPoolingSet PoolingSet;

	UPROPERTY()
	TWeakObjectPtr<class APostProcessVolume> CashedPostProcessVolume = nullptr;
};
