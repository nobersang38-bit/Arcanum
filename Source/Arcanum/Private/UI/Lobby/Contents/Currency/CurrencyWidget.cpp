#include "UI/Lobby/Contents/Currency/CurrencyWidget.h"
#include "GameplayTags/ArcanumTags.h"
#include "Components/TextBlock.h"

void UCurrencyWidget::NativeConstruct()
{
	// 초기값
	if (GoldText)
	{
		GoldText->SetText(FText::AsNumber(0));
	}

	if (ShardText)
	{
		ShardText->SetText(FText::AsNumber(0));
	}

	if (SoulText)
	{
		SoulText->SetText(FText::AsNumber(0));
	}
}

void UCurrencyWidget::RefreshCurrencyUI(const FPlayerData& InPlayerData)
{
	const FCurrencyData* goldData = InPlayerData.PlayerCurrency.CurrencyDatas.Find(Arcanum::PlayerData::Currencies::NonRegen::Gold::Value);
	const FCurrencyData* shardData = InPlayerData.PlayerCurrency.CurrencyDatas.Find(Arcanum::PlayerData::Currencies::NonRegen::Shard::Value);
	const FCurrencyData* soulData = InPlayerData.PlayerCurrency.CurrencyDatas.Find(Arcanum::PlayerData::Currencies::NonRegen::Soul::Value);

	const int64 goldAmount = (goldData) ? goldData->CurrAmount : 0;
	const int64 shardAmount = (shardData) ? shardData->CurrAmount : 0;
	const int64 soulAmount = (soulData) ? soulData->CurrAmount : 0;

	if (GoldText)
	{
		GoldText->SetText(FText::AsNumber(goldAmount));
	}

	if (ShardText)
	{
		ShardText->SetText(FText::AsNumber(shardAmount));
	}

	if (SoulText)
	{
		SoulText->SetText(FText::AsNumber(soulAmount));
	}
}
