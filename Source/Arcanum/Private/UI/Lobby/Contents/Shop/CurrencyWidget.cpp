#include "UI/Lobby/Contents/Shop/CurrencyWidget.h"
#include "Core/ARGameInstance.h"
#include "Components/TextBlock.h"

void UCurrencyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (UARGameInstance* gameInstance = GetGameInstance<UARGameInstance>())
	{
		gameInstance->OnCurrencyChanged.AddDynamic(this, &UCurrencyWidget::RefreshCurrencyUI);
	}

	RefreshCurrencyUI();
}

void UCurrencyWidget::NativeDestruct()
{
	if (UARGameInstance* gameInstance = GetGameInstance<UARGameInstance>())
	{
		gameInstance->OnCurrencyChanged.RemoveDynamic(this, &UCurrencyWidget::RefreshCurrencyUI);
	}

	Super::NativeDestruct();
}

void UCurrencyWidget::RefreshCurrencyUI()
{
	if (UWorld* world = GetWorld())
	{
		if (UARGameInstance* gameInstance = Cast<UARGameInstance>(world->GetGameInstance()))
		{
			const FPlayerData& playerData = gameInstance->GetPlayerData();

			const FCurrencyData* goldData = playerData.PlayerCurrency.CurrencyDatas.Find(Arcanum::PlayerData::Currencies::NonRegen::Gold::Value);
			const FCurrencyData* shardData = playerData.PlayerCurrency.CurrencyDatas.Find(Arcanum::PlayerData::Currencies::NonRegen::Shard::Value);
			const FCurrencyData* soulData = playerData.PlayerCurrency.CurrencyDatas.Find(Arcanum::PlayerData::Currencies::NonRegen::Soul::Value);

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
	}
}