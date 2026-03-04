#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataInfo/GachaData/Data/FGachaBannerData.h"
#include "GachaProbabilityWidget.generated.h"

class UButton;
class UVerticalBox;
class UGachaProbabilityRowWidget;

UCLASS()
class ARCANUM_API UGachaProbabilityWidget : public UUserWidget
{
	GENERATED_BODY()
	
#pragma region 언리얼 기본생성
protected:
    virtual void NativeConstruct() override;
#pragma endregion


public:
    ///** 배너 정보를 받아서 UI를 갱신하는 핵심 함수 */
    //UFUNCTION(BlueprintCallable, Category = "Gacha")
    //void UpdateProbabilityList(const FGachaBannerData& BannerData);

    void HandleClose();

protected:
    /** 등급별 줄(Row) 위젯을 담을 컨테이너 */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UVerticalBox> ProbabilityListContainer;

    /** 한 줄을 구성할 위젯 클래스 */
    UPROPERTY(EditAnywhere, Category = "Gacha|Settings")
    TSubclassOf<UGachaProbabilityRowWidget> ProbabilityRowClass;

    /** 닫기 버튼 (BindWidget) */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> CloseButton;
};
