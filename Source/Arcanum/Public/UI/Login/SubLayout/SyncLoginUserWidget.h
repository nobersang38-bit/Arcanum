#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/DataType/ELoginDataTypes.h"
#include "SyncLoginUserWidget.generated.h"

class UTextBlock;
class UImage;

/** Delegate */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSyncFinished, bool, bIsSuccess, const FString&, ErrorMessage);


UCLASS()
class ARCANUM_API USyncLoginUserWidget : public UUserWidget
{
	GENERATED_BODY()

#pragma region 델리게이트
public:
	UFUNCTION(BlueprintCallable, Category = "Sync")
	void FinishSyncVisual(bool bIsSuccess, const FString& ErrorMessage = TEXT(""));

	/** Sync 성공 알림 */
	UPROPERTY(BlueprintAssignable, Category = "Sync")
	FOnSyncFinished OnSyncFinished;
#pragma endregion

#pragma region 언리얼 기본 생성
protected:
	virtual void NativeConstruct() override;
#pragma endregion

#pragma region 상태 설정
public:
	/** Sync 단계 설정 (HUD에서 호출) */
	UFUNCTION(BlueprintCallable)
	void SetSyncPhase(ESyncPhase InPhase, FText InMsg);

private:
	UFUNCTION()
	void StartPreLogin();
	UFUNCTION()
	void StartPostLogin();
	UFUNCTION()
	void PlayWidgetAnimation();
#pragma endregion

protected:
	/** 현재 Sync 단계 */
	UPROPERTY(BlueprintReadOnly)
	ESyncPhase CurrentPhase = ESyncPhase::PreLogin;	


	/** 현재 상태를 표시할 텍스트 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Msg;

	/** 삥글삥글 이미지 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Img;
};
