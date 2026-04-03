#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GachaHighgradeWidget.generated.h"

UENUM(BlueprintType)
enum class EGachaHighgradeState : uint8
{
	IntroDialogue,
	SilhouetteCamera,
	RevealZoomOut,
	FinalReveal,
	End
};

class UImage;
class UTextBlock;
class UWidgetAnimation;

struct FGachaItemResult;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSilhouetteStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkipRequested);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGachaFinished);
UCLASS()
class ARCANUM_API UGachaHighgradeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable) FOnSilhouetteStart OnSilhouetteStart;
	UPROPERTY(BlueprintAssignable) FOnSkipRequested OnSkipRequested;
	UPROPERTY(BlueprintAssignable) FOnGachaFinished OnGachaFinished;
	
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UFUNCTION(BlueprintCallable) void SkipToFinal();
	UFUNCTION(BlueprintCallable) void InitializeGacha(const FGachaItemResult& ItemData, const FText& InText);
	UFUNCTION() void OnSilhouetteCameraMovingFinished();
protected:
	UPROPERTY(BlueprintReadOnly) EGachaHighgradeState CurrentState;

	/** 애니메이션들 */
	UPROPERTY(meta = (BindWidgetAnim), Transient) UWidgetAnimation* DialogueAnim;
	
	UPROPERTY(BlueprintReadOnly) FText GachaDialogueText;
	
	UPROPERTY(meta = (BindWidgetAnim), Transient) UWidgetAnimation* SilhouetteAnim;
	UPROPERTY(meta = (BindWidgetAnim), Transient) UWidgetAnimation* RevealAnim;

	/** UI 요소 */
	UPROPERTY(meta = (BindWidget)) UTextBlock* DialogueText;
	UPROPERTY(meta = (BindWidget)) UImage* SilhouetteImage;
	UPROPERTY(meta = (BindWidget)) UImage* RevealImage;
	UPROPERTY(meta = (BindWidget)) UImage* BackGround;

	/** 애니메이션 종료 콜백 */
	UFUNCTION() void OnDialogueFinished();
	UFUNCTION() void OnSilhouetteFinished();
	UFUNCTION() void OnRevealFinished();

	/** 상태 진행 */
	void PlayNextState();

	bool Isclicked = false; // click시 true
};