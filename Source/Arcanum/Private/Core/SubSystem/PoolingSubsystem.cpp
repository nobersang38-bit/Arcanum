// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/SubSystem/PoolingSubsystem.h"
#include "Interface/PoolingInterface.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/PostProcessVolume.h"

void UPoolingSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	TArray<AActor*> PostProcessVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APostProcessVolume::StaticClass(), PostProcessVolumes);

	if(!PostProcessVolumes.IsEmpty())
	{
		CashedPostProcessVolume = Cast<APostProcessVolume>(PostProcessVolumes[0]);
	}
}

void UPoolingSubsystem::SetPoolSet(const FPoolingSet& SetData)
{
	// ==========================================
	// 1. 기존 데이터 초기화 (Cleanup)
	// ==========================================

	// 관리 중이던 모든 액터를 월드에서 파괴
	for (const auto& Pair : ActorToTagMap)
	{
		AActor* PooledActor = Pair.Key;
		if (IsValid(PooledActor))
		{
			PooledActor->Destroy();
		}
	}

	// 모든 관리용 컨테이너 비우기
	ActivateItems.Empty();
	DeactivedItems.Empty();
	ActorToTagMap.Empty();
	ClassToTagMap.Empty();

	// ==========================================
	// 2. 새로운 풀 데이터 세팅 및 스폰
	// ==========================================

	PoolingSet = SetData;

	for (const TTuple<FGameplayTag, TSubclassOf<AActor>>& Pair : PoolingSet.PoolSet)
	{
		FName PoolKey = Pair.Key.GetTagName();
		TSubclassOf<AActor> ActorClass = Pair.Value;

		if (!ActorClass) continue;

		// 클래스와 키 매핑 등록
		ClassToTagMap.Add(ActorClass, PoolKey);

		for (int32 i = 0; i < PoolingSet.DefaultPoolSize; ++i)
		{
			if (AActor* NewActor = GetWorld()->SpawnActor<AActor>(ActorClass))
			{
				ActorToTagMap.Add(NewActor, PoolKey);
				Internal_DeactiveItem(NewActor); // 스폰 직후 비활성화 상태로 만들기
				DeactivedItems.FindOrAdd(PoolKey).Items.Add(NewActor);
			}
		}
	}
}

AActor* UPoolingSubsystem::SpawnFromPool(const FGameplayTag& SpawnTag, const FTransform& InTransform)
{
	FName PoolKey = SpawnTag.GetTagName();

	// 1. 비활성화된 풀에 여유가 있는지 확인
	if (DeactivedItems.Contains(PoolKey) && DeactivedItems[PoolKey].Items.Num() > 0)
	{
		auto It = DeactivedItems[PoolKey].Items.CreateIterator();
		AActor* PooledActor = *It;
		It.RemoveCurrent();

		if (IsValid(PooledActor))
		{
			//UE_LOG(LogTemp, Warning, TEXT("풀링 활성화"));
			ActivateItems.FindOrAdd(PoolKey).Items.Add(PooledActor);
			Internal_ActivateItem(PooledActor, InTransform);
			return PooledActor;
		}
	}

	// 2. 비활성 풀이 비어있다면 새로 스폰
	TSubclassOf<AActor>* ClassPtr = PoolingSet.PoolSet.Find(SpawnTag);
	if (ClassPtr && *ClassPtr)
	{
		if (AActor* NewActor = GetWorld()->SpawnActor<AActor>(*ClassPtr, InTransform))
		{
			//UE_LOG(LogTemp, Warning, TEXT("풀링 스폰"));
			ActorToTagMap.Add(NewActor, PoolKey);
			ActivateItems.FindOrAdd(PoolKey).Items.Add(NewActor);
			Internal_ActivateItem(NewActor, InTransform);
			return NewActor;
		}
	}

	return nullptr;
}

AActor* UPoolingSubsystem::SpawnFromPool(const FGameplayTag& SpawnTag, TSubclassOf<AActor> InClass, const FTransform& InTransform)
{
	if (!InClass) return nullptr;

	FName PoolKey = SpawnTag.GetTagName();

	// 풀 세트에 등록되어 있지 않다면 새로 등록
	if (!PoolingSet.PoolSet.Contains(SpawnTag))
	{
		PoolingSet.PoolSet.Add(SpawnTag, InClass);
		ClassToTagMap.Add(InClass, PoolKey);
	}

	return SpawnFromPool(SpawnTag, InTransform);
}

