// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Actor/FloatingDamageText.h"
#include "UI/Battle/Common/FloatingDamageTextWidget.h"
#include "Components/WidgetComponent.h"
#include "Core/SubSystem/PoolingSubsystem.h"
#include "Animation/WidgetAnimation.h"

// Sets default values
AFloatingDamageText::AFloatingDamageText()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	FloatingWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("FloatingWidgetComponent"));
	SetRootComponent(FloatingWidgetComponent);
}

// Called when the game starts or when spawned
void AFloatingDamageText::BeginPlay()
{
	Super::BeginPlay();
	
	if (UFloatingDamageTextWidget* FDTW = Cast<UFloatingDamageTextWidget>(FloatingWidgetComponent->GetWidget()))
	{
		CashedFloatingDamageTextWidget = FDTW;

		if (CashedFloatingDamageTextWidget.IsValid())
		{
			CashedFloatingDamageTextWidget->SetVisibility(ESlateVisibility::Hidden);
			FWidgetAnimationDynamicEvent AnimFinishEvent;
			AnimFinishEvent.BindUFunction(this, FName("Internal_Deactive"));
			CashedFloatingDamageTextWidget->BindToAnimationFinished(CashedFloatingDamageTextWidget->HideAnim, AnimFinishEvent);
		}
	}
}

// Called every frame
void AFloatingDamageText::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFloatingDamageText::SetText(float InValue)
{
	if (CashedFloatingDamageTextWidget.IsValid())
	{
		FString Result = FString::Printf(TEXT("%.0f"), InValue);
		CashedFloatingDamageTextWidget->SetText(FText::FromString(Result));
	}
}

void AFloatingDamageText::SetText(FText InText)
{
	if (CashedFloatingDamageTextWidget.IsValid())
	{
		CashedFloatingDamageTextWidget->SetText(InText);
	}
}

void AFloatingDamageText::ActivateItem()
{
	if (CashedFloatingDamageTextWidget.IsValid() && CashedFloatingDamageTextWidget->HideAnim)
	{
		CashedFloatingDamageTextWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		CashedFloatingDamageTextWidget->PlayAnimation(CashedFloatingDamageTextWidget->HideAnim);
	}
}

void AFloatingDamageText::DeactiveItem()
{
	if (CashedFloatingDamageTextWidget.IsValid() && CashedFloatingDamageTextWidget->HideAnim)
	{
		CashedFloatingDamageTextWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AFloatingDamageText::Internal_Deactive()
{
	UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>();
	if (PoolingSubsystem)
	{
		PoolingSubsystem->DeactiveItem(this);
	}
}

