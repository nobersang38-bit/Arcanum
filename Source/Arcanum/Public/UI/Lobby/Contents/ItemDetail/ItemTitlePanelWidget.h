#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemTitlePanelWidget.generated.h"

class UTextBlock;

/**
 * 추영호
 * 아이템 이름 / 강화 수치 표시 위젯
 */

UCLASS()
class ARCANUM_API UItemTitlePanelWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/* 아이템 이름 */
	void SetItemNameText(const FText& InItemNameText);

	/* 강화 수치 */
	void SetUpgradeLevelText(const FText& InUpgradeLevelText);

	/* 타이틀 데이터 초기화 */
	void ClearTitleData();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemNameText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> UpgradeLevelText;

};