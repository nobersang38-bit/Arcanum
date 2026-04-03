#include "UI/Gacha/GachaHighgradeWidget.h"

#include "Core/ARGameInstance.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"

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
	
	if (DialogueAnim) {
		float EndTime = DialogueAnim->GetEndTime();
		PlayAnimation(DialogueAnim);
		SetAnimationCurrentTime(RevealAnim, EndTime);
		PauseAnimation(RevealAnim);
	}

	if (RevealAnim) PlayAnimation(RevealAnim);
	if (OnSkipRequested.IsBound()) OnSkipRequested.Broadcast();
}

void UGachaHighgradeWidget::InitializeGacha(const FGachaItemResult& ItemData, const FText& InText)
{
	GachaDialogueText = InText;
	if (DialogueText) DialogueText->SetText(GachaDialogueText);

	// TODO: 나중에 ItemData 기반 이미지 세팅
	 if (RevealImage && ItemData.CharacterColorTexture)
	 {
	     RevealImage->SetBrushFromTexture(ItemData.CharacterColorTexture.LoadSynchronous());
	 }
	 if (SilhouetteImage && ItemData.CharacterSilhouetteTexture)
	 {
		 SilhouetteImage->SetBrushFromTexture(ItemData.CharacterSilhouetteTexture.LoadSynchronous());
	 }
	 if (BackGround && ItemData.CharacterBgTexture)
	 {
		 BackGround->SetBrushFromTexture(ItemData.CharacterBgTexture.LoadSynchronous());
	 }
}

FReply UGachaHighgradeWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (CurrentState == EGachaHighgradeState::End) {
		if (OnGachaFinished.IsBound()) OnGachaFinished.Broadcast();
		return FReply::Handled();
	}

	if (!Isclicked) {
		Isclicked = !Isclicked;
		SkipToFinal();
		CurrentState = EGachaHighgradeState::End;
	}

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
	if (Isclicked) return;

	CurrentState = EGachaHighgradeState::SilhouetteCamera;
	OnSilhouetteStart.Broadcast();
	PlayNextState();
}
void UGachaHighgradeWidget::OnSilhouetteFinished()
{
	if (Isclicked) return;
	CurrentState = EGachaHighgradeState::FinalReveal;
}
void UGachaHighgradeWidget::OnSilhouetteCameraMovingFinished()
{
	if (Isclicked) return;
	PlayNextState();
}
void UGachaHighgradeWidget::OnRevealFinished()
{
	if (Isclicked) return;
	CurrentState = EGachaHighgradeState::End;
	//if (OnGachaFinished.IsBound()) OnGachaFinished.Broadcast();
}