#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GachaFinalResultWidget.generated.h"

class UGachaItemContainerSubWidget;

class UWrapBox;
class UButton;

struct FGachaItemResult;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReturnLobbyClicked);

UCLASS()
class ARCANUM_API UGachaFinalResultWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    virtual void NativeConstruct() override;

    // --- 핵심 위젯 바인딩 ---

    /** 컨테이너 */
    UPROPERTY(meta = (BindWidget)) class UWrapBox* IconContainer;
    UPROPERTY(meta = (BindWidget)) class UScrollBox* ResultScrollBox;
    UPROPERTY(meta = (BindWidget)) class USizeBox* ContainerSizeBox;

    UPROPERTY(meta = (BindWidget)) class UButton* ReturnButton;

    // --- 이벤트 및 노출 함수 ---
    UPROPERTY(BlueprintAssignable) FOnReturnLobbyClicked OnReturnLobbyClicked;

    UFUNCTION(BlueprintCallable) void SetGachaResults(const TArray<FGachaItemResult>& InResults);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly) TSubclassOf<UGachaItemContainerSubWidget> ResultItemWidgetClass;
    UPROPERTY() UGachaItemContainerSubWidget* ResultItemWidget;

private:
    UFUNCTION() void HandleReturnClicked();
    void OptimizeLayout(int32 ItemCount);

    // 타이머 핸들
    FTimerHandle DisplayTimerHandle;
    int32 CurrentDisplayIndex;

    // 임시 저장소
    TArray<FGachaItemResult> PendingResults;

    // 실제 하나씩 생성하는 함수
    void SpawnNextItem();

    // 연출 스킵용
    UFUNCTION(BlueprintCallable) void SkipAnimation();
};