#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnhancementHUDWidget.generated.h"

class ULobbyHUD;
class UInventoryHUDWidget;

/**
 * 추영호
 * 강화 UI
 */

UCLASS()
class ARCANUM_API UEnhancementHUDWidget : public UUserWidget
{
	GENERATED_BODY()

#pragma region 부모 로비 참조
public:
	/* 로비가 생성/바인딩 후 주입 */
	void SetParentLobby(ULobbyHUD* InLobby) { ParentLobby = InLobby; }

private:
	UPROPERTY()
	TObjectPtr<ULobbyHUD> ParentLobby;
#pragma endregion

protected:
	virtual void NativeConstruct() override;

private:
	/* 장비만으로 구성해서 인벤토리 */
	void RefreshEquipmentOnlyInventory();

protected:
	/* 기존 인벤 위젯 재사용 (강화창에 배치) */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidet))
	TObjectPtr<UInventoryHUDWidget> InventoryHUDWidget;
};
