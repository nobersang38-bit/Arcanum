#include "UI/Gacha/GachaFinalResultWidget.h"
#include "UI/Gacha/SubLayout/GachaItemContainerSubWidget.h"
#include "Core/ARGameInstance.h"

#include "Components/WrapBox.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "Components/OverlaySlot.h"

void UGachaFinalResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ReturnButton) {
		ReturnButton->OnClicked.RemoveAll(this);
		ReturnButton->OnClicked.AddDynamic(this, &UGachaFinalResultWidget::HandleReturnClicked);
	}
}

void UGachaFinalResultWidget::SetGachaResults(const TArray<FGachaItemResult>& InResults)
{
	if (!IconContainer || !ResultItemWidgetClass) return;

	IconContainer->ClearChildren();
	PendingResults = InResults;
	CurrentDisplayIndex = 0;

	GetWorld()->GetTimerManager().ClearTimer(DisplayTimerHandle);
	OptimizeLayout(PendingResults.Num());

	if (PendingResults.Num() > 0) {
		float DisplayInterval = 0.1f;
		GetWorld()->GetTimerManager().SetTimer(DisplayTimerHandle, this, &UGachaFinalResultWidget::SpawnNextItem, DisplayInterval, true );
	}
}

void UGachaFinalResultWidget::OptimizeLayout(int32 ItemCount)
{
	if (!IconContainer) return;
	
	IconContainer->SetHorizontalAlignment(HAlign_Center);

	if (ContainerSizeBox) {
		float MaxHeight = (ItemCount <= 10) ? 450.0f : 800.0f;
		ContainerSizeBox->SetMaxDesiredHeight(MaxHeight);
	}

	if (UOverlaySlot* ScrollSlot = Cast<UOverlaySlot>(ResultScrollBox->Slot)) {
		if (PendingResults.Num() > 10) {
			ScrollSlot->SetPadding(FMargin(0.f, 100.f, 0.f, 100.f));
			ContainerSizeBox->SetHeightOverride(800.f);
		}
	}
}

void UGachaFinalResultWidget::HandleReturnClicked()
{
	int64 temp = 0;
	for (auto i : PendingResults) {
		if (!i.bIsNew) temp += i.Quantity;
	}
	// 0402 중복가챠 소울->조각으로 변경
	FPlayerAccountService::AddCurrency(this, Arcanum::PlayerData::Currencies::NonRegen::Shard::Value, temp);
	OnReturnLobbyClicked.Broadcast();
}

void UGachaFinalResultWidget::SpawnNextItem()
{
	if (!PendingResults.IsValidIndex(CurrentDisplayIndex)) {
		GetWorld()->GetTimerManager().ClearTimer(DisplayTimerHandle);
		return;
	}

	UGachaItemContainerSubWidget* NewItem = CreateWidget<UGachaItemContainerSubWidget>(this, ResultItemWidgetClass);
	if (NewItem) {
		//PendingResults[CurrentDisplayIndex].Quantity++;
		NewItem->SetItemData(PendingResults[CurrentDisplayIndex]);
		
		IconContainer->AddChildToWrapBox(NewItem);
		if (ResultScrollBox) ResultScrollBox->ScrollToEnd();

		UE_LOG(LogTemp, Warning, TEXT("Size: %s"), *NewItem->GetDesiredSize().ToString());
	}

	CurrentDisplayIndex++;
}

void UGachaFinalResultWidget::SkipAnimation()
{
	GetWorld()->GetTimerManager().ClearTimer(DisplayTimerHandle);
	for (int32 i = CurrentDisplayIndex; i < PendingResults.Num(); ++i) {
		if (UGachaItemContainerSubWidget* NewItem = CreateWidget<UGachaItemContainerSubWidget>(this, ResultItemWidgetClass)) {
			NewItem->SetItemData(PendingResults[i]);
			IconContainer->AddChildToWrapBox(NewItem);
		}
	}

	CurrentDisplayIndex = PendingResults.Num();
	if (ResultScrollBox) ResultScrollBox->ScrollToEnd();
}