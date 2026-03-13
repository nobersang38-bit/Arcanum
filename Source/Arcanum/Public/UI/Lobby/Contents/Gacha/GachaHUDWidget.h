#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NativeGameplayTags.h"
#include "UI/Lobby/LobbyHUD.h"
#include "DataInfo/GachaData/DataTable/DTGachaBannerData.h"
#include "GachaHUDWidget.generated.h"

class UImage;
class UButton;
class UVerticalBox;
class UGachaPullButtonWidget;
class UGachaProbabilityWidget;
class UGachaBannerButtonWidget;

UCLASS()
class ARCANUM_API UGachaHUDWidget : public UUserWidget
{
    GENERATED_BODY()

    #pragma region 언리얼 기본생성 / 초기화
public:
    void SetParentLobby(ULobbyHUD* InLobby) { ParentLobby = InLobby; }
protected:
    virtual void NativeConstruct() override;
private:
    UPROPERTY() TObjectPtr<ULobbyHUD> ParentLobby;
    /** 배너 목록 초기화 (데이터 테이블 기반으로 생성) */
    void InitBannerList();
    UPROPERTY() TArray<FDTGachaBannerDataRow> ActiveBannerDataList;
    UFUNCTION() void HandleTimeUpdated(FDateTime CurrentTime);
#pragma endregion

#pragma region 배너 버튼 관련
protected:
    UPROPERTY(meta = (BindWidget)) TObjectPtr<UVerticalBox> BannerVerticalBox;

    /** 버튼 위젯 클래스 (에디터에서 할당) */
    UPROPERTY(EditAnywhere, Category = "Gacha|Settings")
    TSubclassOf<UGachaBannerButtonWidget> BannerButtonClass;

    /** 선택된 배너의 상세 픽업 일러스트 */
    UPROPERTY(meta = (BindWidget)) TObjectPtr<UImage> DetailedBannerImage;
private:
    /** 현재 유효한 배너 버튼 전부 보관 */
    UPROPERTY() TArray<TObjectPtr<UGachaBannerButtonWidget>> BannerButtons;
    /** 현재 선택된 배너 버튼 보관 */
    UPROPERTY() TObjectPtr<UGachaBannerButtonWidget> CurrentSelectedButton;
    /** 버튼 클릭 시 호출될 함수 */
    void OnBannerSelected(FGameplayTag SelectedBannerTag);
    void UpdateDetailedView(const FDTGachaBannerDataRow* InData);
    /** 상세 이미지를 부드럽게 교체하기 위한 텍스처 설정 함수 */
    void UpdateDetailedImage(TSoftObjectPtr<UTexture2D> NewTexture);

    UPROPERTY() FCurrencyCost CurrencyCost;
    #pragma endregion

#pragma region 픽업 선택 버튼 관련
protected:
    UPROPERTY(meta = (BindWidget)) TObjectPtr<UGachaPullButtonWidget> SinglePullButton;
    UPROPERTY(meta = (BindWidget)) TObjectPtr<UGachaPullButtonWidget> MultiPullButton;
    private:
    void RequestGacha(int32 InPullCount);
#pragma endregion

#pragma region 확률 버튼
protected:
    /** 확률 공지 팝업을 띄우는 버튼 */
    UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> ProbabilityInfoButton;
    UPROPERTY(meta = (BindWidget)) TObjectPtr<UGachaProbabilityWidget> ProbabilityWidget;

    private:
    /** 확률 버튼 클릭 핸들러 */
    UFUNCTION() void HandleProbabilityButtonClicked();
#pragma endregion

#pragma region 가챠 맵
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TSoftObjectPtr<UWorld> GachaMap;
#pragma endregion

};