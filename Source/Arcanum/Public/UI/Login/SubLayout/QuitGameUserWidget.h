#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuitGameUserWidget.generated.h"

class UTextBlock;
class UButton;

UCLASS()
class ARCANUM_API UQuitGameUserWidget : public UUserWidget
{
	GENERATED_BODY()
		
public:
	/** 외부에서 에러 메시지 띄우기 위해서*/
	void SetQuitMessage(const FString& Reason);

protected:
	/** 버튼 바인딩용 */
	virtual void NativeConstruct() override;
	
protected:
	/** 종료 사유를 표시할 텍스트 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Msg;

	/** 클릭 시 종료 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Btn;

private:
	UFUNCTION()
	void OnQuitButtonClicked();
};
