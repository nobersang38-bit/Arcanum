#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Lobby/Contents/ItemDetail/ItemDetailHelper.h"
#include "ItemTooltipWidget.generated.h"

class UItemTitlePanelWidget;
class UItemStatPanelWidget;
class UTextBlock;

/**
 * 추영호
 * 아이템 툴팁 전체 표시 위젯
 */

UCLASS()
class ARCANUM_API UItemTooltipWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/* 툴팁 데이터 적용 */
	void SetTooltipData(const FItemDisplayViewData& InViewData);

	/* 툴팁 데이터 초기화 */
	void ClearTooltipData();

	/* 아이템 툴팁 데이터 적용 */
	void ApplyDisplayData(const FItemDisplayViewData& InViewData);
protected:
	/* 아이템 이름 패널 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UItemTitlePanelWidget> ItemTitlePanelWidget;

	/* 현재 능력치 패널 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UItemStatPanelWidget> CurrentStatPanelWidget;

	/* 설명 텍스트 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> DescText;

	/* 판매 가격 텍스트 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> SellPriceText;
};