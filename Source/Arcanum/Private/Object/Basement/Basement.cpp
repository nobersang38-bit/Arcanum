// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/Basement/Basement.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Components/WidgetComponent.h"
#include "UI/InGame/UnitHealthWidget.h"
#include "Component/BasementCombatComponent.h"

// Sets default values
ABasement::ABasement()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//CurrentBasementHealth = MaxBasementHealth;

	// 캡슐 말고 스태틱 메시의 콜리전 사용
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	BasementCombatComponent = CreateDefaultSubobject<UBasementCombatComponent>(TEXT("BasementCombatComponent"));

	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidgetComponent"));
	HealthWidgetComponent->SetupAttachment(RootComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CrystalMesh"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionProfileName(TEXT("BlockAll"));
}

// Called when the game starts or when spawned
void ABasement::BeginPlay()
{
	Super::BeginPlay();

	//if (CommonEnemyClass)
	//{
	//	GetWorld()->GetTimerManager().SetTimer(
	//		SpawnTimerHandle,
	//		this,
	//		&ABasement::EnemySpawnTick,
	//		SpawnCommonInterval,
	//		true
	//	);
	//}
	if (UUserWidget* TempHealthWidgetUser = Cast<UUserWidget>(HealthWidgetComponent->GetWidget()))
	{
		if (UUnitHealthWidget* TempHealthWidget = Cast<UUnitHealthWidget>(TempHealthWidgetUser))
		{
			BasementCombatComponent->OnBasementChangeHealth.RemoveDynamic(TempHealthWidget, &UUnitHealthWidget::SetPercentFloat);
			BasementCombatComponent->OnBasementChangeHealth.AddDynamic(TempHealthWidget, &UUnitHealthWidget::SetPercentFloat);

			FEnemyBasement Stat = BasementCombatComponent->GetBasementStat();
			float CurrentHealth = Stat.CommandCenterHP.GetTotalValue();
			float MaxHealth = CurrentHealth;
			TempHealthWidget->SetPercentFloat(CurrentHealth, MaxHealth);
		}
	}
}

FGameplayTag ABasement::GetTeamTag()
{

	return TeamTag;
}

