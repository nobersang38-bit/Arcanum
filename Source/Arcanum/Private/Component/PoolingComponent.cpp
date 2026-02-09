// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/PoolingComponent.h"
#include "Data/Pooling/PoolSpawnManager.h"
#include "Interface/Poolable.h"

// Sets default values for this component's properties
UPoolingComponent::UPoolingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PoolSpawnManager = CreateDefaultSubobject<UPoolSpawnManager>(TEXT("SpawnManager"));
	// ...
}


// Called when the game starts
void UPoolingComponent::BeginPlay()
{
	Super::BeginPlay();
	PoolSpawnManager->PoolingComponent = this;
	IsSettingInvalid();
	if (!IsError) InitialItemsAdd();
}


// Called every frame
void UPoolingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPoolingComponent::Setting(int32 InDefaultPoolCount, int32 InMaxPoolCount)
{
	if (InDefaultPoolCount != -1)
	{
		DefaultPoolSize = InDefaultPoolCount;
	}

	if (InMaxPoolCount != -1)
	{
		MaxPoolSize = InMaxPoolCount;
	}

	IsSettingInvalid();
}

void UPoolingComponent::InitialItemsAdd()
{
	if (IsSpawnClassInvalid() || IsError) return;

	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
		{
			PoolClear();

			FTransform TempTransform = FTransform(FRotator::ZeroRotator, FVector(GetComponentLocation()), FVector::OneVector);

			for (int i = 0; i < DefaultPoolSize; i++)
			{
				UObject* TempItem = PoolSpawnManager->Spawn(TempTransform);

				int32 IDX = PooledItems.Num();
				PooledItems.AddTail(TempItem);
				DeactivedItems.Add(TempItem);

				
				if (TempItem->Implements<UPoolable>())
				{
					IPoolable::Execute_SetPoolComponent(TempItem, this);
					IPoolable::Execute_DeactivateItem(TempItem);
				}
				else
				{
					Log_ItemMissingInterface(TempItem->GetClass()->GetName());
				}
			}
		});
}

void UPoolingComponent::PooledItemMoveFirstToLast(UObject* InMoveItem)
{
	if (IsSpawnClassInvalid() || IsError) return;

	if (PooledItems.Contains(InMoveItem))
	{
		PooledItems.AddTail(InMoveItem);
		PooledItems.RemoveNode(InMoveItem);
	}
}

void UPoolingComponent::SetLocation(UObject* Object, EReferenceLocation ReferenceLocation, const FVector& InLocation)
{
	/*if (!PoolSpawnManager) return;

	if (AActor* Actor = Cast<AActor>(Object))
	{
		switch (ReferenceLocation)
		{
		case EReferenceLocation::Absolute:
			Actor->SetActorLocation(InLocation);
			break;
		case EReferenceLocation::Relative:
			FVector ResultLocation = GetOwner()->GetTransform().TransformPosition(InLocation);
			UE_LOG(LogTemp, Log, TEXT("위치 : %f, %f, %f"), ResultLocation.X, ResultLocation.Y, ResultLocation.Z);
			Actor->SetActorLocation(ResultLocation);
			break;
		default:
			break;
		}
	}
	else return;

	if (USceneComponent* SceneComponent = Cast<USceneComponent>(Object))
	{
		switch (ReferenceLocation)
		{
		case EReferenceLocation::Absolute:
			SceneComponent->SetWorldLocation(InLocation);
			break;
		case EReferenceLocation::Relative:
			FVector ResultLocation = GetOwner()->GetTransform().TransformPosition(InLocation);
			UE_LOG(LogTemp, Log, TEXT("위치 : %f, %f, %f"), ResultLocation.X, ResultLocation.Y, ResultLocation.Z);
			SceneComponent->SetWorldLocation(ResultLocation);
			break;
		default:
			break;
		}
	}
	else return;*/
}

bool UPoolingComponent::IsSpawnClassInvalid() const
{
	if (!PoolSpawnManager->GetSpawnClass())
	{
		UE_LOG(LogTemp, Error, TEXT("UPoolingComponent의 SpawnClass가 Null입니다"));
		return true;
	}
	else
	{
		return false;
	}
}

void UPoolingComponent::Log_ItemMissingInterface(FString InErrorClassName) const
{
	UE_LOG(LogTemp, Error, TEXT("UPoolingComponent : %s에 IPoolable가 없습니다"), *InErrorClassName);
}

