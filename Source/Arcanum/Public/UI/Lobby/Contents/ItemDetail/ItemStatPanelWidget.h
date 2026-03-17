#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Lobby/Contents/ItemDetail/ItemDetailHelper.h"
#include "ItemStatPanelWidget.generated.h"

class UVerticalBox;
class UItemStatLineWidget;

/**
 * 추영호
 * 아이템 능력치 표시 위젯
 */

UCLASS()
class ARCANUM_API UItemStatPanelWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/* 능력치 목록 데이터 적용 */
	void SetStatLines(const TArray<FItemStatLineViewData>& InStatLines);

	/* 능력치 목록 초기화 */
	void ClearStatLines();

protected:
	/* 능력치 라인 컨테이너 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UVerticalBox> StatLineBox;

	/* 능력치 1줄 위젯 클래스 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDetail")
	TSubclassOf<UItemStatLineWidget> ItemStatLineWidgetClass;
};