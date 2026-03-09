#include "UI/Gacha/GachaLevelHUD.h"

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
	UE_LOG(LogTemp, Log, TEXT("Gacha Skip Button Clicked"));

}