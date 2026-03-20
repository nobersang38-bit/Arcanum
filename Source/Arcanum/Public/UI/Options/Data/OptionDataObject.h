#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "OptionDataObject.generated.h"

/// 260318 추가

UENUM(BlueprintType)
enum class EOptionType : uint8 {
    Scalar,
    Bool,
    Enum
};

UCLASS(Blueprintable, BlueprintType)
class ARCANUM_API UOptionDataObject : public UObject
{
	GENERATED_BODY()
	
public:
    DECLARE_MULTICAST_DELEGATE_OneParam(FOnOptionValueChanged, UOptionDataObject*);
    FOnOptionValueChanged OnOptionValueChanged;

    UPROPERTY(EditAnywhere, BlueprintReadOnly) FName DataID;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) FText DisplayName;
    UPROPERTY(EditAnywhere, BlueprintReadOnly) EOptionType OptionType;

    virtual void UpdateValue(float InValue) {}
	virtual void UpdateValue(bool InValue) {}
	virtual void UpdateValue(int32 InIndex) {}
};