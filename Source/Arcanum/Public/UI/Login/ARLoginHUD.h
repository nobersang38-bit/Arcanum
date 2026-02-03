#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Login/DataType/ELoginDataTypes.h"
#include "ARLoginHUD.generated.h"

class UKeyInputUserWidget;
class ULoginUserWidget;
class USyncLoginUserWidget;

UCLASS()
class ARCANUM_API UARLoginHUD : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region 언리얼 기본 생성
protected:
	virtual void NativeConstruct() override;
#pragma endregion

#pragma region PressAnyKey
public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UKeyInputUserWidget> KeyInputWidget;

private:
	UFUNCTION()
	void OnPressAnyKey();
#pragma endregion

#pragma region 데이터 동기화용 위젯
public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USyncLoginUserWidget> SyncLoginWidget;
private:
	UFUNCTION()
	void OnPreLoginSyncFinished();
#pragma endregion

#pragma region 아디/비번용
public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<ULoginUserWidget> LoginUserWidget;
#pragma endregion
};
