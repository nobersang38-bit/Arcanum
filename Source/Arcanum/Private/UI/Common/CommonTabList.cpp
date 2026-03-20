#include "UI/Common/CommonTabList.h"
#include "GameplayTags/ArcanumTags.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "Components/WidgetSwitcher.h"

/// 260318 추가
void UCommonTabList::NativeConstruct()
{
    Super::NativeConstruct();

    if (PrevBtn) {
        PrevBtn->OnClicked.RemoveDynamic(this, &UCommonTabList::OnPrevButtonClicked);
        PrevBtn->OnClicked.AddDynamic(this, &UCommonTabList::OnPrevButtonClicked);
    }
    if (NextBtn) {
        NextBtn->OnClicked.RemoveDynamic(this, &UCommonTabList::OnNextButtonClicked);
        NextBtn->OnClicked.AddDynamic(this, &UCommonTabList::OnNextButtonClicked);
    }
}

void UCommonTabList::InitializeTabs()
{
    if (!Container) return;

    for (const auto& Pair : TabClasses) {
        const FGameplayTag& Tag = Pair.Key;
        TSubclassOf<UUserWidget> WidgetClass = Pair.Value;

        if (!WidgetClass) continue;

        UUserWidget* Widget = CreateWidget<UUserWidget>(GetOwningPlayer(), WidgetClass);

        if (Widget) {
            Container->AddChild(Widget);
            Widget->SetVisibility(ESlateVisibility::Hidden);

            TabInstances.Add(Tag, Widget);
        }
    }

    if (TabOrder.Num() > 0) SetCurrentTab(TabOrder[0]);
}
void UCommonTabList::GoToNext()
{
    if (TabOrder.Num() == 0) return;

    int32 Index = TabOrder.IndexOfByKey(CurrentTag);
    if (Index == INDEX_NONE) return;

    Index = (Index + 1) % TabOrder.Num();
    SetCurrentTab(TabOrder[Index]);
}

void UCommonTabList::GoToPrev()
{
    if (TabOrder.Num() == 0) return;

    int32 Index = TabOrder.IndexOfByKey(CurrentTag);
    if (Index == INDEX_NONE) return;

    Index = (Index - 1 + TabOrder.Num()) % TabOrder.Num();
    SetCurrentTab(TabOrder[Index]);
}
void UCommonTabList::SetCurrentTab(FGameplayTag NewTag)
{
    if (CurrentTag == NewTag) return;

    for (auto& Pair : TabInstances) {
        if (UUserWidget* TabWidget = Pair.Value) {
            ESlateVisibility NewVisibility = (Pair.Key == NewTag) ?
                ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed;

            TabWidget->SetVisibility(NewVisibility);
        }
    }

    UUserWidget* TargetWidget = TabInstances.FindRef(NewTag);
    if (TargetWidget && TabSwitcher) {
        TargetWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
        TabSwitcher->SetActiveWidget(TargetWidget);
        CurrentTag = NewTag;
        if (TabNameText) {
            FString DisplayName = GetLeafNameFromTag(NewTag).ToString().ToUpper();
            TabNameText->SetText(FText::FromString(DisplayName));
        }
    }
}
void UCommonTabList::OnPrevButtonClicked() { GoToPrev(); }
void UCommonTabList::OnNextButtonClicked() { GoToNext(); }