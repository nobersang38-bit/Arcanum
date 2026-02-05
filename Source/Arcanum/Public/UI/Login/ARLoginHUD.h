#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Login/DataType/ELoginDataTypes.h"
#include "GameplayTags/ArcanumTags.h"
#include "ARLoginHUD.generated.h"

class UButton;

class UKeyInputUserWidget;
class ULoginUserWidget;
class USyncLoginUserWidget;
class UQuitGameUserWidget;
class UAnnouncetUserWidget;

class ALoginCharacter;

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
#pragma endregion

#pragma region 공지사항 용
public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UAnnouncetUserWidget> AnnouncetUserWidget;
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	FString AnnounceString;
private:
	UFUNCTION()
	void HandleAnnouncementClose();
#pragma endregion

#pragma region 아디/비번용
public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<ULoginUserWidget> LoginUserWidget;

	UFUNCTION(BlueprintCallable)
	void OnGuestLoginButtonClicked();
protected:
	/** 게스트 로그인 버튼 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> GuestLoginButton;

	UFUNCTION(BlueprintCallable)
	void HandleLoginOK(const FString& ID, const FString& PW);
	UFUNCTION(BlueprintCallable)
	void HandleLoginCancel();
private:
	void StartPostLogin(const FString& ID, const FString& PW);
	UFUNCTION()
	void OnPostLoginFinished(bool bSuccess, const FString& ErrorMessage);
#pragma endregion

#pragma region 게임 종료용
public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UQuitGameUserWidget> QuitGameWidget;
#pragma endregion

public:
	/** 특정 GameplayTag를 가진 CineCamera를 찾아서 ViewTarget으로 설정 */
	UFUNCTION(BlueprintCallable, Category = "Camera")
	void SetCameraByRole(FGameplayTag CameraRole, bool bImmediately = true);

	/** A카메라에서 B카메라로 이동할때의 이동 시간 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "00_Setting")
	float CameraMoving = 1.f;
	UPROPERTY()
	TObjectPtr<ALoginCharacter> LoginCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "00_Setting")
	FName NextMapName = FName("LobbyMap");
};
