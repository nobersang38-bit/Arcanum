#pragma once

#include "CoreMinimal.h"
#include "UI/Options/Data/OptionDataObject.h"
#include "OptionDataScalar.generated.h"

/// 260318 추가

UCLASS()
class ARCANUM_API UOptionDataScalar : public UOptionDataObject
{
	GENERATED_BODY()
	
public:
    UOptionDataScalar() { OptionType = EOptionType::Scalar; }
    float Value;
    float MinValue = 0.f;
    float MaxValue = 1.f;

    virtual void UpdateValue(float InValue) override {
        Value = FMath::Clamp(InValue, MinValue, MaxValue);
        OnOptionValueChanged.Broadcast(this);
    }
};
