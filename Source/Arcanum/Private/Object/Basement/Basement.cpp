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
#include "Component/Stats/CharacterBattleStatsComponent.h"
#include "Component/StatusActionComponent.h"

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

	CharacterBattleStatsComponent = CreateDefaultSubobject<UCharacterBattleStatsComponent>(TEXT("CharacterBattleStatsComponent"));

	StatusActionComponent = CreateDefaultSubobject<UStatusActionComponent>(TEXT("StatusActionComponent"));
}

UCharacterBattleStatsComponent* ABasement::GetStatComponent()
{
	return CharacterBattleStatsComponent;
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
	StatusActionComponent->SetupAction();
	if (UUserWidget* TempHealthWidgetUser = Cast<UUserWidget>(HealthWidgetComponent->GetWidget()))
	{
		if (UUnitHealthWidget* TempHealthWidget = Cast<UUnitHealthWidget>(TempHealthWidgetUser))
		{
			//CharacterBattleStatsComponent->OnCharacterRegenStatChanged.Remove(TempHealthWidget, &UUnitHealthWidget::SetPercent);
			CharacterBattleStatsComponent->OnCharacterRegenStatChanged.AddUObject(TempHealthWidget, &UUnitHealthWidget::SetPercent);

			const FRegenStat* Stat = CharacterBattleStatsComponent->FindRegenStat(Arcanum::BattleStat::Character::Regen::Health::Root);
			TempHealthWidget->SetPercentFloat(Stat->Current, Stat->BaseMax);
		}
	}
}

FGameplayTag ABasement::GetTeamTag() const
{
	return TeamTag;
}

void ABasement::AddLevelModifierEntry(const FLevelModifierEntry& LevelModifierEntry)
{

}

void ABasement::AddDerivedStatModifier(const FDerivedStatModifier& DerivedStatModifier)
{
	CharacterBattleStatsComponent->ApplyDurationModifier(DerivedStatModifier);
}

void ABasement::ChangeStat(const FGameplayTag& InTag, float InValue)
{
	CharacterBattleStatsComponent->ChangeStatValue(InTag, InValue, nullptr);
}

