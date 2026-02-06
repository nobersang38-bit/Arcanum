// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PoolSpawnManager.generated.h"

// 김도현
// 어떤 것을 스폰할지 정하는 열거형
UENUM(BlueprintType)
enum class ESpawnType : uint8
{
	None = 0		UMETA(DisplayName = "None"),
	Actor			UMETA(DisplayName = "Actor"),
	SceneComponent	UMETA(DisplayName = "SceneComponent"),
};

// 김도현
// 스폰을 직접 실행할 오브젝트
UCLASS()
class ARCANUM_API UPoolSpawnManager : public UObject
{
	GENERATED_BODY()
	friend class UPoolingComponent;
public:
	FORCEINLINE TSubclassOf<UObject> GetSpawnClass() const
	{
		return SpawnClass;
	}

	FORCEINLINE ESpawnType GetSpawnType() const
	{
		return SpawnType;
	}

	UObject* Spawn(const FTransform& InTransform);

private:
	//어떤 클래스를 만들고 싶은가
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSetting", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UObject> SpawnClass = nullptr;

	//어떤 방식의 스폰을 할것인가
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnSetting", meta = (AllowPrivateAccess = "true"))
	ESpawnType SpawnType = ESpawnType::None;

private:
	class UPoolingComponent* PoolingComponent = nullptr;
	FTransform SpawnTransform;

	AActor* ActorSpawn();

	USceneComponent* SceneComponentSpawn();
};
