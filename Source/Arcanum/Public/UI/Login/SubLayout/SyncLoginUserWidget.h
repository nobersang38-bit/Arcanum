#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Login/DataType/ELoginDataTypes.h"
#include "SyncLoginUserWidget.generated.h"

/** Delegate */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSyncFinished);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSyncFailed, const FString&, ErrorMessage);

UCLASS()
class ARCANUM_API USyncLoginUserWidget : public UUserWidget
{
	GENERATED_BODY()

#pragma region 델리게이트
public:
	/** Sync 성공 */
	UFUNCTION(BlueprintCallable, Category = "Sync")
	void FinishSyncVisual();

	/** Sync 실패 */
	UFUNCTION(BlueprintCallable, Category = "Sync")
	void FailSyncVisual(const FString& ErrorMessage);

	/** Sync 성공 알림 */
	UPROPERTY(BlueprintAssignable, Category = "Sync")
	FOnSyncFinished OnSyncFinished;
	/** Sync 실패 알림 */
	UPROPERTY(BlueprintAssignable, Category = "Sync")
	FOnSyncFailed OnSyncFailed;
#pragma endregion

#pragma region 언리얼 기본 생성
protected:
	virtual void NativeConstruct() override;
#pragma endregion

#pragma region 상태 설정
public:
	/** Sync 단계 설정 (HUD에서 호출) */
	UFUNCTION(BlueprintCallable)
	void SetSyncPhase(ESyncPhase InPhase);

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
};
