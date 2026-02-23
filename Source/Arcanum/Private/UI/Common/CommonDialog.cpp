#include "UI/Common/CommonDialog.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"

void UCommonDialog::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (TitleText) TitleText->SetText(TitleTxt);
	if (MessageText) MessageText->SetText(MessageTxt);

	if (TitleImg && TitleTexture) TitleImg->SetBrushFromTexture(TitleTexture);
	if (MessageImg && MessageTexture) MessageImg->SetBrushFromTexture(MessageTexture);

	if (CancelButton) CancelButton->SetVisibility(DialogType == EDialogType::OKCancel ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UCommonDialog::NativeConstruct()
{
	Super::NativeConstruct();

	if (OKButton) {
		OKButton->OnClicked.RemoveDynamic(this, &UCommonDialog::OnOKClicked);
		OKButton->OnClicked.AddDynamic(this, &UCommonDialog::OnOKClicked);
	}
	if (CancelButton) {
		CancelButton->OnClicked.RemoveDynamic(this, &UCommonDialog::OnCancelClicked);
		CancelButton->OnClicked.AddDynamic(this, &UCommonDialog::OnCancelClicked);
	}
}

void UCommonDialog::OnOKClicked()
{
	OnResult.Broadcast(EDialogResult::OK);
}

void UCommonDialog::OnCancelClicked()
{
	OnResult.Broadcast(EDialogResult::Cancel);
}