#include "UI/Gacha/GachaHighgradeWidget.h"

#include "Core/ARGameInstance.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UGachaHighgradeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CurrentState = EGachaHighgradeState::IntroDialogue;

	if (DialogueAnim) {
		FWidgetAnimationDynamicEvent EndDelegate;
		EndDelegate.BindDynamic(this, &UGachaHighgradeWidget::OnDialogueFinished);
		BindToAnimationFinished(DialogueAnim, EndDelegate);
	}
	if (SilhouetteAnim) {
		FWidgetAnimationDynamicEvent EndDelegate;
		EndDelegate.BindDynamic(this, &UGachaHighgradeWidget::OnSilhouetteFinished);
		BindToAnimationFinished(SilhouetteAnim, EndDelegate);
	}
	if (RevealAnim) {
		FWidgetAnimationDynamicEvent EndDelegate;
		EndDelegate.BindDynamic(this, &UGachaHighgradeWidget::OnRevealFinished);
		BindToAnimationFinished(RevealAnim, EndDelegate);
	}

	PlayNextState();
}

void UGachaHighgradeWidget::SkipToFinal()
{
	StopAllAnimations();
	CurrentState = EGachaHighgradeState::FinalReveal;
	if (RevealAnim) PlayAnimation(RevealAnim);
}

void UGachaHighgradeWidget::InitializeGacha(const FGachaItemResult& ItemData, const FText& InText)
{
	GachaDialogueText = InText;
	if (DialogueText) DialogueText->SetText(GachaDialogueText);

	// TODO: 나중에 ItemData 기반 이미지 세팅
	// if (RevealImage && ItemData.CharacterTexture)
	// {
	//     RevealImage->SetBrushFromTexture(ItemData.CharacterTexture);
	// }
}

FReply UGachaHighgradeWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	SkipToFinal();
	return FReply::Handled();
}

void UGachaHighgradeWidget::PlayNextState()
{
	switch (CurrentState) {
	case EGachaHighgradeState::IntroDialogue:
		if (DialogueAnim) PlayAnimation(DialogueAnim);
		break;

	case EGachaHighgradeState::SilhouetteCamera:
		if (SilhouetteAnim) PlayAnimation(SilhouetteAnim);
		break;

	case EGachaHighgradeState::RevealZoomOut:
		CurrentState = EGachaHighgradeState::FinalReveal;
		PlayNextState();
		break;

	case EGachaHighgradeState::FinalReveal:
		if (RevealAnim) PlayAnimation(RevealAnim);
		break;

	default:
		break;
	}
}
void UGachaHighgradeWidget::OnDialogueFinished()
{
	CurrentState = EGachaHighgradeState::SilhouetteCamera;
	OnSilhouetteStart.Broadcast();
	PlayNextState();
}
void UGachaHighgradeWidget::OnSilhouetteFinished()
{
	CurrentState = EGachaHighgradeState::FinalReveal;
}
void UGachaHighgradeWidget::OnSilhouetteCameraMovingFinished()
{
	PlayNextState();
}
void UGachaHighgradeWidget::OnRevealFinished()
{
	CurrentState = EGachaHighgradeState::End;
}