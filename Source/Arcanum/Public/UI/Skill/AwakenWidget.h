#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AwakenWidget.generated.h"

/*
 * 추영호
 * - 각성 버튼 / 각성 게이지 전용 위젯
 */

class UButton;
class UImage;
class UMaterialInstanceDynamic;

/* 각성 버튼 클릭 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAwakenClickedRequested);

UCLASS()
class ARCANUM_API UAwakenWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	/* 버튼 바인딩 분리 */
	void BindButtonEvents();

	/* 게이지 MID 초기화 로직 */
	void InitGaugeMaterial();

public:
	/* UI 갱신 함수 */
	void SetGaugePercent(float InPercent);

	/* 각성 상태 UI 갱신 함수 */
	void SetAwakenState(bool bInAwakenActive);

	/* 초기 갱신 */
	void RefreshAwakenVisual(float InGaugePercent);

private:
	/* 각성 버튼 투명화 */
	void UpdateAwakenButtonVisual(float InGaugePercent);

private:
	/* 버튼 클릭 -> HUD로 요청 */
	UFUNCTION()
	void HandleAwakenClicked();

public:
	UPROPERTY(BlueprintAssignable, Category = "Awaken")
	FOnAwakenClickedRequested OnAwakenClickedRequested;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> ButtonAwaken = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> ImageAwakenGauge = nullptr;

private:
	UPROPERTY(Transient)
	TObjectPtr<UMaterialInstanceDynamic> GaugeMID = nullptr;
};
