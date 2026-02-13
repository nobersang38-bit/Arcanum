#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/DataType/ELoginDataTypes.h"
#include "UI/DataType/EDialogResult.h"
#include "GameplayTags/ArcanumTags.h"
#include "ARLoginHUD.generated.h"

class UKeyInputUserWidget;
class ULoginUserWidget;
class USyncLoginUserWidget;
class UQuitGameUserWidget;
class UAnnouncetUserWidget;
class ULoginPanelWidget;
class UCommonBtnWidget;
class UCommonDialog;

class UVerticalBox;
class ALoginCharacter;

/*
 * 역할 : 김유진
 * 1. 타이틀 - 로그인 화면
 * 2. 공지사항, 설정, 플레이, 종료 버튼 관리
 */

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
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SyncText;
private:
	/** 서버 동기화용 */
	UFUNCTION()
	void OnPreLoginSyncFinished(bool bIsSuccess, const FString& ErrorMessage);
	/** ID 비번 확인 후 플레이어 데이터 가져올때 */
	UFUNCTION()
	void OnPostLoginSyncFinished(bool bIsSuccess, const FString& ErrorMessage);
	void VisibleSyncWidget(bool bIsSuccess, const FString& Message = TEXT(""));
#pragma endregion

#pragma region 공지사항
public:
	UFUNCTION(BlueprintCallable)
	void ClickAnnounceBtn();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> AnnounceBtn;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UAnnouncetUserWidget> AnnouncetUserWidget;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	FString AnnounceString;
private:
	/** 버튼 클릭으로 공지사항 열때 WBP에서 호출*/
	UFUNCTION()
	void HandleAnnouncementOpen(bool bIsSuccess);
	UFUNCTION()
	void HandleAnnouncementClose();
#pragma endregion

#pragma region 설정
public:
	/// Todo : 추후 세팅 만들면 추가 예정.
	UFUNCTION(BlueprintCallable)
	void ClickSettingBtn();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> SettingBtn;
#pragma endregion

#pragma region 플레이
public:
	UFUNCTION(BlueprintCallable)
	void ClickPlayBtn();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> PlayBtn;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<ULoginPanelWidget> LoginPanelWidget;
private:
	UFUNCTION()
	void HandlePlayBtn(bool IsState);
#pragma endregion

#pragma region 종료
public:
	UFUNCTION(BlueprintCallable)
	void ClickExitBtn();
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UQuitGameUserWidget> QuitGameWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> ExitBtn;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonDialog> ExitCommonDialog;
private:
	UFUNCTION()
	void OnExitCommonDialog(EDialogResult res);
#pragma endregion

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UVerticalBox> VerticalBox;

	UPROPERTY()
	TObjectPtr<ALoginCharacter> LoginCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "00_Setting")
	FName NextMapName = FName("LobbyMap");
};
