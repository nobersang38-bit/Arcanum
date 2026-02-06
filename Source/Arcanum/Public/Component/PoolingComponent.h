// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PoolingComponent.generated.h"

// 김도현
// 액티브될때 기준 좌표
UENUM(BlueprintType)
enum class EReferenceLocation : uint8
{
	Absolute 	UMETA(DisplayName = "Absolute"),
	Relative	UMETA(DisplayName = "Relative"),
};

// 김도현
// 풀링 컴포넌트
// Todo : 잘 작동하나 확인해야함 특히 SetLocation함수
class UPoolSpawnManager;
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ARCANUM_API UPoolingComponent : public USceneComponent
{
	GENERATED_BODY()
	//여기에서 해당 클래스에 간섭해야하는 경우도 있으니 IPoolable넣어야 함, 없으면 여기에서 Deactive를 불가능
public:
	// Sets default values for this component's properties
	UPoolingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	/// <summary>
	/// 초기세팅
	/// 디테일창에도 존재
	/// </summary>
	/// <param name="InDefaultPoolSize">처음에 초기에 풀링 되어야 하는 값을 넣어야함</param>
	/// <param name="InMaxPoolSize">풀링의 최대값을 설정, 이 값은 절대 넘지 않음</param>
	void Setting(int32 InDefaultPoolSize = -1, int32 InMaxPoolSize = -1);

public:

	/// <summary>
	/// 생성 또는 활성화
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "Default")
	UObject* ActivatePooledItem(EReferenceLocation ReferenceLocation = EReferenceLocation::Relative, const FVector& InLocation = FVector::ZeroVector);

	/// <summary>
	/// 비활성화
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "Default")
	void DeactivatePooledItem(UObject* InDeactivedItem);

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Default")
	void PoolClear();

private:
	//풀링된 오브젝트들 (실제로 월드에 존재함)
	TDoubleLinkedList<UObject*> PooledItems;

	//초기 pool은 몇개인가
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DefaultSetting", meta = (AllowPrivateAccess = "true", ToolTip = "초기 pool은 몇개일까요"))
	int32 DefaultPoolSize = 4;

	//최대 Pool은 몇개인가
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DefaultSetting", meta = (AllowPrivateAccess = "true", ToolTip = "최대 Pool은 몇개일까요"))
	int32 MaxPoolSize = 10;

	//Item 최대개수 도달시 마지막 인덱스 강제 활성화 여부
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DefaultSetting", meta = (AllowPrivateAccess = "true", ToolTip = "Item 최대개수 도달시 마지막 인덱스 강제 활성화"))
	bool ForceActivate = true;

	// 스폰 타입 지정
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Instanced, Category = "Instance", meta = (AllowPrivateAccess = "true", ToolTip = "스폰 타입을 지정하세요"))
	TObjectPtr<UPoolSpawnManager> PoolSpawnManager;


private://내부

	//다음 틱에 풀 생성
	void InitialItemsAdd();

	//PooledItems의 선택한 키를 제일 뒤로 옮김, 이때 키값도 요소들중 제일 큰 키로 바뀜
	void PooledItemMoveFirstToLast(UObject* InMoveItem);

	// 위치변경
	void SetLocation(UObject* Object, EReferenceLocation ReferenceLocation = EReferenceLocation::Relative, const FVector& InLocation = FVector::ZeroVector);

	//이건 이제 비활성화된 아이템들의 모음
	TSet<UObject*> DeactivedItems;

	//디버그
	UFUNCTION(CallInEditor, Category = "Debug")
	void TestField()
	{
		UE_LOG(LogTemp, Log, TEXT("\n\n PooledItems"));
		for (auto& i : PooledItems)
		{
			UE_LOG(LogTemp, Log, TEXT("Name : %s	Adress : %p"), *i->GetName(), i);
		}

		UE_LOG(LogTemp, Log, TEXT("\n\n DeactivedItems"));
		for (auto& i : DeactivedItems)
		{
			UE_LOG(LogTemp, Log, TEXT("Value : %s	Adress : %p"), *i->GetName(), i);
		}
	}

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Debug")
	bool IsError = false;

private://Error
	bool IsSpawnClassInvalid() const;
	void Log_ItemMissingInterface(FString InErrorClassName) const;

	//무거울 수 있으니 한번만
	bool IsSettingInvalid();
};
