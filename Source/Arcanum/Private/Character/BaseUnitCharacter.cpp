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
#include "Kismet/GameplayStatics.h"
#include "GameplayTagsManager.h"

// Sets default values
ABaseUnitCharacter::ABaseUnitCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	UnitCombatComponent = CreateDefaultSubobject<UUnitCombatComponent>(TEXT("UnitCombatComponent"));
	CharacterBattleStatsComponent = CreateDefaultSubobject<UCharacterBattleStatsComponent>(TEXT("CharacterBattleStatsComponent"));
	HealthBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarComponent"));

	//StaticMeshCharacter = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshCharacter"));
	//StaticMeshCharacter->SetupAttachment(RootComponent);

	HealthBarComponent->SetupAttachment(RootComponent);

	GetCharacterMovement()->bUseRVOAvoidance = true;
	GetCharacterMovement()->AvoidanceConsiderationRadius = 200.0f;
	GetCharacterMovement()->SetRVOAvoidanceWeight(0.0f);

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

FGameplayTag ABaseUnitCharacter::GetTeamTag()
{
	UGameplayTagsManager& TagManager = UGameplayTagsManager::Get();
	FGameplayTag TeamTag = TagManager.RequestGameplayTagDirectParent(UnitData.Info.InfoSetting.Tag);
	if (TeamTag.IsValid())
	{
		return TeamTag;
	}

	return FGameplayTag::EmptyTag;
};

void ABaseUnitCharacter::BeginPlay()
{
	Super::BeginPlay();

	DataInitialize();
	if (UUserWidget* TempHealthWidgetUser = Cast<UUserWidget>(HealthBarComponent->GetWidget()))
	{
		if (UCharacterHealthWidget* TempHealthWidget = Cast<UCharacterHealthWidget>(TempHealthWidgetUser))
		{
			CharacterBattleStatsComponent->OnCharacterRegenStatChanged.AddUObject(TempHealthWidget, &UCharacterHealthWidget::SetPercent);
			float CurrentHealth = 0.0f;
			float MaxHealth = 0.0f;

			//Arcanum.BattleStat.Character.Regen.Health
			const TArray<FRegenStat>& RegenStats = CharacterBattleStatsComponent->GetRegenStats();
			for (int i = 0; i < RegenStats.Num(); i++)
			{
				if (RegenStats[i].ParentTag == Arcanum::BattleStat::Character::Regen::Health::Root)
				{
					CurrentHealth = RegenStats[i].Current;
					MaxHealth = RegenStats[i].GetTotalMax();
					break;
				}
			}
			TempHealthWidget->SetPercent(CurrentHealth, MaxHealth);
		}
	}
	OnTakeAnyDamage.AddDynamic(this, &ABaseUnitCharacter::RecievedDamage);

#pragma region Debug
	if (RandomRvoWeight)
	{
		GetCharacterMovement()->SetRVOAvoidanceWeight(FMath::FRandRange(0.0f, 1.0f));
	}
#pragma endregion

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

float ABaseUnitCharacter::GetAttackPower()
{
	float AttackPower = 0.0f;
	const TArray<FNonRegenStat>& NonRegenStats = CharacterBattleStatsComponent->GetNonRegenStats();
	for (int i = 0; i < NonRegenStats.Num(); i++)
	{
		if (NonRegenStats[i].TagName == Arcanum::BattleStat::Character::NonRegen::AttackPower::Root)
		{
			AttackPower = NonRegenStats[i].GetTotalValue();
			break;
		}
	}
	return AttackPower;
}

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
	UnitCombatComponent->SendDamage(GetAttackPower());
}

// 데미지 받기
void ABaseUnitCharacter::RecievedDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	GetCharacterBattleStatsComponent()->ChangeStatValue(Arcanum::BattleStat::Character::Regen::Health::Root, -(FMath::Abs(Damage)), DamageCauser);
}

const FUnitData& ABaseUnitCharacter::GetUnitData()
{
	return UnitData;
}

