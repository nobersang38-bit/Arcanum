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
#include "Data/Rows/UnitsDataRow.h"
#include "Component/Stats/CharacterBattleStatsComponent.h"
#include "Components/WidgetComponent.h"
//#include "UI/CharacterHUD/CharacterHealthWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "GameplayTagsManager.h"
#include "Object/Actor/BattlefieldManagerActor.h"
#include "Animation/BaseUnitAnimInstance.h"
#include "UI/InGame/UnitHealthWidget.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
#include "Component/StatusActionComponent.h"

// Sets default values
ABaseUnitCharacter::ABaseUnitCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	UnitCombatComponent = CreateDefaultSubobject<UUnitCombatComponent>(TEXT("UnitCombatComponent"));
	CharacterBattleStatsComponent = CreateDefaultSubobject<UCharacterBattleStatsComponent>(TEXT("CharacterBattleStatsComponent"));
	HealthBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarComponent"));
	StatusActionComponent = CreateDefaultSubobject<UStatusActionComponent>(TEXT("StatusActionComponent"));

	HealthBarComponent->SetupAttachment(RootComponent);

	GetCharacterMovement()->bUseRVOAvoidance = true;
	GetCharacterMovement()->AvoidanceConsiderationRadius = 200.0f;
	GetCharacterMovement()->SetRVOAvoidanceWeight(0.0f);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ABaseUnitCharacter::SetUnit(FUnitInfoSetting InUnitData)
{
	UnitData.Info.InfoSetting = InUnitData;
	IsSetupUnit = true;
	DataInitialize();
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
	OutlineDynamicMI = UMaterialInstanceDynamic::Create(GetMesh()->GetOverlayMaterial(), this);
	GetMesh()->SetOverlayMaterial(OutlineDynamicMI);
	DataInitialize();
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

	if (MemberPropertyName == GET_MEMBER_NAME_CHECKED(ABaseUnitCharacter, UnitData))
	{
		if (USkeletalMesh* SkelMesh = UnitData.Info.AnimSetting.SkeletalMesh.LoadSynchronous())
		{
			GetMesh()->SetSkeletalMesh(SkelMesh);
		}
		//UpdateUnitData();
	}
}
#endif

void ABaseUnitCharacter::AnimSetting()
{
	switch (UnitData.Info.AnimSetting.AnimMode)
	{
	case EAnimMode::AnimBlueprint:
		if (!UnitData.Info.AnimSetting.SkeletalMesh.IsNull())// 스켈레탈 메시 설정
		{
			if (USkeletalMesh* SkeletalMesh = UnitData.Info.AnimSetting.SkeletalMesh.LoadSynchronous())
			{
				GetMesh()->SetSkeletalMesh(SkeletalMesh);
			}
		}
		if (UnitData.Info.AnimSetting.AnimInstance) // 애님 인스턴스 설정
		{
			GetMesh()->SetAnimInstanceClass(UnitData.Info.AnimSetting.AnimInstance);
			if (UBaseUnitAnimInstance* TemplateAnimInstance = Cast<UBaseUnitAnimInstance>(GetMesh()->GetAnimInstance()))
			{
				TemplateAnimInstance->SetAnimations(UnitData.Info.AnimSetting.Idle, UnitData.Info.AnimSetting.Run);
			}
		}
		break;
	case EAnimMode::AnimToTexture:
		GetMesh()->SetHiddenInGame(true);
		GetMesh()->Deactivate();
		if (ABattlefieldManagerActor* BattlefieldManagerActor = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>()->GetBattlefieldManagerActor())
		{
			BattlefieldManagerActor->SpawnStaticMesh(UnitData.Info.InfoSetting.Tag, this);
		}
		break;

	default:
		break;
	}
}

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
	GetCharacterMovement()->SetRVOAvoidanceWeight((FMath::Rand32() % 11) * 0.1f);
	CharacterBattleStatsComponent->InitComponent();
	if (SetPercentDelegateHandle.IsValid())
	{
		CharacterBattleStatsComponent->OnCharacterRegenStatChanged.Remove(SetPercentDelegateHandle);
	}
	// Todo KDH : 임시
	//CharacterBattleStatsComponent->ChangeStatValue(Arcanum::BattleStat::Character::Regen::Health::Root, 100.0f, this);
	if (!IsSetupUnit)
	{
		UpdateUnitData();
	}
	AnimSetting();

	if (UUserWidget* TempHealthWidgetUser = Cast<UUserWidget>(HealthBarComponent->GetWidget()))
	{
		if (UUnitHealthWidget* TempHealthWidget = Cast<UUnitHealthWidget>(TempHealthWidgetUser))
		{
			SetPercentDelegateHandle = CharacterBattleStatsComponent->OnCharacterRegenStatChanged.AddUObject(TempHealthWidget, &UUnitHealthWidget::SetPercent);
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
			TempHealthWidget->SetPercentFloat(CurrentHealth, MaxHealth);
		}
	}
	OnTakeAnyDamage.RemoveDynamic(this, &ABaseUnitCharacter::RecievedDamage);
	OnTakeAnyDamage.AddDynamic(this, &ABaseUnitCharacter::RecievedDamage);

