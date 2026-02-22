#include "UI/PlayerHUD/PlayerHUD.h"
#include "UI/Skill/BasicAttackWidget.h"   
#include "UI/Skill/AwakenWidget.h"        
#include "Component/SkillComponent.h"
#include "Component/EquipmentComponent.h"

void UPlayerHUD::NativeConstruct()
{
	Super::NativeConstruct();

	BindOwnerComponents();
	BindWidgetEvents();
	BindComponentDelegates();
	RefreshAllUI();
}

void UPlayerHUD::NativeDestruct()
{
	if (SkillComponent)
	{
		SkillComponent->OnGaugePercentChanged.RemoveDynamic(this, &UPlayerHUD::HandleGaugePercentChanged);
		SkillComponent->OnAwakenStateChanged.RemoveDynamic(this, &UPlayerHUD::HandleAwakenStateChanged);
	}

	if (EquipmentComponent)
	{
		EquipmentComponent->OnEquipChanged.RemoveDynamic(this, &UPlayerHUD::HandleEquipChanged);
	}

	if (BasicAttackWidget)
	{
		BasicAttackWidget->OnBasicAttackClickedRequested.RemoveDynamic(this, &UPlayerHUD::HandleBasicAttackClicked);
	}

	if (AwakenWidget)
	{
		AwakenWidget->OnAwakenClickedRequested.RemoveDynamic(this, &UPlayerHUD::HandleAwakenClicked);
	}

	Super::NativeDestruct();
}

void UPlayerHUD::BindOwnerComponents()
{
	if (APawn* owningPawn = GetOwningPlayerPawn())
	{ 
		if (!SkillComponent)
		{ 
			SkillComponent = owningPawn->FindComponentByClass<USkillComponent>();
		}
		
		if (!EquipmentComponent) 
		{ 
			EquipmentComponent = owningPawn->FindComponentByClass<UEquipmentComponent>();
		}
	}
}

void UPlayerHUD::BindWidgetEvents()
{
	if (BasicAttackWidget)
	{
		BasicAttackWidget->OnBasicAttackClickedRequested.RemoveDynamic(this, &UPlayerHUD::HandleBasicAttackClicked);
		BasicAttackWidget->OnBasicAttackClickedRequested.AddDynamic(this, &UPlayerHUD::HandleBasicAttackClicked);
	}

	if (AwakenWidget)
	{
		AwakenWidget->OnAwakenClickedRequested.RemoveDynamic(this, &UPlayerHUD::HandleAwakenClicked);
		AwakenWidget->OnAwakenClickedRequested.AddDynamic(this, &UPlayerHUD::HandleAwakenClicked);
	}
}

void UPlayerHUD::BindComponentDelegates()
{
	if (SkillComponent)
	{
		SkillComponent->OnGaugePercentChanged.RemoveDynamic(this, &UPlayerHUD::HandleGaugePercentChanged);
		SkillComponent->OnGaugePercentChanged.AddDynamic(this, &UPlayerHUD::HandleGaugePercentChanged);

		SkillComponent->OnAwakenStateChanged.RemoveDynamic(this, &UPlayerHUD::HandleAwakenStateChanged);
		SkillComponent->OnAwakenStateChanged.AddDynamic(this, &UPlayerHUD::HandleAwakenStateChanged);
	}

	if (EquipmentComponent)
	{
		EquipmentComponent->OnEquipChanged.RemoveDynamic(this, &UPlayerHUD::HandleEquipChanged);
		EquipmentComponent->OnEquipChanged.AddDynamic(this, &UPlayerHUD::HandleEquipChanged);
	}
}

void UPlayerHUD::RefreshAllUI()
{
	if (SkillComponent)
	{
		const float gaugePercent = SkillComponent->GetGaugePercent();

		if (AwakenWidget)
		{
			AwakenWidget->RefreshAwakenVisual(gaugePercent);
		}
	}
}

void UPlayerHUD::HandleBasicAttackClicked()
{
	if (SkillComponent)
	{
		FGameplayTag basicAttackSkillTag = SkillComponent->GetBasicAttackSkillTag();

		if (basicAttackSkillTag.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("BasicAttackSkillTag = %s"), *basicAttackSkillTag.ToString());

			SkillComponent->ActivateSkill(basicAttackSkillTag);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("HandleBasicAttackClicked"));
}

void UPlayerHUD::HandleAwakenClicked()
{
	if (SkillComponent)
	{
		SkillComponent->ActivateAwaken();
	}
}

void UPlayerHUD::HandleEquipChanged(EEquipSlot InSlot, FGameplayTag InNewEquipTag)
{
	if (InSlot == EEquipSlot::Weapon)
	{
		/* 기본공격 아이콘 */
	}
}

void UPlayerHUD::HandleGaugePercentChanged(float InPercent)
{
	if (AwakenWidget)
	{
		AwakenWidget->SetGaugePercent(InPercent);
	}
}

void UPlayerHUD::HandleAwakenStateChanged(bool bInAwakenActive, FGameplayTag InAwakenTag)
{
	if (AwakenWidget)
	{
		AwakenWidget->SetAwakenState(bInAwakenActive);
	}
}