bool UPoolingComponent::IsSettingInvalid()
{
	bool bIsError = false;
	if (!GetOwner())
	{
		UE_LOG(LogTemp, Error, TEXT("UPoolingComponent의 Owner가 없습니다"));
		bIsError = true;
	}

	if (!PoolSpawnManager)
	{
		UE_LOG(LogTemp, Error, TEXT("%s : PoolSpawnManager가 없습니다	%s:%d"), *GetOwner()->GetName(), TEXT(__FILE__), __LINE__);
		bIsError = true;
	}

	if (DefaultPoolSize < 0)
	{
		UE_LOG(LogTemp, Error, TEXT("%s : DefaultPoolSize가 1보다 작습니다	%s:%d"), *GetOwner()->GetName(), TEXT(__FILE__), __LINE__);
		bIsError = true;
	}

	if (DefaultPoolSize > MaxPoolSize)
	{
		UE_LOG(LogTemp, Error, TEXT("%s : MaxPoolSize가 DefaultPoolSize보다 작습니다	%s:%d"), *GetOwner()->GetName(), TEXT(__FILE__), __LINE__);
		bIsError = true;
	}

	if (!PoolSpawnManager->GetSpawnClass())
	{
		UE_LOG(LogTemp, Error, TEXT("%s : UPoolingComponent의 SpawnClass가 Null입니다	%s:%d"), *GetOwner()->GetName(), TEXT(__FILE__), __LINE__);
		bIsError = true;
	}

	if (PoolSpawnManager->GetSpawnType() == ESpawnType::None)
	{
		UE_LOG(LogTemp, Error, TEXT("%s : SpawnType이 None입니다	%s:%d"), *GetOwner()->GetName(), TEXT(__FILE__), __LINE__);
		bIsError = true;
	}

	if (!PoolSpawnManager->GetSpawnClass() || !(PoolSpawnManager->GetSpawnClass()->ImplementsInterface(UPoolable::StaticClass())))
	{
		UE_LOG(LogTemp, Error, TEXT("%s : SpawnClass에 IPoolable가 없습니다	%s:%d"), *GetOwner()->GetName(), TEXT(__FILE__), __LINE__);
		bIsError = true;
	}
	this->IsError = bIsError;
	return bIsError;
}

UObject* UPoolingComponent::ActivatePooledItem(EReferenceLocation ReferenceLocation, const FVector& InLocation)
{
	//비활성화 아이템 중 첫번째 활성화 함수 호출 -> 비활성화 아이템Set에서 지우기
	if (IsSpawnClassInvalid() || IsError) return nullptr;

	if (!DeactivedItems.IsEmpty()) //비활성화 중인게 있다면 이걸 활성화가능
	{
		auto DeactivedItem = DeactivedItems.begin();
		if ((*DeactivedItem)->Implements<UPoolable>())
		{
			IPoolable::Execute_ActivateItem((*DeactivedItem));
			SetLocation((*DeactivedItem), ReferenceLocation, InLocation);

			if (DeactivedItems.Contains(*DeactivedItem))//액티브에서 지우고 뒤에 추가후 Deactiveds에서 삭제 
			{
				if (PooledItems.Contains(*DeactivedItem))
				{
					PooledItemMoveFirstToLast(*DeactivedItem);

					DeactivedItems.Remove(*DeactivedItem);//비활성화 아이템들에서 제거
					//UE_LOG(LogTemp, Log, TEXT("비활성인 것 중 하나 선택해서 활성화 완료 디버그"));
					return *DeactivedItem;
				}
			}
		}
		else
		{
			Log_ItemMissingInterface((*DeactivedItem)->GetClass()->GetName());
		}
	}
	else if (PooledItems.Num() < MaxPoolSize)//새로 생성 및 추가
	{
		FTransform TempTransform = FTransform(FRotator::ZeroRotator, FVector(GetComponentLocation()), FVector::OneVector);
		UObject* TempItem = PoolSpawnManager->Spawn(TempTransform);

		PooledItems.AddTail(TempItem);

		if (TempItem->Implements<UPoolable>())
		{
			IPoolable::Execute_SetPoolComponent(TempItem, this);
			IPoolable::Execute_DeactivateItem(TempItem);
			IPoolable::Execute_ActivateItem(TempItem);
			SetLocation(TempItem, ReferenceLocation, InLocation);

			return TempItem;
			//UE_LOG(LogTemp, Log, TEXT("새로 생성 완료 디버그"));
		}
		else
		{
			Log_ItemMissingInterface(TempItem->GetClass()->GetName());
		}
	}
	else if (ForceActivate)
	{
		PooledItemMoveFirstToLast(PooledItems.GetHead()->GetValue());
		UObject* TempItem = PooledItems.GetTail()->GetValue();

		if (TempItem->Implements<UPoolable>())
		{
			IPoolable::Execute_DeactivateItem(TempItem);
			IPoolable::Execute_ActivateItem(TempItem);
			SetLocation(TempItem, ReferenceLocation, InLocation);

			UE_LOG(LogTemp, Warning, TEXT("UPoolingComponent : 제일 오래 활성화된 아이템을 강제 재활성화 했습니다"));

			return TempItem;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UPoolingComponent : 더 생성할 수 없습니다"));
	}

	return nullptr;
}

void UPoolingComponent::DeactivatePooledItem(UObject* InDeactivedItem)
{
	if (!InDeactivedItem) return;
	if (!PooledItems.Contains(InDeactivedItem)) return;

	if (InDeactivedItem->Implements<UPoolable>())
	{
		IPoolable::Execute_DeactivateItem(InDeactivedItem);
		UE_LOG(LogTemp, Log, TEXT("DeactivatePooledItem인터페이스 실행됨"));
	}

	DeactivedItems.Add(InDeactivedItem);
}

void UPoolingComponent::PoolClear()
{
	for (auto& Item : PooledItems)
	{
		if (Item->Implements<UPoolable>())
		{
			IPoolable::Execute_DestroyItem(Item);
		}
		else
		{
			Log_ItemMissingInterface(Item->GetName());
		}
	}
	PooledItems.Empty();
}

