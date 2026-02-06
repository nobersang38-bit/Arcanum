// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Poolable.generated.h"

// 김도현
// 풀링용 인터페이스
UINTERFACE(MinimalAPI)
class UPoolable : public UInterface
{
	GENERATED_BODY()
};

// 김도현
// 풀링용 인터페이스
class ARCANUM_API IPoolable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// 꼭 이걸로 컴포넌트를 할당해야합니다
	UFUNCTION(BlueprintNativeEvent)
	void SetPoolComponent(class UPoolingComponent* InComponent);

	// 자식에서 호출하면 안됩니다
	UFUNCTION(BlueprintNativeEvent)
	void DestroyItem();

	// 자식에서 호출하면 안됩니다
	UFUNCTION(BlueprintNativeEvent)
	void ActivateItem();

	// 자식에서 호출하면 안됩니다
	UFUNCTION(BlueprintNativeEvent)
	void DeactivateItem();
};
