#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AnnouncetUserWidget.generated.h"

class UTextBlock;
class UScrollBox;
class UButton;
class UImage;

/** 브로드캐스트용 버튼 클릭 이벤트 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCloseClicked);

/**
 * Version : 1.0.0.0 2026/02/04
 * 역할 :
 * 1. 로그인 화면 공지사항 표시
 * 2. 여러 줄 텍스트 지원, 스크롤 가능
 * 3. 닫기 버튼으로 숨기기
 */
UCLASS()
class ARCANUM_API UAnnouncetUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/** HUD가 구독할 수 있는 이벤트 */
	UPROPERTY(BlueprintAssignable, Category = "Announcement")
	FOnCloseClicked OnCloseClicked;
	
protected:
	/** 공지 텍스트 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AnnouncementText;

	/** 스크롤 박스 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox;

	/** 닫기 버튼 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CloseButton;

	/** 배경 이미지 */
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UImage> BackgroundImage;

public:
	/** 공지 텍스트 설정 */
	UFUNCTION(BlueprintCallable, Category = "Announcement")
	void SetAnnouncementText(const FText& NewText);

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleCloseButtonClicked();
};
