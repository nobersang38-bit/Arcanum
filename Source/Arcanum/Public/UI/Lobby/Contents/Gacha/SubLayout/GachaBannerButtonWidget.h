#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NativeGameplayTags.h"
#include "GachaBannerButtonWidget.generated.h"

class UImage;
class UButton;
class UBorder;
class UTexture2D;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnBannerClicked, FGameplayTag);

UCLASS()
class ARCANUM_API UGachaBannerButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region 델리게이트
public:
    FOnBannerClicked BannerClicked;
private:
    UFUNCTION() void HandleClicked();
#pragma endregion

#pragma region 언리얼 기본생성
protected:
    virtual void NativeConstruct() override;
#pragma endregion

#pragma region 태그 설정
public:
    FGameplayTag BannerTag;
#pragma endregion

#pragma region 버튼 설정
public:
    void SetSelected(bool bSelected);
    void SetBannerTexture(UTexture2D* InTexture);
    void SetBannerTexture(UTexture2D* InNormal, UTexture2D* InHover, UTexture2D* InClick);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button Settings", meta = (ExposeOnSpawn = "true"))
    TSoftObjectPtr<UTexture2D> NormalTexture;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button Settings", meta = (ExposeOnSpawn = "true"))
    TSoftObjectPtr<UTexture2D> HoverTexture;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button Settings", meta = (ExposeOnSpawn = "true"))
    TSoftObjectPtr<UTexture2D> ClickTexture;

protected:
    UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> BannerButton;
    //UPROPERTY(meta = (BindWidget)) TObjectPtr<UBorder> SelectionBorder;
#pragma endregion
};
