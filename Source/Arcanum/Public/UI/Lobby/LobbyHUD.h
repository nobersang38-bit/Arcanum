#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/DataType/EDialogResult.h"
#include "UI/Lobby/Contents/Character/CharacterHUDWidget.h"
#include "DataInfo/PlayerData/FPlayerData.h"
#include "DataInfo/InventoryData/Data/InventoryViewSlot.h"
#include "LobbyHUD.generated.h"

/*
 * 역할 : 김유진
 * 1. 메뉴 버튼 (전투, 캐릭터, 강화, 상점, 가챠)
 * 2. 설정, 종료 버튼 관리
 */

class UDataTable;
class UCommonBtnWidget;
class UCommonDialog;
class UHorizontalBox;
class UBackgroundBlur;
class UWidgetSwitcher;
class UCurrencyWidget;
class UShopHUDWidget;
class UARGameInstance;
class UInventoryHUDWidget;
class UEnhancementHUDWidget;
struct FDTEquipmentInfoRow;
struct FDTPotionInfoRow;
struct FDTItemCatalogRow;


UCLASS()
class ARCANUM_API ULobbyHUD : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region 언리얼 기본 생성
protected:
	virtual void NativeConstruct() override;
	/** 플레이어 데이터 저장 성공 시 갱신용 함수*/
	UFUNCTION() void HandleSaveCompleted(bool bSuccess);
#pragma endregion

#pragma region 데이터 캐시
public:
	/* 로비 공통 UI 갱신 (재화/인벤/상점 등) */
	void RefreshAllLobbyUI();

	/* 플레이어 데이터 캐시 Getter */
	const FPlayerData& GetCachedPlayerData() const { return CachedPlayerData; }

public:
	FPlayerData CachedPlayerData;
#pragma endregion

#pragma region 바인딩 메뉴 버튼
	// 상단 메뉴 : 전투, 캐릭터, 강화, 상점, 가챠
	// 설정, 종료

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> BattleMenuBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> CharacterMenuBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> EnhancementMenuBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> ShopMenuBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> GachaMenuBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> SettingBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonBtnWidget> QuitBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UHorizontalBox> MenuHorizontalBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UHorizontalBox> SettingUHorizontalBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UBackgroundBlur> BackgroundBlur;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UDataTable> CharacterDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UDataTable> CharacterBattleStatsDataTable;
	
private:
	UFUNCTION()
	void ClickBattleMenuBtn();

	UFUNCTION()
	void ClickCharacterMenuBtn();

	UFUNCTION()
	void ClickEnhancementMenuBtn();

	UFUNCTION()
	void ClickShopMenuBtn();

	UFUNCTION()
	void ClickGachaMenuBtn();

	UFUNCTION()
	void ClickSettingBtn();

	UFUNCTION()
	void ClickQuitBtn();

#pragma endregion

#pragma region 재화
protected:
	/* 재화 UI 갱신 */
	void RefreshLobbyCurrencyUI();

protected:
	/* 재화 위젯 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCurrencyWidget> CurrencyWidget;
#pragma endregion

#pragma region 전투

#pragma endregion

#pragma region 캐릭터
//protected:
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
//	TSubclassOf<UCharacterHUDWidget> CharacterWidgetClass;
//
//private:
//	UCharacterHUDWidget* CharacterWidget = nullptr;
#pragma endregion

#pragma region 강화
protected:
	/* 강화 위젯 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UEnhancementHUDWidget> EnhancementHUDWidget;

#pragma endregion

#pragma region 인벤토리
public:
	/* Getter: ShopHUD에서 InventoryHUD 접근 */
	UInventoryHUDWidget* GetInventoryHUDWidget() const { return InventoryHUDWidget; }

	/* Getter: 인벤 총 생성 슬롯 */
	int32 GetInventoryCapacity() const { return FMath::Max(0, CachedPlayerData.InventoryCapacity); }

protected:
	/* 인벤토리 위젯 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UInventoryHUDWidget> InventoryHUDWidget;
#pragma endregion

#pragma region 상점
protected:
	/* 상점 위젯 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UShopHUDWidget> ShopHUDWidget;

#pragma endregion

#pragma endregion

#pragma region 가챠

#pragma endregion

#pragma region 설정

#pragma endregion

#pragma region 종료
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonDialog> ExitCommonDialog;
private:
	UFUNCTION()
	void OnExitCommonDialog(EDialogResult res);
#pragma endregion

};
