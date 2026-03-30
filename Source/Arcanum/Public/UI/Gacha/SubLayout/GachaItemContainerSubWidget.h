#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Core/ARGameInstance.h"
#include "GachaItemContainerSubWidget.generated.h"

class UImage;
class UTextBlock;

USTRUCT(BlueprintType)
struct FGradeTextureSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, UTexture2D*> GrageTexture2D;
};

UCLASS()
class ARCANUM_API UGachaItemContainerSubWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable) void SetItemData(const FGachaItemResult& InData);

protected:
	virtual void NativeConstruct() override;

	/** 아이템 아이콘 */
	UPROPERTY(meta = (BindWidget)) UImage* ItemIcon;

	/** 등급 배경 이미지) */
	UPROPERTY(meta = (BindWidget)) UImage* RarityBG;

	/** 아이템 이름 */
	UPROPERTY(meta = (BindWidget)) UTextBlock* ItemNameText;

	/** 중복 획득 또는 New 같은 텍스트 */
	UPROPERTY(meta = (BindWidget)) UTextBlock* AmountText;

	/** 등급별 테두리나 이펙트 제어용 데이터 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TMap<FGameplayTag, FGradeTextureSet> GradeMatMap;

	/** 등장 애니메이션 */
	UFUNCTION(BlueprintImplementableEvent) void PlayAppearAnimation();

private:
	FGachaItemResult CachedData;
};