#include "UI/Gacha/SubLayout/GachaItemContainerSubWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UGachaItemContainerSubWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if(!ItemIcon) UE_LOG(LogTemp, Log, TEXT("뭔가 없다"));
	if(!RarityBG) UE_LOG(LogTemp, Log, TEXT("뭔가 없다"));
	if(!ItemNameText) UE_LOG(LogTemp, Log, TEXT("뭔가 없다"));
	if (!AmountText) UE_LOG(LogTemp, Log, TEXT("뭔가 없다"));
}
void UGachaItemContainerSubWidget::SetItemData(const FGachaItemResult& InData)
{
    CachedData = InData;

    if (RarityBG) {
        bool bFound = false;
        for (const auto& Pair : GradeMatMap) {
            if (CachedData.GradeTag.MatchesTag(Pair.Key)) {
                if (UTexture2D* const* FoundTex = Pair.Value.GrageTexture2D.Find(CachedData.GradeTag)) {
                    RarityBG->SetBrushFromTexture(*FoundTex);
                    bFound = true;
                    break;
                }
            }
        }
        if (!bFound) UE_LOG(LogTemp, Log, TEXT("태그에 맞는 텍스쳐 없음"));
    }

    UDataTable* TargetTable = CachedData.SourceTable.LoadSynchronous();
    if (TargetTable) {
        FName RowName = GetLeafNameFromTag(CachedData.ItemTag);
        static const FString Context(TEXT("GachaItemContext"));
        if (CachedData.ItemTag.MatchesTag(Arcanum::Player::ID::Root)) {
            auto* Row = TargetTable->FindRow<FDTCharacterBaseInfoRow>(RowName, Context);
            if (Row) {
                if (ItemIcon) ItemIcon->SetBrushFromTexture(Row->BattleCharacterInfo.CharacterIcon.LoadSynchronous());
                if (ItemNameText) ItemNameText->SetText(CachedData.ItemName);
            }
        }
        else if (CachedData.ItemTag.MatchesTag(Arcanum::Items::Rarity::Root)) {
            auto* Row = TargetTable->FindRow<FDTEquipmentInfoRow>(RowName, Context);
            if (Row) {
                if (ItemIcon) ItemIcon->SetBrushFromTexture(Row->Icon.LoadSynchronous());
                if (ItemNameText) ItemNameText->SetText(CachedData.ItemName);
            }
        }
    }

    if (AmountText) {
        if (CachedData.bIsNew) {
            AmountText->SetText(FText::FromString(TEXT("NEW")));
            AmountText->SetVisibility(ESlateVisibility::HitTestInvisible);
        }
        else if (CachedData.Quantity > 1) {
            AmountText->SetText(FText::Format(
                FText::FromString(TEXT("x{0}")),
                FText::AsNumber(CachedData.Quantity)
            ));
            AmountText->SetVisibility(ESlateVisibility::HitTestInvisible);
        }
        else AmountText->SetVisibility(ESlateVisibility::Hidden);
    }
    PlayAppearAnimation();
}