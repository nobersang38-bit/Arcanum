#include "UI/Skill/BasicAttackWidget.h"
#include "Components/Button.h"

void UBasicAttackWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BindButtonEvents();
}

void UBasicAttackWidget::NativeDestruct()
{
	if (ButtonBasicAttack)
	{
		ButtonBasicAttack->OnClicked.RemoveDynamic(this, &UBasicAttackWidget::HandleBasicAttackClicked);
	}

	Super::NativeDestruct();
}

void UBasicAttackWidget::BindButtonEvents()
{
	if (ButtonBasicAttack)
	{
		ButtonBasicAttack->OnClicked.RemoveDynamic(this, &UBasicAttackWidget::HandleBasicAttackClicked);
		ButtonBasicAttack->OnClicked.AddDynamic(this, &UBasicAttackWidget::HandleBasicAttackClicked);
	}
}

void UBasicAttackWidget::HandleBasicAttackClicked()
{
	OnBasicAttackClickedRequested.Broadcast();
}
