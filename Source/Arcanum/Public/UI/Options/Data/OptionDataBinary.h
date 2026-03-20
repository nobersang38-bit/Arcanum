#pragma once

#include "CoreMinimal.h"
#include "UI/Options/Data/OptionDataObject.h"
#include "OptionDataBinary.generated.h"

/// 260318 추가

UCLASS()
class ARCANUM_API UOptionDataBinary : public UOptionDataObject
{
	GENERATED_BODY()

public:
    UOptionDataBinary() { OptionType = EOptionType::Bool; }
    bool bValue;

    virtual void UpdateValue(bool InValue) override {
        bValue = InValue;
        OnOptionValueChanged.Broadcast(this);
    }
};