#include "UI/Gacha/GachaLevelHUD.h"
#include "UI/Gacha/ARGachaController.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UGachaLevelHUD::NativeConstruct()
{
	Super::NativeConstruct();

	if (SkipButton) {
		SkipButton->OnClicked.RemoveDynamic(this, &UGachaLevelHUD::OnSkipClicked);
		SkipButton->OnClicked.AddDynamic(this, &UGachaLevelHUD::OnSkipClicked);
	}
}
void UGachaLevelHUD::OnSkipClicked()
{
	AARGachaController* PC = Cast<AARGachaController>(GetOwningPlayer());
	if (PC) PC->RequestSkipGacha();
}