#pragma region Debug
	if (RandomRvoWeight)
	{
		GetCharacterMovement()->SetRVOAvoidanceWeight(FMath::FRandRange(0.0f, 1.0f));
	}
#pragma endregion
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
	UnitCombatComponent->LightHitReaction(Damage);
	OuntLineStart(OutLineCurve, OutLineTime, 0.005f, OutlineTimeHandle, OutlineDynamicMI, RefOutlineTime);
}

void ABaseUnitCharacter::UpdateUnitData()
{
	if (DTUnitDataRowHandle.DataTable && !DTUnitDataRowHandle.RowName.IsNone())
	{
		if (const FUnitsDataRow* AllyRow = DTUnitDataRowHandle.DataTable->FindRow<FUnitsDataRow>(DTUnitDataRowHandle.RowName, TEXT("Load")))
		{
			if (AllyRow) UnitData.Info.InfoSetting = (*AllyRow).UnitData;
		}
		else if (const FUnitsDataRow* EnemyRow = DTUnitDataRowHandle.DataTable->FindRow<FUnitsDataRow>(DTUnitDataRowHandle.RowName, TEXT("Load")))
		{
			if (EnemyRow) UnitData.Info.InfoSetting = (*EnemyRow).UnitData;
		}
	}
}

const FUnitData& ABaseUnitCharacter::GetUnitData()
{
	return UnitData;
}

bool ABaseUnitCharacter::GetIsDead()
{
	if (UnitCombatComponent)
	{
		return UnitCombatComponent->GetIsDead();
	}
	return true;
}

void ABaseUnitCharacter::UnitActivate()
{
	DataInitialize();
	if (UnitCombatComponent)
	{
		UnitCombatComponent->UnitActivate();
	}
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseUnitCharacter::UnitDeactive()
{
	if (UnitCombatComponent)
	{
		UnitCombatComponent->UnitDeactive();
	}
}

void ABaseUnitCharacter::OuntLineStart(const UCurveFloat* CurveFloat, float InTime, float DeltaTime, FTimerHandle& InTimerHandle, UMaterialInstanceDynamic* MaterialInstance, float& RefTime)
{
	RefTime = 0.0f;
	FTimerDelegate OutlineDelegate;
	OutlineDelegate.BindWeakLambda(this, [this, MaterialInstance, CurveFloat, InTime, DeltaTime, &InTimerHandle, &RefTime]()
		{
			if (MaterialInstance)
			{
				float Time = FMath::Clamp(RefTime / InTime, 0.0f, 1.0f);
				float Value = CurveFloat->GetFloatValue(Time);
				MaterialInstance->SetScalarParameterValue(FName("Weight0"), Value);
				RefTime += DeltaTime;

				if (Time >= 1.0f)
				{
					MaterialInstance->SetScalarParameterValue(FName("Weight0"), 0.0f);
					GetWorld()->GetTimerManager().ClearTimer(InTimerHandle);
				}
			}
		});

	GetWorld()->GetTimerManager().ClearTimer(InTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(InTimerHandle, OutlineDelegate, DeltaTime, true);
}

void ABaseUnitCharacter::ActivateItem()
{
	UnitActivate();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	if (HealthBarComponent)
	{
		HealthBarComponent->SetHiddenInGame(false);
	}
}

void ABaseUnitCharacter::DeactiveItem()
{
	UnitDeactive();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetActorLocation(FVector(0.0f, 0.0f, -9999.0f));
	if (HealthBarComponent)
	{
		HealthBarComponent->SetHiddenInGame(true);
	}
}