AActor* UPoolingSubsystem::SpawnFromPool(TSubclassOf<AActor> InClass, const FTransform& InTransform)
{
	if (!InClass) return nullptr;

	FName PoolKey = InClass->GetFName();

	// 매핑이 없다면 런타임 등록
	if (!ClassToTagMap.Contains(InClass))
	{
		ClassToTagMap.Add(InClass, PoolKey);
	}

	// 1. 비활성화된 풀에 여유가 있는지 확인
	if (DeactivedItems.Contains(PoolKey) && DeactivedItems[PoolKey].Items.Num() > 0)
	{
		auto It = DeactivedItems[PoolKey].Items.CreateIterator();
		AActor* PooledActor = *It;
		It.RemoveCurrent();

		if (IsValid(PooledActor))
		{
			//UE_LOG(LogTemp, Warning, TEXT("풀링 활성화"));
			ActivateItems.FindOrAdd(PoolKey).Items.Add(PooledActor);
			Internal_ActivateItem(PooledActor, InTransform);
			return PooledActor;
		}
	}

	// 2. 비활성 풀이 비어있다면 클래스 이름으로 바로 스폰
	FActorSpawnParameters SpawnParam;
	SpawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	if (AActor* NewActor = GetWorld()->SpawnActor<AActor>(InClass, InTransform, SpawnParam))
	{
		//UE_LOG(LogTemp, Warning, TEXT("풀링 스폰"));
		ActorToTagMap.Add(NewActor, PoolKey);
		ActivateItems.FindOrAdd(PoolKey).Items.Add(NewActor);
		Internal_ActivateItem(NewActor, InTransform);
		return NewActor;
	}

	return nullptr;
}

bool UPoolingSubsystem::ActivateItem(AActor* InActor)
{
	if (!InActor) return false;

	if (FName* CategoryName = ClassToTagMap.Find(InActor->GetClass()))
	{
		if (FPoolItemsData* PoolItemData = DeactivedItems.Find(*CategoryName))
		{
			if (AActor** FindActor = PoolItemData->Items.Find(InActor))
			{
				ActivateItems.FindOrAdd(*CategoryName).Items.Add(InActor);
				DeactivedItems.FindOrAdd(*CategoryName).Items.Remove(InActor);
				return true;
			}
		}
	}
	return false;
}

bool UPoolingSubsystem::DeactiveItem(AActor* InActor)
{
	if (!IsValid(InActor)) return false;

	if (FName* FoundKey = ActorToTagMap.Find(InActor))
	{
		FName PoolKey = *FoundKey;

		ActivateItems.FindOrAdd(PoolKey).Items.Remove(InActor);
		DeactivedItems.FindOrAdd(PoolKey).Items.Add(InActor);

		Internal_DeactiveItem(InActor);
		return true;
	}

	return false;
}

void UPoolingSubsystem::DestroyItem(AActor* InActor)
{
	if (!IsValid(InActor)) return;

	if (FName* FoundKey = ActorToTagMap.Find(InActor))
	{
		FName PoolKey = *FoundKey;
		ActivateItems.FindOrAdd(PoolKey).Items.Remove(InActor);
		DeactivedItems.FindOrAdd(PoolKey).Items.Remove(InActor);
		ActorToTagMap.Remove(InActor);
	}

	InActor->Destroy();
}

void UPoolingSubsystem::Internal_ActivateItem(AActor* InActor, const FTransform& InTransform)
{
	if (InActor)
	{
		AActor* DefaultActor = InActor->GetClass()->GetDefaultObject<AActor>();
		FVector DefaultScale = DefaultActor->GetRootComponent()->GetRelativeScale3D();

		FTransform ResultTransform = InTransform;
		FVector Scale = FVector(ResultTransform.GetScale3D().X * DefaultScale.X, ResultTransform.GetScale3D().Y * DefaultScale.Y, ResultTransform.GetScale3D().Z * DefaultScale.Z);
		ResultTransform.SetScale3D(Scale);
		InActor->SetActorTransform(ResultTransform);
	}

	if (InActor && InActor->GetClass()->ImplementsInterface(UPoolingInterface::StaticClass()))
	{
		auto Interface = Cast<IPoolingInterface>(InActor);
		Interface->ActivateItem();
	}
	else
	{
		InActor->SetActorHiddenInGame(false);
		InActor->SetActorEnableCollision(true);
		InActor->SetActorTickEnabled(true);
	}
}

void UPoolingSubsystem::Internal_DeactiveItem(AActor* InActor)
{
	if (InActor->GetClass()->ImplementsInterface(UPoolingInterface::StaticClass()))
	{
		auto Interface = Cast<IPoolingInterface>(InActor);
		Interface->DeactiveItem();
	}
	else
	{
		InActor->SetActorHiddenInGame(true);
		InActor->SetActorEnableCollision(false);
		InActor->SetActorTickEnabled(false);
	}
}