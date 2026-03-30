#include "UI/Gacha/GachaHighgradeActor.h"

#include "UI/Gacha/GachaHighgradeWidget.h"
#include "Core/ARGameInstance.h"

#include "Components/WidgetComponent.h"

AGachaHighgradeActor::AGachaHighgradeActor()
{
	PrimaryActorTick.bCanEverTick = false;

	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	RootComponent = WidgetComp;

	WidgetComp->SetWidgetSpace(EWidgetSpace::World);
	WidgetComp->SetDrawSize(FVector2D(1920, 1080));
	WidgetComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}
void AGachaHighgradeActor::InitializeWidget(const FGachaItemResult& ItemData, const FText& Dialog)
{
	CachedWidget = Cast<UGachaHighgradeWidget>(WidgetComp->GetUserWidgetObject());
	if (CachedWidget) {
		CachedWidget->InitializeGacha(ItemData, Dialog);
		CachedWidget->OnSilhouetteStart.AddDynamic(this, &AGachaHighgradeActor::HandleSilhouetteStart);
	}
}
void AGachaHighgradeActor::HandleSilhouetteStart()
{
	OnSilhouetteStart.Broadcast();
}