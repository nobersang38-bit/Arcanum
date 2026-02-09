#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KeyInputUserWidget.generated.h"

class UTextBlock;
class UButton;

/*
 * Version : 1.0.0.0 2026/02/03
 * 클래스 역할 :
 *  1. Press Any Key 표시
 *  2. 클릭/가상패드 터치 등 최초 1회 감지
 *  3. 입력 감지 시 Delegate Broadcast
 */

// 입력 감지 시 블루프린트나 다른 클래스에서 반응할 수 있도록 하는 다이나믹 멀티캐스트 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnyKeyPressed);

UCLASS()
class ARCANUM_API UKeyInputUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** 최초 입력이 감지되었을 때 호출되는 이벤트*/
	UPROPERTY(BlueprintAssignable, Category = "Input | Events")
	FOnAnyKeyPressed OnAnyKeyPressed;

	/** 필요 시 위젯내 애니메이션 재생*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PlayWidgetAnimation();

	UFUNCTION(BlueprintCallable)
	void PlayWidgetAnimation_Implementation();

	/** 모든 종류의 입력을 공통적으로 처리하고 델리게이트를 호출하는 내부 함수 */
	UFUNCTION(BlueprintCallable)
	void HandleAnyInput();
protected:
	/** 위젯 생성 시 초기화 로직: 포커스 설정 등을 처리 */
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnButtonClicked();

	/** 중복 입력 처리를 방지하기 위한 플래그 (true면 이미 입력이 처리) */
	bool bInputReceived = false;

#pragma region WBP 바인딩
protected:
	/** Press Any Key 텍스트 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PressAnyKeyText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> AnyKeyButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString AnyText = TEXT("Press Any Key");
#pragma endregion
};