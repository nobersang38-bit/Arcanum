#include "UI/Lobby/Contents/Gacha/SubLayout/GachaProbabilityWidget.h"

#include "Components/Button.h"
#include "Components/VerticalBox.h"

void UGachaProbabilityWidget::NativeConstruct()
{
    if (CloseButton) {
        CloseButton->OnClicked.AddDynamic(this, &UGachaProbabilityWidget::HandleClose);
    }
}

//void UGachaProbabilityWidget::UpdateProbabilityList(const TArray<FGachaRarityProbability>& Probabilities)
//{
//    if (!ProbabilityListContainer || !ProbabilityRowClass) return;
//
//    ProbabilityListContainer->ClearChildren();
//
//    // 1. 등급별 확률표 순회 (SSR, SR, R...)
//    for (const FGachaRarityProbability& RarityInfo : BannerData.RarityProbabilities)
//    {
//        // --- [등급 헤더 추가] ---
//        UGachaProbabilityRowWidget* HeaderRow = CreateWidget<UGachaProbabilityRowWidget>(this, ProbabilityRowClass);
//        if (HeaderRow)
//        {
//            // 등급명(Tag를 텍스트로)과 총 확률 전달
//            HeaderRow->SetAsHeader(FText::FromName(RarityInfo.GradeTag.GetTagName()), RarityInfo.Probability, FLinearColor(1, 0.8, 0, 1));
//            ProbabilityListContainer->AddChild(HeaderRow);
//        }
//
//        // 2. 해당 등급의 세부 풀 정보 찾기
//        const FGachaGradePool* FoundPool = BannerData.GradePools.FindByPredicate([&](const FGachaGradePool& Pool) {
//            return Pool.GradeTag == RarityInfo.GradeTag;
//            });
//
//        if (FoundPool)
//        {
//            // --- [캐릭터 리스트 추가 로직] ---
//            // 여기서 FoundPool->PickupCharacters(픽업)와 
//            // FoundPool->CommonPoolTable(상시)을 뒤져서 캐릭터 로우 생성
//
//            // 예시: 픽업 캐릭터 먼저 추가
//            for (const FGameplayTag& PickupTag : FoundPool->PickupCharacters)
//            {
//                UGachaProbabilityRowWidget* CharRow = CreateWidget<UGachaProbabilityRowWidget>(this, ProbabilityRowClass);
//                if (CharRow)
//                {
//                    // 니케식 계산: 등급확률 * 픽업비율 / 픽업캐릭터수
//                    float FinalProb = (RarityInfo.Probability * FoundPool->PickupRatio) / FoundPool->PickupCharacters.Num();
//
//                    // 이름과 아이콘은 나중에 마스터 테이블에서 꺼내와야 함 (우선 태그로 표시)
//                    CharRow->SetAsCharacter(nullptr, FText::FromName(PickupTag.GetTagName()), FinalProb, true);
//                    ProbabilityListContainer->AddChild(CharRow);
//                }
//            }
//
//            // TODO: 상시 캐릭터들도 같은 방식으로 추가...
//        }
//    }
//}

void UGachaProbabilityWidget::HandleClose()
{
    SetVisibility(ESlateVisibility::Collapsed);
}