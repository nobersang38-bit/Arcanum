// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseUnitCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Data/DataAssets/DATargetPriorityWeight.h"
#include "Core/SubSystem/BattlefieldManagerSubsystem.h"
#include "GameplayTags/ArcanumTags.h"
#include "Components/SphereComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Component/UnitCombatComponent.h"
#include "Data/Rows/AllyUnitsDataRow.h"
#include "Data/Rows/EnemyUnitsDataRow.h"
#include "Component/Stats/CharacterBattleStatsComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/CharacterHUD/CharacterHealthWidget.h"
#include "GameplayTags/ArcanumTags.h"

// Sets default values
ABaseUnitCharacter::ABaseUnitCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	UnitCombatComponent = CreateDefaultSubobject<UUnitCombatComponent>(TEXT("UnitCombatComponent"));
	CharacterBattleStatsComponent = CreateDefaultSubobject<UCharacterBattleStatsComponent>(TEXT("CharacterBattleStatsComponent"));
	HealthBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarComponent"));
	HealthBarComponent->SetupAttachment(RootComponent);

	GetCharacterMovement()->bUseRVOAvoidance = true;
	GetCharacterMovement()->AvoidanceConsiderationRadius = 200.0f;
	GetCharacterMovement()->SetRVOAvoidanceWeight(0.0f);

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

FGameplayTag ABaseUnitCharacter::GetTeamTag()
{
	if (UnitData.Stat.CommonStat.TeamID.IsValid())
	{
		return UnitData.Stat.CommonStat.TeamID;
	}

	return FGameplayTag::EmptyTag;
};

void ABaseUnitCharacter::BeginPlay()
{
	Super::BeginPlay();

	DataInitialize();
	if (UUserWidget* TempHealthWidgetUser = Cast<UUserWidget>(HealthBarComponent->GetWidget()))
	{
		UE_LOG(LogTemp, Warning, TEXT("체력 변경 발동1"));
		if (UCharacterHealthWidget* TempHealthWidget = Cast<UCharacterHealthWidget>(TempHealthWidgetUser))
		{
			UE_LOG(LogTemp, Warning, TEXT("체력 변경 발동2"));
			CharacterBattleStatsComponent->OnCharacterRegenStatChanged.AddUObject(TempHealthWidget, &UCharacterHealthWidget::SetPercent);

			CharacterBattleStatsComponent->ChangeStatValue(Arcanum::BattleStat::Character::Regen::Health::Root, -1.0f, nullptr);
		}
	}
}

void ABaseUnitCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}
#if WITH_EDITOR
void ABaseUnitCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName MemberPropertyName = (PropertyChangedEvent.MemberProperty != nullptr) ? PropertyChangedEvent.MemberProperty->GetFName() : NAME_None;

	if (MemberPropertyName == GET_MEMBER_NAME_CHECKED(ABaseUnitCharacter, DTUnitDataRowHandle))
	{
		if (DTUnitDataRowHandle.DataTable && !DTUnitDataRowHandle.RowName.IsNone()) 
		{
			if (const FAllyUnitsDataRow* AllyRow = DTUnitDataRowHandle.DataTable->FindRow<FAllyUnitsDataRow>(DTUnitDataRowHandle.RowName, TEXT("Load")))
			{
				if (AllyRow) UnitData = (*AllyRow).UnitData;
			}
			else if(const FEnemyUnitsDataRow* EnemyRow = DTUnitDataRowHandle.DataTable->FindRow<FEnemyUnitsDataRow>(DTUnitDataRowHandle.RowName, TEXT("Load")))
			{
				if (EnemyRow) UnitData = (*EnemyRow).UnitData;
			}
		}
	}
}
#endif

void ABaseUnitCharacter::DataInitialize()
{
	if (!UnitData.Info.BodySetting.SkeletalMesh.IsNull())// 스켈레탈 메시 설정
	{
		if (USkeletalMesh* SkeletalMesh = UnitData.Info.BodySetting.SkeletalMesh.LoadSynchronous())
		{
			GetMesh()->SetSkeletalMesh(SkeletalMesh);
		}
	}

	if (UnitData.Info.AnimSetting.AnimInstance) // 애님 인스턴스 설정
	{
		GetMesh()->SetAnimInstanceClass(UnitData.Info.AnimSetting.AnimInstance);
	}
	/*GetCharacterMovement()->MaxWalkSpeed = Speed;
	UnitData.Info.AISetting.AttackRange = Range;*/
}

FUnitRuntimeData& ABaseUnitCharacter::GetUnitRuntimeData()
{
	return UnitCombatComponent->GetUnitRuntimeData();
}

void ABaseUnitCharacter::OnAttackNotifyTriggered()
{
	UnitCombatComponent->TakeDamage();
}

void ABaseUnitCharacter::ApplyDamage(float InDamage, AActor* DamageCauser)
{
	UnitCombatComponent->ApplyDamage(InDamage, DamageCauser);
}

const FUnitData& ABaseUnitCharacter::GetUnitData()
{
	return UnitData;
}

