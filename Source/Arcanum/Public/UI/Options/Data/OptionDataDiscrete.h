#pragma once

#include "CoreMinimal.h"
#include "UI/Options/Data/OptionDataObject.h"
#include "OptionDataDiscrete.generated.h"

/// 260318 추가

UCLASS()
class ARCANUM_API UOptionDataDiscrete : public UOptionDataObject
{
	GENERATED_BODY()
	
public:
    UOptionDataDiscrete() { OptionType = EOptionType::Enum; }
    int32 Index;
    TArray<FText> Options;

    virtual void UpdateValue(int32 InIndex) override {
        if (Options.IsValidIndex(InIndex)) {
            Index = InIndex;
            OnOptionValueChanged.Broadcast(this);
        }
    }
};
