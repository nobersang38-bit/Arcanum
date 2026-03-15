#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Lobby/Contents/ItemDetail/ItemDetailHelper.h"
#include "ItemStatLineWidget.generated.h"

class UTextBlock;

/**
 * 추영호
 * 아이템 능력치 1줄 표시 위젯
 */
UCLASS()
class ARCANUM_API UItemStatLineWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/* 능력치 1줄 데이터 적용 */
	void SetStatLineData(const FItemStatLineViewData& InStatLineData);

	/* 능력치 1줄 초기화 */
	void ClearStatLineData();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> StatNameText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> StatValueText;
};
