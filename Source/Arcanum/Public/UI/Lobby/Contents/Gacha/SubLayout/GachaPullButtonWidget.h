#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GachaPullButtonWidget.generated.h"

class UButton;
class UTextBlock;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPullClicked, int32);

UCLASS()
class ARCANUM_API UGachaPullButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region 델리게이트
public:
    FOnPullClicked OnPullClicked;
private:
    UFUNCTION() void HandleClicked();
#pragma endregion

#pragma region 언리얼 기본생성
protected:
    virtual void NativeConstruct() override;
#pragma endregion

	
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int32 PullCount = 1;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> PullButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> CurrencyText;

    UFUNCTION()
    void SetTextCurrency(FText InText);
};
