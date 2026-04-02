#include "UI/Battle/BattlePlayerController.h"
#include "Blueprint/UserWidget.h"
#include "UI/Battle/Contents/InBattleHUDWidget.h"
#include "UI/Battle/SubLayout/BattleAllyUnitPanelWidget.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Core/SubSystem/BattlefieldManagerSubsystem.h"
#include "GameplayTags/ArcanumTags.h"
#include "Core/SubSystem/GameTimeSubsystem.h"
#include "Core/ARPlayerAccountService.h"
#include "UI/Battle/SubLayout/BattleAllyUnitSlotWidget.h"
#include "Character/BaseUnitCharacter.h"
#include "Core/SubSystem/PoolingSubsystem.h"
#include "Character/PlayerCharacter.h"
#include "UI/Battle/SubLayout/BattleBattleEndWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Object/Actor/SpawnCheckDecal.h"
#include "DataInfo/SkillData/Data/FSkillInfo.h"
#include "Object/Actor/SelectedArrow.h"
#include "Object/Skills/SkillActor.h"
#include "Object/Skills/SkillBase.h"
#include "AIController.h"
#include "Core/SubSystem/GameDataSubsystem.h"
#include "Object/Actor/SkillRangeDecal.h"
#include "Engine/PostProcessVolume.h"
#include "UI/Battle/SubLayout/BattleActionButtonWidget.h"
#include "Camera/CameraComponent.h"
#include "Component/Stats/CharacterBattleStatsComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Log/Logger.h"

// ========================================================
// 언리얼 기본 생성
// ========================================================
void ABattlePlayerController::BeginPlay()
{
	Super::BeginPlay();
	InitialSkillBase();
	UltimatePostProcessVolume = Cast<APostProcessVolume>(UGameplayStatics::GetActorOfClass(this, APostProcessVolume::StaticClass()));
	if (!SelectedArrowInstance && SelectedArrowClass)
	{
		FTransform Transform;
		Transform.SetLocation(FVector(0.0f, 0.0f, -9999.0f));
		SelectedArrowInstance = GetWorld()->SpawnActor<ASelectedArrow>(SelectedArrowClass, Transform);
	}

	if (!SkillRangeDecalInstance && SkillRangeDecalClass)
	{
		FTransform Transform;
		Transform.SetLocation(FVector(0.0f, 0.0f, -9999.0f));
		SkillRangeDecalInstance = GetWorld()->SpawnActor<ASkillRangeDecal>(SkillRangeDecalClass, Transform);
		SkillRangeDecalInstance->SetOwner(GetCharacter());
	}

	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UInBattleHUDWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();

			if (HUDWidgetInstance->GetBattleEndWidget())
			{
				HUDWidgetInstance->GetBattleEndWidget()->OnInBattleLobbyButtonClick.AddDynamic(this, &ABattlePlayerController::OpenLobbyLevel);
			}
		}
	}

	SetupMainHUDWidget();
	SetupInputMode();
	RefreshSkillCooldown();
	RefreshBattlePotion();
	BindBuffUI();

	UBattlefieldManagerSubsystem* BattleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
	if (!BattleSubsystem) return;

	if (BattleSubsystem)
	{
		const TArray<FRegenStat>& PlayerBattleRegenStat = BattleSubsystem->GetInBattleData().PlayerBattleData.PlayerBattleRegenStat;
		for (const auto& PlayerBattleRegenStatMeat : PlayerBattleRegenStat)
		{
			if (MeatTag == PlayerBattleRegenStatMeat.ParentTag)
			{
				MeatValue.AddFrom(PlayerBattleRegenStatMeat);
				MeatValue.Current = PlayerBattleRegenStatMeat.BaseMax;
				break;
			}
		}
	}

	//if (BattleSubsystem)
	//{
	//	const TArray<FRegenStat>& PlayerBattleRegenStat = BattleSubsystem->GetInBattleData().PlayerBattleStat.RegenStats;
	//	for (const auto& PlayerBattleRegenStatMeat : PlayerBattleRegenStat)
	//	{
	//		if (ManaTag == PlayerBattleRegenStatMeat.ParentTag)
	//		{
	//			ManaValue.AddFrom(PlayerBattleRegenStatMeat);
	//			ManaValue.Current = PlayerBattleRegenStatMeat.BaseMax;
	//			break;
	//		}
	//	}
	//}

	float MeatTickInterval = 0.1f;
	FTimerDelegate MeatDelegate;
	MeatDelegate.BindUObject(this, &ABattlePlayerController::UpdateMeatValue, MeatTickInterval);
	GetWorld()->GetTimerManager().ClearTimer(MeatTimer);
	GetWorld()->GetTimerManager().SetTimer(MeatTimer, MeatDelegate, MeatTickInterval, true);

	float ManaTickInterval = 0.1f;
	FTimerDelegate ManaDelegate;
	ManaDelegate.BindUObject(this, &ABattlePlayerController::UpdateManaValue, ManaTickInterval);
	GetWorld()->GetTimerManager().ClearTimer(ManaTimer);
	GetWorld()->GetTimerManager().SetTimer(ManaTimer, ManaDelegate, ManaTickInterval, true);

	UGameTimeSubsystem* TimeSubsystem = GetGameInstance()->GetSubsystem<UGameTimeSubsystem>();

	if (BattleSubsystem)
	{
		UsingAllyUnits.Empty();
		for (auto Iter = BattleSubsystem->GetUsingAllyUnitData().begin(); Iter != BattleSubsystem->GetUsingAllyUnitData().end(); ++Iter)
		{
			const FUnitInfoSetting& UnitDataE = Iter->Value;
			UsingAllyUnits.Add(UnitDataE.Tag, UnitDataE);
		}

		SetAllyUsingWidget();

		BattleSubsystem->OnMatchEnded.AddUObject(this, &ABattlePlayerController::BattleEnd);
	}

	if (TimeSubsystem)
	{
		if (BattleSubsystem)
		{
			BattleSubsystem->StartTime();

			TimeSubsystem->OnStageSecondChanged.RemoveDynamic(this, &ABattlePlayerController::UpdateStageTime);
			TimeSubsystem->OnStageSecondChanged.AddDynamic(this, &ABattlePlayerController::UpdateStageTime);
		}
	}

	float CoolTImeTickInterval = 0.05f;
	FTimerDelegate CoolTimeDelegate;
	CoolTimeDelegate.BindUObject(this, &ABattlePlayerController::Internal_CoolTimeTick, CoolTImeTickInterval);
	GetWorld()->GetTimerManager().ClearTimer(CoolTimeTimer);
	GetWorld()->GetTimerManager().SetTimer(CoolTimeTimer, CoolTimeDelegate, CoolTImeTickInterval, true);

	FTimerDelegate BattlePotionCooldown;
	BattlePotionCooldown.BindUObject(this, &ABattlePlayerController::UpdateBattlePotionCooldown, BattlePotionCooldownTickInterval);
	GetWorld()->GetTimerManager().ClearTimer(BattlePotionCooldownTimer);
	GetWorld()->GetTimerManager().SetTimer(BattlePotionCooldownTimer, BattlePotionCooldown, BattlePotionCooldownTickInterval, true);

	GetWorld()->GetTimerManager().ClearTimer(PlayerLocationProgressTimeHandle);
	GetWorld()->GetTimerManager().SetTimer(PlayerLocationProgressTimeHandle, this, &ABattlePlayerController::UpdatePlayerLocationProgress, PlayerLocationProgressUpdateInterval, true, 0.0f);

	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (playerCharacter)
	{
		if (UCameraComponent* playerCamera = playerCharacter->GetCamera())
		{
			UltimateDefaultFOV = playerCamera->FieldOfView;
		}
	}
	if (UltimatePostProcessVolume)
	{
		UltimatePostProcessVolume->BlendWeight = 0.0f;
		UltimatePostProcessVolume->bUnbound = false;
	}

	if (HUDWidgetInstance)
	{
		BattleSubsystem->OnChangeAllyBaseHealth.AddDynamic(HUDWidgetInstance, &UInBattleHUDWidget::SetAllyBaseHealthBarProgress);
		BattleSubsystem->OnChangeEnemyBaseHealth.AddDynamic(HUDWidgetInstance, &UInBattleHUDWidget::SetEnemyBaseHealthBarProgress);
	}

}

void ABattlePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		SubSystem->AddMappingContext(InputMappingContext, 0);
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ABattlePlayerController::InputMove);
		//EnhancedInputComponent->BindAction(IA_BasicAttack, ETriggerEvent::Completed, this, &ABattlePlayerController::BasicAttack);
		//EnhancedInputComponent->BindAction(IA_BasicSkill, ETriggerEvent::Completed, this, &ABattlePlayerController::BasicSkill);
		EnhancedInputComponent->BindAction(IA_BasicAttackSkill, ETriggerEvent::Completed, this, &ABattlePlayerController::InputBasicAttack);
		EnhancedInputComponent->BindAction(IA_CommonSkill, ETriggerEvent::Completed, this, &ABattlePlayerController::InputSkill);
		EnhancedInputComponent->BindAction(IA_UltimateSkill, ETriggerEvent::Started, this, &ABattlePlayerController::UltimateSkillPressed);
		EnhancedInputComponent->BindAction(IA_UltimateSkill, ETriggerEvent::Completed, this, &ABattlePlayerController::UltimateSkillReleased);
		EnhancedInputComponent->BindAction(IA_Item1, ETriggerEvent::Completed, this, &ABattlePlayerController::Item1);
		EnhancedInputComponent->BindAction(IA_Item2, ETriggerEvent::Completed, this, &ABattlePlayerController::Item2);
		EnhancedInputComponent->BindAction(IA_WeaponSwap, ETriggerEvent::Completed, this, &ABattlePlayerController::WeaponSwap);
		EnhancedInputComponent->BindAction(IA_AutoManual, ETriggerEvent::Completed, this, &ABattlePlayerController::AutoManualModePC);
		EnhancedInputComponent->BindAction(IA_InputCancel, ETriggerEvent::Started, this, &ABattlePlayerController::SlotSelectCancel);
		EnhancedInputComponent->BindAction(IA_CommonButton, ETriggerEvent::Started, this, &ABattlePlayerController::CommonButton);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ABattlePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SelectedUnit.IsValid())
	{
		FHitResult HitResult;
		GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
		FVector ResultLocation = HitResult.ImpactPoint;
		float CapsuleHeight = SelectedUnit->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 0.5f;
		ResultLocation = ResultLocation + (FVector::UpVector * CapsuleHeight);
		SelectedUnit->SetActorLocation(ResultLocation);

		FVector Direction = FVector::RightVector;
		SelectedUnit->SetActorRotation(Direction.Rotation());

		UBattlefieldManagerSubsystem* BattleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
		if (BattleSubsystem)
		{
			if (BattleSubsystem->GetSpawnCheckDecal())
			{
				bool bIsInside = false;
				FVector ResultLocation2 = BattleSubsystem->GetSpawnCheckDecal()->CalculateMinDistanceLocation(ResultLocation, bIsInside);
				ResultLocation2 += FVector::UpVector * 100.0f;

				//ResultLocation = 스폰되어야할 최종 위치
				if (bIsInside)
				{
					// 마우스가 안에 있을때의 로직

					if (SelectedUnit2.IsValid())
					{
						SelectedUnit2->SetActorHiddenInGame(true);
						SelectedArrowInstance->SetActive(false);
					}
				}
				else
				{
					if (SelectedUnit2.IsValid())
					{
						SelectedUnit2->SetActorHiddenInGame(false);
					}
					else
					{
						// 내부에 액터가 없으면 생성
						if (FUnitInfoSetting* UnitData = UsingAllyUnits.Find(SpawnTag))
						{
							UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>();
							if (PoolingSubsystem)
							{

								FHitResult HitResult2;
								GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult2);
								FTransform Transform;
								Transform.SetLocation(HitResult2.ImpactPoint);
								Transform.SetRotation(Direction.Rotation().Quaternion());
								// Todo KDH : 미리 로드해 놓아야함
								AActor* EnemyUnitInstance = PoolingSubsystem->SpawnFromPool(UnitData->UnitClass.LoadSynchronous(), Transform);
								ABaseUnitCharacter* EnemyUnitCharacter = Cast<ABaseUnitCharacter>(EnemyUnitInstance);
								SetSpawnDecalActive(true);
								if (EnemyUnitCharacter)
								{
									EnemyUnitCharacter->SetUnit(*UnitData, true);
									SelectedUnit2 = EnemyUnitCharacter;
								}
							}
						}
					}

					if (SelectedUnit2.IsValid())
					{
						FHitResult HitResult3;
						GetWorld()->LineTraceSingleByChannel(HitResult3, ResultLocation2, ResultLocation2 + FVector::UpVector * -1000.0f, ECollisionChannel::ECC_WorldStatic);
						float Unit2Height = SelectedUnit2->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 0.5f;
						FVector ResultUnit2Location = HitResult3.ImpactPoint + FVector::UpVector * Unit2Height;
						SelectedUnit2->SetActorLocation(ResultUnit2Location);

						SelectedArrowInstance->SetArrow(ResultLocation, ResultUnit2Location);
						SelectedArrowInstance->SetActive(true);
					}
				}
			}
		}
	}

	if (CurrentSelectedSkillBase.IsValid() && SkillRangeDecalInstance && !bIsUltimateAiming)
	{
		FHitResult HitResult;
		GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
		FVector SkillLocationBack = SkillRangeDecalInstance->SetCursorLocation(HitResult.ImpactPoint);

		SkillLocation = FVector(SkillLocationBack.X, SkillLocationBack.Y, GetPawn()->GetActorLocation().Z + 100.0f);
	}

	UpdateUltimatePresentation(DeltaTime);
}

// ========================================================
// 디버그(콘솔 호출 후, 함수 이름으로 실행가능)
// ========================================================
#pragma region 메인HUD
void ABattlePlayerController::DebugPlayPlayerCharacterHealthBar(float CurrentHealth, float MaxHealth)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("DebugPlayPlayerCharacterHealthBar"));
	HUDWidgetInstance->SetPlayerCharacterHealthBarProgress(CurrentHealth, MaxHealth);
}

//void ABattlePlayerController::DebugBossHealthBar(float CurrentHealth, float MaxHealth)
//{
//	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("DebugBossHealthBar"));
//	HUDWidgetInstance->SetBossHealthBarProgress(CurrentHealth, MaxHealth);
//}

void ABattlePlayerController::DebugAddPlayerInfoPanelSlot()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("DebugAddPlayerInfoPanelSlot"));
	//HUDWidgetInstance->GetPlayerInfoPanel()->AddUnitSlot();
}

void ABattlePlayerController::DebugRemovePlayerInfoPanelSlot(int32 RemoveIDX)
{
	bool Result = HUDWidgetInstance->GetPlayerInfoPanel()->RemoveUnitSlot(RemoveIDX);
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("DebugRemovePlayerInfoPanelSlot %d"), static_cast<int32>(Result));
}
#pragma endregion

//bool ABattlePlayerController::SkillStarter(FGameplayTag InSkillTag, int32 InLevel, bool bIsUltimate)
//{
//	SkillCancel();
//	InLevel++;
//	TSubclassOf<class ASkillActor> SkillActorClass;
//	FString SkillName;
//	USkillBase* SkillBaseBack = nullptr;
//
//	if (!SkillBaseInstances.Contains(InSkillTag))
//	{
//		SkillBaseBack = NewObject<USkillBase>(this);
//		SkillBaseInstances.Add(InSkillTag, SkillBaseBack);
//		UGameDataSubsystem* DataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGameDataSubsystem>();
//		if (DataSubsystem)
//		{
//			SkillName = InSkillTag.ToString();
//			int32 LastDot;
//			if (SkillName.FindLastChar('.', LastDot))
//			{
//				// 찾은 인덱스 다음(+1)부터 끝까지 남기고 앞은 다 자릅니다.
//				SkillName = SkillName.RightChop(LastDot + 1);
//			}
//
//			FDTSkillsDataRow* DTSkillsDataRow = DataSubsystem->GetRow<FDTSkillsDataRow>(Arcanum::DataTable::SkillData, FName(SkillName));
//
//			if (DTSkillsDataRow)
//			{
//				SkillBaseBack->Initialize(GetPawn(), &DTSkillsDataRow->SkillData, InLevel, DTSkillsDataRow->SkillData.TargetFilterTag);
//			}
//			else return false;
//		}
//	}
//	else
//	{
//		SkillBaseBack = *SkillBaseInstances.Find(InSkillTag);
//
//		SkillName = InSkillTag.ToString();
//		int32 LastDot;
//		if (SkillName.FindLastChar('.', LastDot))
//		{
//			// 찾은 인덱스 다음(+1)부터 끝까지 남기고 앞은 다 자릅니다.
//			SkillName = SkillName.RightChop(LastDot + 1);
//		}
//	}
//
//	UGameDataSubsystem* DataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGameDataSubsystem>();
//	if (DataSubsystem)
//	{
//		FDTSkillsDataRow* DTSkillsDataRow = DataSubsystem->GetRow<FDTSkillsDataRow>(Arcanum::DataTable::SkillData, FName(SkillName));
//		if (DTSkillsDataRow)
//		{
//			SkillActorClass = DTSkillsDataRow->SkillData.SkillClass.LoadSynchronous();
//		}
//	}
//
//	if (!UseSkillCost(InSkillTag)) return false;
//
//	if (SkillActorClass)
//	{
//		UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>();
//		if (PoolingSubsystem)
//		{
//			FTransform Transform;
//			Transform.SetLocation(GetPawn()->GetActorLocation());
//
//			FVector2D Direction2D = FVector2D(SkillLocation) - FVector2D(GetPawn()->GetActorLocation());
//			FRotator RotatorDirection = FVector(Direction2D.X, Direction2D.Y, 0.0f).Rotation();
//			Transform.SetRotation(RotatorDirection.Quaternion());
//			AActor* SkillActor = PoolingSubsystem->SpawnFromPool(SkillActorClass, Transform);
//			ASkillActor* SkillInstance = Cast<ASkillActor>(SkillActor);
//			if (SkillInstance)
//			{
//				FHitResult HitResult;
//				GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, HitResult);
//				if (HitResult.GetActor())
//				{
//					UE_LOG(LogTemp, Error, TEXT("%s"), *HitResult.GetActor()->GetName());
//					AActor* TargetActor = HitResult.GetActor()->GetClass()->ImplementsInterface(UTeamInterface::StaticClass()) ? HitResult.GetActor() : nullptr;
//					if (SkillTargetActor.IsValid())
//					{
//						SkillInstance->SetTargetActor(SkillTargetActor.Get());
//					}
//					else
//					{
//						SkillInstance->SetTargetActor(TargetActor);
//					}
//					SkillInstance->SetTargetLocation(SkillLocation);
//					SkillInstance->ActivateSkillActor(SkillBaseBack, GetPawn(), Transform.GetLocation(), Transform.GetRotation().Rotator());
//					if (UAnimMontage* SkillMontage = SkillBaseBack->GetSkillInfo()->SkillMontage)
//					{
//						if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn()))
//						{
//							PlayerCharacter->GetSourceSkeletaMeshComponent()->GetAnimInstance()->Montage_Play(SkillMontage);
//						}
//					}
//					bIsSkillSuccess = true;
//					SkillTargetActor = nullptr;
//					return true;
//				}
//			}
//		}
//	}
//	return false;
//}

void ABattlePlayerController::ReadySkillSet(FGameplayTag InSkillTag, int32 InLevel, bool bIsUltimate)
{
	if (CurrentSelectedSkillBase.IsValid())
	{
		SkillCancel();
		return;
	}
	SkillCancel();
	FString SkillName;
	USkillBase* SkillBaseBack = nullptr;

	if (!SkillBaseInstances.Contains(InSkillTag))
	{
		SkillBaseBack = NewObject<USkillBase>(this);
		SkillBaseInstances.Add(InSkillTag, SkillBaseBack);
		UGameDataSubsystem* DataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGameDataSubsystem>();
		if (DataSubsystem)
		{
			SkillName = InSkillTag.ToString();
			int32 LastDot;
			if (SkillName.FindLastChar('.', LastDot))
			{
				// 찾은 인덱스 다음(+1)부터 끝까지 남기고 앞은 다 자릅니다.
				SkillName = SkillName.RightChop(LastDot + 1);
			}

			FDTSkillsDataRow* DTSkillsDataRow = DataSubsystem->GetRow<FDTSkillsDataRow>(Arcanum::DataTable::SkillData, FName(SkillName));

			if (DTSkillsDataRow)
			{
				SkillBaseBack->Initialize(GetPawn(), &DTSkillsDataRow->SkillData, InLevel, DTSkillsDataRow->SkillData.TargetFilterTag);
				CurrentSelectedSkillBase = SkillBaseBack;
			}
		}
	}
	else
	{
		SkillBaseBack = *SkillBaseInstances.Find(InSkillTag);
		CurrentSelectedSkillBase = SkillBaseBack;
	}

	if (CurrentSelectedSkillBase.IsValid())
	{
		if (!SkillCostChecker(CurrentSelectedSkillBase->GetSkillInfo().SkillNameTag, InLevel))
		{
			SkillCancel();
			return;
		}
	}

	if (SkillRangeDecalInstance && SkillBaseBack)
	{
		FHitResult HitResult;
		GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
		SkillRangeDecalInstance->SkillRangeDecalOn(SkillBaseBack->GetSkillInfo().EnabledRange, GetPawn(), HitResult.ImpactPoint);
	}

	if (bIsAutoManual)
	{
		CommonButton();
	}
}

//void ABattlePlayerController::CurrentSelectedSkillStarter()
//{
//	if (!CurrentSelectedSkillBase.IsValid()) return;
//
//	if (const FSkillInfo* SkillInfo = CurrentSelectedSkillBase->GetSkillInfo())
//	{
//		SkillStarter(SkillInfo->SkillNameTag, CurrentSelectedSkillBase->GetCurrentLevelEntry()->Level - 1);
//	}
//	SkillCancel();
//}

void ABattlePlayerController::SkillCancel()
{
	bIsSkillSuccess = false;
	CurrentSelectedSkillBase = nullptr;
	SkillRangeDecalInstance->SkillRangeDecalOff();
	UBattlefieldManagerSubsystem* battleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
	if (battleSubsystem)
	{
		if (battleSubsystem->IsLengendaryWeapon())
		{
			UltimateSkillEnd();
		}
	}
}

bool ABattlePlayerController::SkillCostChecker(FGameplayTag InSkillTag, int32 InLevel, bool bIsOnlyManaCheck)
{
	TSubclassOf<class ASkillActor> skillActorClass;
	FString skillName;
	USkillBase* skillBaseBack = nullptr;

	if (!SkillBaseInstances.Contains(InSkillTag))
	{
		skillBaseBack = NewObject<USkillBase>(this);
		SkillBaseInstances.Add(InSkillTag, skillBaseBack);

		UGameDataSubsystem* dataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGameDataSubsystem>();
		if (!dataSubsystem)	return false;

		skillName = InSkillTag.ToString();
		int32 lastDot;
		if (skillName.FindLastChar('.', lastDot))
		{
			skillName = skillName.RightChop(lastDot + 1);
		}

		FDTSkillsDataRow* dtSkillsDataRow = dataSubsystem->GetRow<FDTSkillsDataRow>(Arcanum::DataTable::SkillData, FName(skillName));
		if (!dtSkillsDataRow) return false;

		skillBaseBack->Initialize(GetPawn(), &dtSkillsDataRow->SkillData, InLevel, dtSkillsDataRow->SkillData.TargetFilterTag);
	}
	else
	{
		skillBaseBack = *SkillBaseInstances.Find(InSkillTag);
		skillName = InSkillTag.ToString();

		int32 lastDot;
		if (skillName.FindLastChar('.', lastDot))
		{
			skillName = skillName.RightChop(lastDot + 1);
		}
	}

	if (!skillBaseBack) return false;
	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (!playerCharacter) return false;
	UCharacterBattleStatsComponent* statComponent = playerCharacter->FindComponentByClass<UCharacterBattleStatsComponent>();
	if (!statComponent) return false;
	const FRegenStat* manaStat = statComponent->FindRegenStat(ManaTag);
	if (!manaStat) return false;

	float manaCost = 0.0f;
	float meatCost = 0.0f;

	const FLevelModifierEntry* levelModifier = skillBaseBack->GetCurrentLevelEntry();
	if (!levelModifier) return false;

	float* currentCoolTime = SkillCoolTimes.Find(InSkillTag);
	if (!currentCoolTime)
	{
		currentCoolTime = &SkillCoolTimes.Add(InSkillTag);
		*currentCoolTime = 0.0f;

		if (!bIsOnlyManaCheck)
		{
			return true;
		}
	}

	for (const auto& cost : levelModifier->Cost)
	{
		if (cost.StatTag == ManaValueTag)
		{
			manaCost += cost.Value.Flat;
		}
		else if (cost.StatTag == MeatTag)
		{
			meatCost += cost.Value.Flat;
		}
	}

	const float currentMana = manaStat->Current;
	const bool bHasEnoughMana = currentMana >= FMath::Abs(manaCost);
	const bool bHasEnoughMeat = MeatValue.Current >= FMath::Abs(meatCost);

	if (bIsOnlyManaCheck)
	{
		return bHasEnoughMana && bHasEnoughMeat;
	}

	return (*currentCoolTime <= 0.0f) && bHasEnoughMana && bHasEnoughMeat;


	//TSubclassOf<class ASkillActor> SkillActorClass;
	//FString SkillName;
	//USkillBase* SkillBaseBack = nullptr;

	//if (!SkillBaseInstances.Contains(InSkillTag))
	//{
	//	SkillBaseBack = NewObject<USkillBase>(this);
	//	SkillBaseInstances.Add(InSkillTag, SkillBaseBack);
	//	UGameDataSubsystem* DataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGameDataSubsystem>();
	//	if (DataSubsystem)
	//	{
	//		SkillName = InSkillTag.ToString();
	//		int32 LastDot;
	//		if (SkillName.FindLastChar('.', LastDot))
	//		{
	//			// 찾은 인덱스 다음(+1)부터 끝까지 남기고 앞은 다 자릅니다.
	//			SkillName = SkillName.RightChop(LastDot + 1);
	//		}

	//		FDTSkillsDataRow* DTSkillsDataRow = DataSubsystem->GetRow<FDTSkillsDataRow>(Arcanum::DataTable::SkillData, FName(SkillName));

	//		if (DTSkillsDataRow)
	//		{
	//			SkillBaseBack->Initialize(GetPawn(), &DTSkillsDataRow->SkillData, InLevel, DTSkillsDataRow->SkillData.TargetFilterTag);
	//		}
	//		else return false;
	//	}
	//}
	//else
	//{
	//	SkillBaseBack = *SkillBaseInstances.Find(InSkillTag);

	//	SkillName = InSkillTag.ToString();
	//	int32 LastDot;
	//	if (SkillName.FindLastChar('.', LastDot))
	//	{
	//		// 찾은 인덱스 다음(+1)부터 끝까지 남기고 앞은 다 자릅니다.
	//		SkillName = SkillName.RightChop(LastDot + 1);
	//	}
	//}

	//float MaxCoolTime = 0.0f;

	//float ManaCost = 0.0f;
	//float MeatCost = 0.0f;
	//if (SkillBaseBack)
	//{
	//	if (const FLevelModifierEntry* LevelModifier = SkillBaseBack->GetCurrentLevelEntry())
	//	{
	//		MaxCoolTime = LevelModifier->Cooldown;
	//		float* CurrentCoolTime = SkillCoolTimes.Find(InSkillTag);
	//		if (!CurrentCoolTime)
	//		{
	//			CurrentCoolTime = &SkillCoolTimes.Add(InSkillTag);
	//			*CurrentCoolTime = 0.0f;
	//			if (!bIsOnlyManaCheck) return true;
	//		}

	//		if (CurrentCoolTime)
	//		{
	//			for (const auto& Cost : LevelModifier->Cost) // 코스트 체크
	//			{
	//				if (Cost.StatTag == ManaValueTag)
	//				{
	//					ManaCost += Cost.Value.Flat;
	//				}
	//				else if (Cost.StatTag == MeatTag)
	//				{
	//					MeatCost += Cost.Value.Flat;
	//				}
	//			}

	//			float TempCoolTime = *CurrentCoolTime;
	//			if (!bIsOnlyManaCheck)
	//			{
	//				if (*CurrentCoolTime <= 0.0f && ManaValue.Current >= FMath::Abs(ManaCost) && MeatValue.Current >= FMath::Abs(MeatCost))
	//				{
	//					return true;
	//				}
	//				else
	//				{
	//					return false;
	//				}
	//			}
	//			else
	//			{
	//				if (ManaValue.Current >= FMath::Abs(ManaCost) && MeatValue.Current >= FMath::Abs(MeatCost))
	//				{
	//					return true;
	//				}
	//				else
	//				{
	//					return false;
	//				}
	//			}

	//		}
	//	}
	//}

	//return false;
}

// ========================================================
// 메인HUD
// ========================================================
void ABattlePlayerController::SetupMainHUDWidget()
{
	//HUDWidgetInstance->OnClickBasicAttack.AddDynamic(this, &ABattlePlayerController::BasicAttack);
	//HUDWidgetInstance->OnClickBasicSkill.AddDynamic(this, &ABattlePlayerController::BasicSkill);
	HUDWidgetInstance->OnClickBasicAttack.AddDynamic(this, &ABattlePlayerController::InputBasicAttack);
	HUDWidgetInstance->OnClickBasicSkill.AddDynamic(this, &ABattlePlayerController::InputSkill);
	HUDWidgetInstance->OnPressedUltimateSkill.AddDynamic(this, &ABattlePlayerController::UltimateSkillPressed);
	HUDWidgetInstance->OnReleasedUltimateSkill.AddDynamic(this, &ABattlePlayerController::UltimateSkillReleased);

	HUDWidgetInstance->OnClickWeaponSwap.AddDynamic(this, &ABattlePlayerController::WeaponSwap);
	HUDWidgetInstance->OnClickItem1.AddDynamic(this, &ABattlePlayerController::Item1);
	HUDWidgetInstance->OnClickItem2.AddDynamic(this, &ABattlePlayerController::Item2);
	HUDWidgetInstance->OnToggleAutoManualMode.AddDynamic(this, &ABattlePlayerController::AutoManualModeMobile);

	UBattlefieldManagerSubsystem* battleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
	if (battleSubsystem && HUDWidgetInstance)
	{
		HUDWidgetInstance->RefreshWeaponSkillIcons(
			battleSubsystem->GetCurrentWeaponIcon(),
			battleSubsystem->GetCurrentBasicSkillIcon(),
			battleSubsystem->GetLegendaryWeaponIcon());

		RefreshSkillCost();
	}
}

void ABattlePlayerController::UpdatePlayerLocationProgress()
{
	UBattlefieldManagerSubsystem* BattleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
	if (!AllyBasement.IsValid() && BattleSubsystem)
	{
		AllyBasement = BattleSubsystem->GetAllyBasement();
	}

	if (!EnemyBasement.IsValid() && BattleSubsystem)
	{
		EnemyBasement = BattleSubsystem->GetEnemyBasement();
	}

	if (!AllyBasement.IsValid() && !EnemyBasement.IsValid()) return;

	HUDWidgetInstance->SetPlayerLocationProgress(AllyBasement->GetActorLocation(), EnemyBasement->GetActorLocation(), GetPawn()->GetActorLocation());
}

void ABattlePlayerController::UpdateStageTime(int32 TimeSecond)
{
	StageTimeSecond = TimeSecond;
	HUDWidgetInstance->SetTime(StageTimeSecond);
}

void ABattlePlayerController::SetAllyUsingWidget()
{
	UBattleAllyUnitPanelWidget* PlayerInfoPanel = HUDWidgetInstance->GetPlayerInfoPanel();
	for (auto UsingAllyUnit : UsingAllyUnits)
	{
		UBattleAllyUnitSlotWidget* UnitSlot = PlayerInfoPanel->AddUnitSlot(UsingAllyUnit.Value);
		if (UnitSlot)
		{
			UnitSlot->SetUnitInfo(UsingAllyUnit.Value.MeatCost, UsingAllyUnit.Value.Icon, UsingAllyUnit.Value.Tag);
			UnitSlot->SetActivateCost(false);
			UnitSlot->SetCoolTimeProgress(0.0f, 0.0f);
			UnitSlot->OnClickUnitSlot.AddDynamic(this, &ABattlePlayerController::ReadySpawnUnit);
			UsingAllyUnitSlots.Add(UsingAllyUnit.Key, UnitSlot);
		}
	}
}

void ABattlePlayerController::UpdateMeatValue(float DeltaTime)
{
	if (UBattleAllyUnitPanelWidget* PlayerInfoPanel = HUDWidgetInstance->GetPlayerInfoPanel())
	{
		FRegenStat& InData = MeatValue;
		InData.Current += DeltaTime * InData.BaseTick;
		InData.Current = FMath::Clamp(InData.Current, 0.0f, InData.BaseMax);
		PlayerInfoPanel->SetMeatCostProgress(InData.Current, InData.BaseMax);

		for (auto Iter : UsingAllyUnits)
		{
			if (UBattleAllyUnitSlotWidget** Slot = UsingAllyUnitSlots.Find(Iter.Key))
			{
				if (Iter.Value.MeatCost <= MeatValue.Current)
				{
					(*Slot)->SetActivateCost(false);
				}
				else
				{
					(*Slot)->SetActivateCost(true);
				}
			}
		}
	}
}

void ABattlePlayerController::UpdateManaValue(float DeltaTime)
{
	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (!playerCharacter) return;

	UCharacterBattleStatsComponent* statComponent = playerCharacter->FindComponentByClass<UCharacterBattleStatsComponent>();
	if (!statComponent) return;

	const FRegenStat* manaStat = statComponent->FindRegenStat(ManaTag);
	if (!manaStat) return;

	UBattlefieldManagerSubsystem* BattleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
	if (!BattleSubsystem) return;

	if (UBattleAllyUnitPanelWidget* PlayerInfoPanel = HUDWidgetInstance->GetPlayerInfoPanel())
	{
		PlayerInfoPanel->SetManaCostProgress(manaStat->Current, manaStat->GetTotalMax());
	}

	for (auto& SkillBase : SkillBaseInstances)
	{
		if (SkillBase.Key == BattleSubsystem->GetCurrentBasicAttackSkillTag())
		{
			bool Result = SkillCostChecker(SkillBase.Key, BattleSubsystem->GetCurrentBasicAttackSkillLevel(), true);
			HUDWidgetInstance->GetBasicAttack()->SetActivateCost(!Result);
		}
		else if (SkillBase.Key == BattleSubsystem->GetCurrentBasicSkillTag())
		{
			bool Result = SkillCostChecker(SkillBase.Key, BattleSubsystem->GetCurrentBasicSkillLevel(), true);
			HUDWidgetInstance->GetBasicSkill()->SetActivateCost(!Result);
		}
		else if (SkillBase.Key == BattleSubsystem->GetLegendaryUltimateSkillTag())
		{
			bool Result = SkillCostChecker(SkillBase.Key, BattleSubsystem->GetLegendaryUltimateSkillLevel(), true);
			HUDWidgetInstance->GetUltimateSkill()->SetActivateCost(!Result);
		}
	}

	//UBattlefieldManagerSubsystem* BattleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
	//if (UBattleAllyUnitPanelWidget* PlayerInfoPanel = HUDWidgetInstance->GetPlayerInfoPanel())
	//{
	//	FRegenStat& InData = ManaValue;
	//	InData.Current += DeltaTime * InData.BaseTick;
	//	InData.Current = FMath::Clamp(InData.Current, 0.0f, InData.BaseMax);
	//	PlayerInfoPanel->SetManaCostProgress(InData.Current, InData.BaseMax);
	//}

	//for (auto& SkillBase : SkillBaseInstances)
	//{
	//	if (SkillBase.Key == BattleSubsystem->GetCurrentBasicAttackSkillTag())
	//	{
	//		bool Result = SkillCostChecker(SkillBase.Key, BattleSubsystem->GetCurrentBasicAttackSkillLevel(), true);
	//		HUDWidgetInstance->GetBasicAttack()->SetActivateCost(!Result);
	//	}
	//	else if (SkillBase.Key == BattleSubsystem->GetCurrentBasicSkillTag())
	//	{
	//		bool Result = SkillCostChecker(SkillBase.Key, BattleSubsystem->GetCurrentBasicSkillLevel(), true);
	//		HUDWidgetInstance->GetBasicSkill()->SetActivateCost(!Result);
	//	}
	//	else if (SkillBase.Key == BattleSubsystem->GetLegendaryUltimateSkillTag())
	//	{
	//		bool Result = SkillCostChecker(SkillBase.Key, BattleSubsystem->GetLegendaryUltimateSkillLevel(), true);
	//		HUDWidgetInstance->GetUltimateSkill()->SetActivateCost(!Result);
	//	}
	//}
}

void ABattlePlayerController::SetPlayerHealthProgress(float CurrentHealth, float MaxHealth)
{
	HUDWidgetInstance->SetPlayerCharacterHealthBarProgress(CurrentHealth, MaxHealth);
}

// ========================================================
// 메인
// ========================================================
//void ABattlePlayerController::BasicAttack()
//{
//	if (UBattlefieldManagerSubsystem* battleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>())
//	{
//		const FGameplayTag skillTag = battleSubsystem->GetCurrentBasicAttackSkillTag();
//		const int32 skillLevel = battleSubsystem->GetCurrentBasicAttackSkillLevel();
//
//		UE_LOG(LogTemp, Warning, TEXT("BasicAttack Tag=%s Level=%d"), *skillTag.ToString(), skillLevel);
//		ReadySkillSet(skillTag, skillLevel);
//	}
//}
//
//void ABattlePlayerController::BasicSkill()
//{
//	if (UBattlefieldManagerSubsystem* battleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>())
//	{
//		const FGameplayTag skillTag = battleSubsystem->GetCurrentBasicSkillTag();
//		const int32 skillLevel = battleSubsystem->GetCurrentBasicSkillLevel();
//
//		UE_LOG(LogTemp, Warning, TEXT("BasicSkill Tag=%s Level=%d"), *skillTag.ToString(), skillLevel);
//		ReadySkillSet(skillTag, skillLevel);
//		//SkillStarter(skillTag, skillLevel);
//	}
//
//	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("BasicSkill"));
//}

void ABattlePlayerController::WeaponSwap()
{
	if (!TryExecuteBattleAction(EBattleActionType::WeaponSwap)) return;
	SkillCancel();

	if (UBattlefieldManagerSubsystem* battleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>())
	{
		const FGameplayTag currentSlotTag = battleSubsystem->GetCurrentWeaponSlotTag();
		bIsWeaponSwapping = true;

		if (currentSlotTag == Arcanum::Items::ItemSlot::Weapon::Slot1)
		{
			battleSubsystem->SetCurrentWeaponSlotTag(Arcanum::Items::ItemSlot::Weapon::Slot2);
		}
		else if (currentSlotTag == Arcanum::Items::ItemSlot::Weapon::Slot2)
		{
			battleSubsystem->SetCurrentWeaponSlotTag(Arcanum::Items::ItemSlot::Weapon::Slot1);
		}

		APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(GetPawn());
		if (playerCharacter)
		{
			playerCharacter->UpdateEquippedWeaponMesh();

			if (UAnimMontage* equipMontage = battleSubsystem->GetCurrentWeaponEquipMontage())
			{
				if (USkeletalMeshComponent* meshComp = playerCharacter->GetMesh())
				{
					if (UAnimInstance* animInstance = meshComp->GetAnimInstance())
					{
						animInstance->Montage_Play(equipMontage);

						FOnMontageEnded montageEndedDelegate;
						montageEndedDelegate.BindUObject(this, &ABattlePlayerController::OnWeaponSwapMontageEnded);
						animInstance->Montage_SetEndDelegate(montageEndedDelegate, equipMontage);
					}
				}
			}
			else
			{
				bIsWeaponSwapping = false;
			}
		}
	}

	UBattlefieldManagerSubsystem* battleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
	if (battleSubsystem && HUDWidgetInstance)
	{
		HUDWidgetInstance->RefreshWeaponSkillIcons(
			battleSubsystem->GetCurrentWeaponIcon(),
			battleSubsystem->GetCurrentBasicSkillIcon(),
			battleSubsystem->GetLegendaryWeaponIcon());

		RefreshSkillCost();

		if (UBattleActionButtonWidget* BasicSkillButton = HUDWidgetInstance->GetBasicSkill())
		{
			/*FGameplayTag Tag = battleSubsystem->GetCurrentBasicSkillTag();
			SkillBaseInstances.Find(Tag)
			BasicSkillButton->SetCostText()*/
		}
		if (UBattleActionButtonWidget* UltimateButton = HUDWidgetInstance->GetUltimateSkill())
		{

		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("WeaponSwap"));
}

void ABattlePlayerController::Item1()
{
	if (!TryExecuteBattleAction(EBattleActionType::Item1)) return;

	SkillCancel();

	UBattlefieldManagerSubsystem* battleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
	if (!battleSubsystem) return;

	TArray<FBattlePotionRuntimeSlotData>& battlePotionSlots = battleSubsystem->GetBattlePotionRuntimeSlots();
	if (!battlePotionSlots.IsValidIndex(0)) return;

	FBattlePotionRuntimeSlotData& potionSlot = battlePotionSlots[0];
	if (!potionSlot.PotionTag.IsValid()) return;
	if (potionSlot.Count <= 0) return;
	if (potionSlot.CurrentCooldown > 0.0f) return;

	UseBattlePotion(0);
	potionSlot.CurrentCooldown = potionSlot.MaxCooldown;
	RefreshBattlePotion();
}

void ABattlePlayerController::Item2()
{
	if (!TryExecuteBattleAction(EBattleActionType::Item2)) return;

	SkillCancel();

	UBattlefieldManagerSubsystem* battleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
	if (!battleSubsystem) return;

	TArray<FBattlePotionRuntimeSlotData>& battlePotionSlots = battleSubsystem->GetBattlePotionRuntimeSlots();
	if (!battlePotionSlots.IsValidIndex(1)) return;

	FBattlePotionRuntimeSlotData& potionSlot = battlePotionSlots[1];
	if (!potionSlot.PotionTag.IsValid()) return;
	if (potionSlot.Count <= 0) return;
	if (potionSlot.CurrentCooldown > 0.0f) return;

	UseBattlePotion(1);
	potionSlot.CurrentCooldown = potionSlot.MaxCooldown;
	RefreshBattlePotion();
}

void ABattlePlayerController::UseBattlePotion(int32 InSlotIndex)
{
	UBattlefieldManagerSubsystem* battleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
	if (!battleSubsystem) return;

	const FInBattleData& inBattleData = battleSubsystem->GetInBattleData();
	if (!inBattleData.PlayerData.BattlePotionSlots.IsValidIndex(InSlotIndex)) return;

	const FBattlePotionSlotData& potionSlot = inBattleData.PlayerData.BattlePotionSlots[InSlotIndex];
	if (!potionSlot.PotionTag.IsValid() || potionSlot.Count <= 0) return;

	const FDTItemCatalogRow* catalogRow = FPlayerAccountService::FindItemCatalogRowByTag(this, potionSlot.PotionTag);
	if (!catalogRow) return;

	UGameDataSubsystem* gameDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGameDataSubsystem>();
	if (!gameDataSubsystem) return;

	FDTPotionInfoRow* potionRow = gameDataSubsystem->GetRow<FDTPotionInfoRow>(catalogRow->DetailTableTag, catalogRow->DetailRowName);
	if (!potionRow) return;

	// 시그온 세트 효과
	float potionMultiplier = 1.0f;

	if (battleSubsystem->GetEquippedSetBonusTag() == Arcanum::Items::SetBonus::Avarice)
	{
		potionMultiplier = 1.2f;
	}

	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (!playerCharacter) return;

	USkeletalMeshComponent* meshComp = playerCharacter->GetMesh();
	if (!meshComp) return;

	// 몽타주
	if (!potionRow->UseMontage.IsNull())
	{
		if (UAnimMontage* useMontage = potionRow->UseMontage.LoadSynchronous())
		{
			if (UAnimInstance* animInstance = meshComp->GetAnimInstance())
			{
				FOnMontageEnded montageEndedDelegate;
				montageEndedDelegate.BindUObject(this, &ABattlePlayerController::OnPotionMontageEnded);

				bIsPotionMontagePlaying = true;

				animInstance->Montage_Play(useMontage);
				animInstance->Montage_SetEndDelegate(montageEndedDelegate, useMontage);
			}
		}
	}

	// 나이아가라
	if (!potionRow->UseNiagara.IsNull())
	{
		if (UNiagaraSystem* useNiagara = potionRow->UseNiagara.LoadSynchronous())
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(
				useNiagara,
				meshComp,
				TEXT("Chest"),
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				EAttachLocation::SnapToTarget,
				false,
				true,
				ENCPoolMethod::AutoRelease,
				true
			);
		}
	}

	if (potionRow->EffectType == EPotionEffectType::Instant)
	{
		UCharacterBattleStatsComponent* statComponent = playerCharacter->FindComponentByClass<UCharacterBattleStatsComponent>();
		if (!statComponent) return;

		float instantValue = potionRow->InstantModifier.Value.Flat * potionMultiplier;

		if (potionRow->InstantModifier.StatTag == Arcanum::BattleStat::Character::Regen::Health::Root)
		{
			const TArray<FRegenStat>& regenStats = statComponent->GetRegenStats();

			for (const FRegenStat& regenStat : regenStats)
			{
				if (regenStat.ParentTag == Arcanum::BattleStat::Character::Regen::Health::Root)
				{
					instantValue = regenStat.GetTotalMax() * potionRow->InstantModifier.Value.Flat * potionMultiplier;
					break;
				}
			}
		}

		playerCharacter->ChangeStat(potionRow->InstantModifier.StatTag, instantValue);
	}
	else
	{
		for (const FDerivedStatModifier& modifier : potionRow->Modifiers)
		{
			FDerivedStatModifier scaledModifier = modifier;
			scaledModifier.Value.Flat *= potionMultiplier;
			scaledModifier.Value.Mul *= potionMultiplier;

			UE_LOG(LogTemp, Warning, TEXT("[Potion] Stat=%s Mult=%.2f FinalFlat=%.2f FinalMul=%.2f"),
				*scaledModifier.StatTag.ToString(),
				 potionMultiplier, scaledModifier.Value.Flat, 
			 	 scaledModifier.Value.Mul);

			playerCharacter->ApplyPotionModifier(scaledModifier);
			//playerCharacter->ApplyPotionModifier(modifier);
		}

		if (HUDWidgetInstance && battleSubsystem->GetBattlePotionRuntimeSlots().IsValidIndex(InSlotIndex))
		{
			if (UTexture2D* buffIcon = battleSubsystem->GetBattlePotionRuntimeSlots()[InSlotIndex].Icon)
			{
				HUDWidgetInstance->AddBuffSlot(potionSlot.PotionTag, 1.0f, buffIcon);
			}
		}
	}

	battleSubsystem->DecreaseBattlePotionCount(InSlotIndex);
}

void ABattlePlayerController::AutoManualModeMobile(bool bIsChecked)
{
	SkillCancel();
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("AutoManualMode"));
	bIsAutoManual = bIsChecked;

	if (!CachedPlayerCharacter.IsValid())
	{
		CachedPlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	}

	if (CachedPlayerCharacter.IsValid())
	{
		CachedPlayerCharacter->SetAutoMode(this, bIsAutoManual);
	}
}

void ABattlePlayerController::AutoManualModePC()
{
	SkillCancel();
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("AutoManualMode"));
}

void ABattlePlayerController::SlotSelectCancel()
{
	if (SelectedSlot.IsValid())
	{
		SelectedSlot = nullptr;
	}

	if (SpawnTag.IsValid())
	{
		SpawnTag = FGameplayTag::EmptyTag;
	}

	for (auto& UsingAllyUnitSlot : UsingAllyUnitSlots)
	{
		UsingAllyUnitSlot.Value->SetSelectSlot(false);
	}

	if (SelectedUnit.IsValid())
	{
		UPoolingSubsystem* PoolingSystem = GetWorld()->GetSubsystem<UPoolingSubsystem>();
		if (PoolingSystem)
		{
			PoolingSystem->DeactiveItem(SelectedUnit.Get());
		}
	}
	if (SelectedUnit2.IsValid())
	{
		UPoolingSubsystem* PoolingSystem = GetWorld()->GetSubsystem<UPoolingSubsystem>();
		if (PoolingSystem)
		{
			PoolingSystem->DeactiveItem(SelectedUnit2.Get());
		}
	}
	SetSpawnDecalActive(false);
	SelectedUnit = nullptr;
	SelectedUnit2 = nullptr;
	if (SelectedArrowInstance)
	{
		SelectedArrowInstance->SetActive(false);
	}

	SkillCancel();
}

void ABattlePlayerController::CommonButton()
{
	if (SelectedUnit.IsValid() && !bIsAutoManual)
	{
		Internal_SpawnUnit();
	}
	else if (CurrentSelectedSkillBase.IsValid())
	{
		//CurrentSelectedSkillStarter();
	}
}

void ABattlePlayerController::ReadySpawnUnit(FGameplayTag InTag, UBattleAllyUnitSlotWidget* Slot)
{
	if (!IsUnitUsingEnable(InTag)) return;

	SlotSelectCancel();

	if (Slot)
	{
		SpawnTag = InTag;
		Slot->SetSelectSlot(true);
		SelectedSlot = Slot;
	}
	if (FUnitInfoSetting* UnitData = UsingAllyUnits.Find(SpawnTag))
	{
		UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>();
		if (PoolingSubsystem)
		{

			FHitResult HitResult;
			GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
			FTransform Transform;
			Transform.SetLocation(HitResult.ImpactPoint);
			// Todo KDH : 미리 로드해 놓아야함
			AActor* EnemyUnitInstance = PoolingSubsystem->SpawnFromPool(UnitData->UnitClass.LoadSynchronous(), Transform);
			ABaseUnitCharacter* EnemyUnitCharacter = Cast<ABaseUnitCharacter>(EnemyUnitInstance);
			SetSpawnDecalActive(true);
			if (EnemyUnitCharacter)
			{
				EnemyUnitCharacter->SetUnit(*UnitData, true);
				SelectedUnit = EnemyUnitCharacter;
			}
		}
	}
}

void ABattlePlayerController::SetSpawnDecalActive(bool bIsOn)
{
	UBattlefieldManagerSubsystem* BattleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
	if (BattleSubsystem)
	{
		if (ASpawnCheckDecal* SpawnCheckDecal = BattleSubsystem->GetSpawnCheckDecal())
		{
			SpawnCheckDecal->SetSpawnDecalActive(bIsOn);
		}
	}
}

ABaseUnitCharacter* ABattlePlayerController::Internal_SpawnUnit(const FVector InSpawnLocation)
{
	if (FUnitInfoSetting* UnitData = UsingAllyUnits.Find(SpawnTag))
	{
		UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>();
		if (PoolingSubsystem)
		{

			FHitResult HitResult;
			GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
			if (IsUnitUsingEnable(SpawnTag))
			{
				UsingUnitCost(SpawnTag);

				GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
				FVector ResultLocation = HitResult.ImpactPoint;
				float CapsuleHeight = SelectedUnit->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 0.5f;

				FVector Direction = FVector::RightVector;

				UBattlefieldManagerSubsystem* BattleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
				if (BattleSubsystem)
				{
					if (BattleSubsystem->GetSpawnCheckDecal())
					{
						bool bIsInSide = false;
						ResultLocation = BattleSubsystem->GetSpawnCheckDecal()->CalculateMinDistanceLocation(ResultLocation, bIsInSide);
					}
				}

				ResultLocation = ResultLocation + (FVector::UpVector * CapsuleHeight);

				FTransform Transform;
				if (InSpawnLocation.IsNearlyZero())
				{
					Transform.SetLocation(ResultLocation);
				}
				else
				{
					Transform.SetLocation(InSpawnLocation);
				}
				Transform.SetRotation(Direction.Rotation().Quaternion());
				// Todo KDH : 미리 로드해 놓아야함
				AActor* EnemyUnitInstance = PoolingSubsystem->SpawnFromPool(UnitData->UnitClass.LoadSynchronous(), Transform);
				ABaseUnitCharacter* EnemyUnitCharacter = Cast<ABaseUnitCharacter>(EnemyUnitInstance);

				if (EnemyUnitCharacter)
				{
					EnemyUnitCharacter->SetUnit(*UnitData);
					SlotSelectCancel();
					return EnemyUnitCharacter;
				}
			}
		}
	}

	return nullptr;
}

bool ABattlePlayerController::UseMeatValue(float Value)
{
	if (MeatValue.Current >= Value)
	{
		MeatValue.Current -= Value;
		MeatValue.Current = FMath::Clamp(MeatValue.Current, 0.0f, MeatValue.BaseMax);
		return true;
	}

	return false;
}

bool ABattlePlayerController::UseManaValue(float Value)
{
	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (!playerCharacter) return false;

	UCharacterBattleStatsComponent* statComponent = playerCharacter->FindComponentByClass<UCharacterBattleStatsComponent>();
	if (!statComponent) return false;

	const FRegenStat* manaStat = statComponent->FindRegenStat(ManaTag);
	if (!manaStat) return false;

	if (manaStat->Current >= Value)
	{
		playerCharacter->ChangeStat(ManaTag, -Value);
		return true;
	}

	return false;

	//if (ManaValue.Current >= Value)
	//{
	//	ManaValue.Current -= Value;
	//	ManaValue.Current = FMath::Clamp(ManaValue.Current, 0.0f, ManaValue.BaseMax);
	//	return true;
	//}

	//return false;
}

bool ABattlePlayerController::UseCoolTime(FGameplayTag InTag)
{
	FUnitInfoSetting* UnitData = UsingAllyUnits.Find(InTag);
	if (!UnitData) return false;

	UnitData->CurrentCoolTime = UnitData->CoolTime;
	return true;
}

bool ABattlePlayerController::UseSkillCost(FGameplayTag InTag)
{
	float MaxCoolTime = 0.0f;

	float manaCost = 0.0f;
	float meatCost = 0.0f;

	TObjectPtr<USkillBase>* skillBaseBack = SkillBaseInstances.Find(InTag);
	if (!skillBaseBack) return false;

	const FLevelModifierEntry* levelModifier = (*skillBaseBack)->GetCurrentLevelEntry();
	if (!levelModifier) return false;

	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (!playerCharacter) return false;

	UCharacterBattleStatsComponent* statComponent = playerCharacter->FindComponentByClass<UCharacterBattleStatsComponent>();
	if (!statComponent) return false;

	const FRegenStat* manaStat = statComponent->FindRegenStat(ManaTag);
	if (!manaStat) return false;

	MaxCoolTime = levelModifier->Cooldown;
	float* currentCoolTime = SkillCoolTimes.Find(InTag);
	if (!currentCoolTime)
	{
		currentCoolTime = &SkillCoolTimes.Add(InTag);
		*currentCoolTime = 0.0f;
	}

	for (const auto& cost : levelModifier->Cost)
	{
		if (cost.StatTag == ManaValueTag)
		{
			manaCost += cost.Value.Flat;
		}
		else if (cost.StatTag == MeatTag)
		{
			meatCost += cost.Value.Flat;
		}
	}

	if (*currentCoolTime > 0.0f) return false;
	if (manaStat->Current < FMath::Abs(manaCost)) return false;
	if (MeatValue.Current < FMath::Abs(meatCost)) return false;

	*currentCoolTime = MaxCoolTime;

	if (!FMath::IsNearlyZero(manaCost))
	{
		playerCharacter->ChangeStat(ManaTag, manaCost);
	}

	if (!FMath::IsNearlyZero(meatCost))
	{
		MeatValue.Current += meatCost;
		MeatValue.Current = FMath::Clamp(MeatValue.Current, 0.0f, MeatValue.BaseMax);
	}

	return true;


	//float MaxCoolTime = 0.0f;

	//float ManaCost = 0.0f;
	//float MeatCost = 0.0f;
	//if (TObjectPtr<USkillBase>* SkillBaseBack = SkillBaseInstances.Find(InTag))
	//{
	//	if (const FLevelModifierEntry* LevelModifier = (*SkillBaseBack)->GetCurrentLevelEntry())
	//	{
	//		MaxCoolTime = LevelModifier->Cooldown;
	//		float* CurrentCoolTime = SkillCoolTimes.Find(InTag);
	//		if (!CurrentCoolTime)
	//		{
	//			CurrentCoolTime = &SkillCoolTimes.Add(InTag);
	//			*CurrentCoolTime = 0.0f;
	//		}

	//		if (CurrentCoolTime)
	//		{
	//			for (const auto& Cost : LevelModifier->Cost) // 코스트 체크
	//			{
	//				if (Cost.StatTag == ManaValueTag)
	//				{
	//					ManaCost += Cost.Value.Flat;
	//				}
	//				else if (Cost.StatTag == MeatTag)
	//				{
	//					MeatCost += Cost.Value.Flat;
	//				}
	//			}

	//			if (*CurrentCoolTime <= 0.0f && ManaValue.Current >= FMath::Abs(ManaCost) && MeatValue.Current >= FMath::Abs(MeatCost))
	//			{
	//				*CurrentCoolTime = MaxCoolTime;
	//				ManaValue.Current += ManaCost;
	//				MeatValue.Current += MeatCost;
	//				return true;
	//			}
	//		}
	//	}
	//}

	//return false;
}

void ABattlePlayerController::BattleEnd(const FMatchData& MatchData)
{
	if (HUDWidgetInstance)
	{
		UBattleBattleEndWidget* BattleEndWidget = HUDWidgetInstance->GetBattleEndWidget();
		if (BattleEndWidget)
		{
			BattleEndWidget->SetVictoryText(MatchData.bIsVictory);
			UE_LOG(LogTemp, Error, TEXT("MatchData.bIsVictory = %d"), MatchData.bIsVictory);
			BattleEndWidget->SetClearTimeText(MatchData.EndTimeSecond);

			UBattlefieldManagerSubsystem* BattleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
			if (BattleSubsystem)
			{
				float PlayingTime = static_cast<float>(BattleSubsystem->GetInBattleData().BattleStageInfo.StageLimitTime) - static_cast<float>(MatchData.EndTimeSecond);
				float Result = static_cast<float>(MatchData.EndTimeSecond) / static_cast<float>(BattleSubsystem->GetInBattleData().BattleStageInfo.StageLimitTime);

				BattleEndWidget->SetClearTimeText(PlayingTime);

				if (MatchData.bIsVictory)
				{
					Result *= 3.0f;
					UE_LOG(LogTemp, Error, TEXT("Result : %f"), Result);
					BattleEndWidget->SetStar(FMath::Clamp<int32>(FMath::RoundToInt(Result), 0, 3));
				}
				else
				{
					BattleEndWidget->SetStar(0);
				}
			}
			else
			{
				BattleEndWidget->SetStar(0);
			}

			BattleEndWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void ABattlePlayerController::OpenLobbyLevel()
{
	// Todo KDH 로비 열기
	UGameplayStatics::OpenLevel(this, FName("LobbyMap"));
}

bool ABattlePlayerController::IsUnitUsingEnable(FGameplayTag InTag)
{
	if (!InTag.IsValid()) return false;
	if (!UsingAllyUnits.Contains(InTag)) return false;

	FUnitInfoSetting UnitData = *UsingAllyUnits.Find(InTag);

	//쿨타임 체크하고 고기 코스트 체크
	if (UnitData.CurrentCoolTime <= 0.0f &&
		UnitData.MeatCost <= MeatValue.Current)
	{
		return true;
	}

	return false;
}

bool ABattlePlayerController::UsingUnitCost(FGameplayTag InTag)
{
	if (!IsUnitUsingEnable(InTag)) return false;
	FUnitInfoSetting* UnitData = UsingAllyUnits.Find(InTag);

	// 고기 사용
	UseMeatValue(UnitData->MeatCost);

	// 쿨타임 돌기
	UseCoolTime(InTag);

	return true;
}

void ABattlePlayerController::Internal_CoolTimeTick(float DeltaTime)
{
	UBattlefieldManagerSubsystem* BattleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();

	for (auto& UsingAllyUnit : UsingAllyUnits)
	{
		if (UsingAllyUnit.Value.CurrentCoolTime > 0.0f)
		{
			float& CurrentCoolTime = UsingAllyUnit.Value.CurrentCoolTime;
			const float& MaxCoolTime = UsingAllyUnit.Value.CoolTime;

			CurrentCoolTime -= DeltaTime;

			if (UBattleAllyUnitSlotWidget** SlotWidget = UsingAllyUnitSlots.Find(UsingAllyUnit.Key))
			{
				(*SlotWidget)->SetCoolTimeProgress(CurrentCoolTime, MaxCoolTime);
			}
		}
	}

	for (auto& SkillCoolTime : SkillCoolTimes)
	{
		SkillCoolTime.Value -= DeltaTime;
		/*if (SkillCoolTime.Key == BattleSubsystem->GetCurrentBasicAttackSkillTag())
		{
			HUDWidgetInstance->GetBasicAttack()->SetCoolTimeProgress(SkillCoolTime.Value, (*SkillBaseInstances.Find(SkillCoolTime.Key))->GetCurrentLevelEntry()->Cooldown);
		}
		else if (SkillCoolTime.Key == BattleSubsystem->GetCurrentBasicSkillTag())
		{
			HUDWidgetInstance->GetBasicSkill()->SetCoolTimeProgress(SkillCoolTime.Value, (*SkillBaseInstances.Find(SkillCoolTime.Key))->GetCurrentLevelEntry()->Cooldown);

		}
		else if (SkillCoolTime.Key == BattleSubsystem->GetLegendaryUltimateSkillTag())
		{
			HUDWidgetInstance->GetUltimateSkill()->SetCoolTimeProgress(SkillCoolTime.Value, (*SkillBaseInstances.Find(SkillCoolTime.Key))->GetCurrentLevelEntry()->Cooldown);
		}*/
	}
}

void ABattlePlayerController::InitialSkillBase()
{
	/*const TMap<FGameplayTag, FDerivedStatModifier>* weaponSlot1OnHitTarget = InBattleData.WeaponOnHitTarget.Find(Arcanum::Items::ItemSlot::Weapon::Slot1);
	const TMap<FGameplayTag, FDerivedStatModifier>* weaponSlot2OnHitTarget = InBattleData.WeaponOnHitTarget.Find(Arcanum::Items::ItemSlot::Weapon::Slot2);
	const TMap<FGameplayTag, FDerivedStatModifier>* legendaryWeaponOnHitTarget = InBattleData.WeaponOnHitTarget.Find(Arcanum::Items::ItemSlot::Weapon::Legendary);

	if (weaponSlot1OnHitTarget)
	{
		const FDerivedStatModifier* healthModifier = weaponSlot1OnHitTarget->Find(Arcanum::BattleStat::Character::Regen::Health::Value);
		if (healthModifier)
		{
			const float damage = healthModifier->Value.Flat;
		}
	}*/
	UBattlefieldManagerSubsystem* BattleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
	UGameDataSubsystem* DataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGameDataSubsystem>();

	FString SkillName;
	USkillBase* SkillBaseBack = nullptr;

	// 슬롯1 기본공격
	FBattleSkillData SkillData = BattleSubsystem->GetInBattleData().BattleWeaponSkill.WeaponSlot1BasicAttackSkill;
	if (!SkillBaseInstances.Contains(SkillData.SkillTag))
	{
		SkillBaseBack = NewObject<USkillBase>(this);
		SkillBaseInstances.Add(SkillData.SkillTag, SkillBaseBack);
		if (DataSubsystem)
		{
			SkillName = SkillData.SkillTag.ToString();
			int32 LastDot;
			if (SkillName.FindLastChar('.', LastDot))
			{
				// 찾은 인덱스 다음(+1)부터 끝까지 남기고 앞은 다 자릅니다.
				SkillName = SkillName.RightChop(LastDot + 1);
			}

			FDTSkillsDataRow* DTSkillsDataRow = DataSubsystem->GetRow<FDTSkillsDataRow>(Arcanum::DataTable::SkillData, FName(SkillName));

			if (DTSkillsDataRow)
			{
				const TMap<FGameplayTag, FDerivedStatModifier>* weaponSlot1OnHitTarget = BattleSubsystem->GetInBattleData().WeaponOnHitTarget.Find(Arcanum::Items::ItemSlot::Weapon::Slot1);

				FDerivedStatModifier AddModi;
				if (weaponSlot1OnHitTarget)
				{
					AddModi = weaponSlot1OnHitTarget->Array()[0].Value;

					SkillBaseBack->Initialize(GetPawn(), &DTSkillsDataRow->SkillData, SkillData.SkillLevel, DTSkillsDataRow->SkillData.TargetFilterTag, AddModi);
				}
				else
				{
					SkillBaseBack->Initialize(GetPawn(), &DTSkillsDataRow->SkillData, SkillData.SkillLevel, DTSkillsDataRow->SkillData.TargetFilterTag, FDerivedStatModifier());

				}
			}
		}
	}

	// 슬롯2 기본공격
	SkillData = BattleSubsystem->GetInBattleData().BattleWeaponSkill.WeaponSlot2BasicAttackSkill;
	if (!SkillBaseInstances.Contains(SkillData.SkillTag))
	{
		SkillBaseBack = NewObject<USkillBase>(this);
		SkillBaseInstances.Add(SkillData.SkillTag, SkillBaseBack);
		if (DataSubsystem)
		{
			SkillName = SkillData.SkillTag.ToString();
			int32 LastDot;
			if (SkillName.FindLastChar('.', LastDot))
			{
				// 찾은 인덱스 다음(+1)부터 끝까지 남기고 앞은 다 자릅니다.
				SkillName = SkillName.RightChop(LastDot + 1);
			}

			FDTSkillsDataRow* DTSkillsDataRow = DataSubsystem->GetRow<FDTSkillsDataRow>(Arcanum::DataTable::SkillData, FName(SkillName));

			if (DTSkillsDataRow)
			{
				const TMap<FGameplayTag, FDerivedStatModifier>* weaponSlot1OnHitTarget = BattleSubsystem->GetInBattleData().WeaponOnHitTarget.Find(Arcanum::Items::ItemSlot::Weapon::Slot2);

				FDerivedStatModifier AddModi;
				if (weaponSlot1OnHitTarget)
				{
					AddModi = weaponSlot1OnHitTarget->Array()[0].Value;

					SkillBaseBack->Initialize(GetPawn(), &DTSkillsDataRow->SkillData, SkillData.SkillLevel, DTSkillsDataRow->SkillData.TargetFilterTag, AddModi);
				}
				else
				{
					SkillBaseBack->Initialize(GetPawn(), &DTSkillsDataRow->SkillData, SkillData.SkillLevel, DTSkillsDataRow->SkillData.TargetFilterTag, FDerivedStatModifier());

				}
			}
		}
	}

	// 슬롯1 기본스킬
	SkillData = BattleSubsystem->GetInBattleData().BattleWeaponSkill.WeaponSlot1BasicSkill;
	if (!SkillBaseInstances.Contains(SkillData.SkillTag))
	{
		SkillBaseBack = NewObject<USkillBase>(this);
		SkillBaseInstances.Add(SkillData.SkillTag, SkillBaseBack);
		if (DataSubsystem)
		{
			SkillName = SkillData.SkillTag.ToString();
			int32 LastDot;
			if (SkillName.FindLastChar('.', LastDot))
			{
				// 찾은 인덱스 다음(+1)부터 끝까지 남기고 앞은 다 자릅니다.
				SkillName = SkillName.RightChop(LastDot + 1);
			}

			FDTSkillsDataRow* DTSkillsDataRow = DataSubsystem->GetRow<FDTSkillsDataRow>(Arcanum::DataTable::SkillData, FName(SkillName));

			if (DTSkillsDataRow)
			{
				SkillBaseBack->Initialize(GetPawn(), &DTSkillsDataRow->SkillData, SkillData.SkillLevel, DTSkillsDataRow->SkillData.TargetFilterTag);
			}
		}
	}

	// 슬롯2 기본스킬
	SkillData = BattleSubsystem->GetInBattleData().BattleWeaponSkill.WeaponSlot2BasicSkill;
	if (!SkillBaseInstances.Contains(SkillData.SkillTag))
	{
		SkillBaseBack = NewObject<USkillBase>(this);
		SkillBaseInstances.Add(SkillData.SkillTag, SkillBaseBack);
		if (DataSubsystem)
		{
			SkillName = SkillData.SkillTag.ToString();
			int32 LastDot;
			if (SkillName.FindLastChar('.', LastDot))
			{
				// 찾은 인덱스 다음(+1)부터 끝까지 남기고 앞은 다 자릅니다.
				SkillName = SkillName.RightChop(LastDot + 1);
			}

			FDTSkillsDataRow* DTSkillsDataRow = DataSubsystem->GetRow<FDTSkillsDataRow>(Arcanum::DataTable::SkillData, FName(SkillName));

			if (DTSkillsDataRow)
			{
				SkillBaseBack->Initialize(GetPawn(), &DTSkillsDataRow->SkillData, SkillData.SkillLevel, DTSkillsDataRow->SkillData.TargetFilterTag);
			}
		}
	}

	//궁극기
	SkillData = BattleSubsystem->GetInBattleData().BattleWeaponSkill.LegendaryUltimateSkill;
	if (!SkillBaseInstances.Contains(SkillData.SkillTag))
	{
		SkillBaseBack = NewObject<USkillBase>(this);
		SkillBaseInstances.Add(SkillData.SkillTag, SkillBaseBack);
		if (DataSubsystem)
		{
			SkillName = SkillData.SkillTag.ToString();
			int32 LastDot;
			if (SkillName.FindLastChar('.', LastDot))
			{
				// 찾은 인덱스 다음(+1)부터 끝까지 남기고 앞은 다 자릅니다.
				SkillName = SkillName.RightChop(LastDot + 1);
			}

			FDTSkillsDataRow* DTSkillsDataRow = DataSubsystem->GetRow<FDTSkillsDataRow>(Arcanum::DataTable::SkillData, FName(SkillName));

			if (DTSkillsDataRow)
			{
				SkillBaseBack->Initialize(GetPawn(), &DTSkillsDataRow->SkillData, SkillData.SkillLevel, DTSkillsDataRow->SkillData.TargetFilterTag);
			}
		}
	}
}

void ABattlePlayerController::RangeDecalOn(float DecalSize)
{
	SkillRangeDecalInstance->SkillRangeDecalOn(DecalSize);
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &ABattlePlayerController::Internal_RangeDecalOn, DecalSize);
	GetWorld()->GetTimerManager().ClearTimer(RangeDecalTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(RangeDecalTimerHandle, TimerDelegate, RangeTime, false, RangeTime);
}

void ABattlePlayerController::Internal_RangeDecalOn(float DecalSize)
{
	SkillRangeDecalInstance->SkillRangeDecalOff();
}

// ========================================================
// 인풋모드 설정
// ========================================================
void ABattlePlayerController::SetupInputMode()
{
#if PLATFORM_WINDOWS
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

#elif PLATFORM_ANDROID || PLATFORM_IOS
	bShowMouseCursor = false;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	SetVirtualJoystickVisibility(false);
#endif

	//FInputModeGameOnly InputMode;
	//FInputModeUIOnly InputMode;
	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	//InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	SetInputMode(InputMode);
}

// ========================================================
// 입력 관련
// ========================================================
void ABattlePlayerController::InputMove(const FInputActionValue& InputValue)
{
	// 1. 입력값을 Vector2D로 변환 (W/S는 Y, A/D는 X)
	FVector2D MovementVector = InputValue.Get<FVector2D>();

	if (APawn* ControlledPawn = GetPawn())
	{
		const FRotator Rotation = PlayerCameraManager->GetCameraRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// 3. 앞방향(Forward)과 오른쪽방향(Right) 계산
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// 4. 궁극기 조준 중이면 캐릭터는 멈추고 프리뷰만 이동
		if (bIsUltimateAiming)
		{
			APlayerCharacter* PC = Cast<APlayerCharacter>(ControlledPawn);
			if (PC && !MovementVector.IsNearlyZero())
			{
				// 입력 방향을 월드 기준으로 변환 및 이동
				FVector MoveDir = FVector(MovementVector.Y, MovementVector.X, 0.0f).GetSafeNormal();
				LastMoveInputDirection = MoveDir;
				CurrentUltimatePreviewLocation += MoveDir * UltimatePreviewMoveSpeed * GetWorld()->GetDeltaSeconds();

				// 캐릭터로부터의 상대 좌표 계산
				const FVector ActorLoc = PC->GetActorLocation();
				FVector RelativePos = CurrentUltimatePreviewLocation - ActorLoc;
				RelativePos.Z = 0.0f;

				// 전방 180도
				const FVector Forward = PC->GetActorForwardVector().GetSafeNormal2D();
				const FVector DirToPreview = RelativePos.GetSafeNormal2D();
				if (FVector::DotProduct(Forward, DirToPreview) < 0.f)
				{
					const FVector Right = PC->GetActorRightVector().GetSafeNormal2D();
					float ProjectionDist = FVector::DotProduct(RelativePos, Right);
					RelativePos = Right * ProjectionDist;
				}

				// 최대 거리 제한
				RelativePos = RelativePos.GetClampedToMaxSize(UltimatePreviewMaxDistance);

				// 최종 위치 확정 및 업데이트
				CurrentUltimatePreviewLocation = ActorLoc + RelativePos;
				PC->UpdateUltimatePreviewLocation(CurrentUltimatePreviewLocation);
			}
		}
		else
		{
			// 5. 평소에는 Pawn에 이동 입력 반영
			ControlledPawn->AddMovementInput(ForwardDirection, MovementVector.Y);
			ControlledPawn->AddMovementInput(RightDirection, MovementVector.X);
		}
	}
}

void ABattlePlayerController::TriggerBasicAttackHit()
{
	UBattlefieldManagerSubsystem* battleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
	if (!battleSubsystem) return;
	const FBattleSkillData* basicAttackSkillData = battleSubsystem->GetCurrentBasicAttackSkillData();
	if (!basicAttackSkillData) return;
	const FSkillInfo* skillInfo = battleSubsystem->GetCurrentBasicAttackSkillInfo();
	if (!skillInfo) return;
	if (!basicAttackSkillData->SkillClass) return;
	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (!playerCharacter) return;
	UPoolingSubsystem* poolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>();
	if (!poolingSubsystem) return;
	UClass* skillActorClass = basicAttackSkillData->SkillClass.Get();
	if (!skillActorClass) return;

	USkillBase* skillObject = nullptr;
	if (SkillBaseInstances.Contains(skillInfo->SkillNameTag))
	{
		skillObject = *SkillBaseInstances.Find(skillInfo->SkillNameTag);
	}
	else
	{
		skillObject = NewObject<USkillBase>(this);
		SkillBaseInstances.Add(skillInfo->SkillNameTag, skillObject);
	}

	UE_LOG(LogTemp, Warning, TEXT("BasicAttack SkillTag=%s SkillLevel=%d"),
		*basicAttackSkillData->SkillTag.ToString(),
		basicAttackSkillData->SkillLevel);

	skillObject->Initialize(playerCharacter, skillInfo, basicAttackSkillData->SkillLevel, skillInfo->TargetFilterTag);

	const FTransform spawnTransform = playerCharacter->GetActorTransform();
	const FVector spawnLocation = spawnTransform.GetLocation();
	const FRotator spawnRotation = spawnTransform.Rotator();

	AActor* spawnedActor = poolingSubsystem->SpawnFromPool(skillActorClass, spawnTransform);
	ASkillActor* skillActor = Cast<ASkillActor>(spawnedActor);
	if (!skillActor) return;

	RangeDecalOn(skillObject->GetSkillInfo().EnabledRange);

	skillActor->ActivateSkillActor(skillObject, playerCharacter, spawnLocation, spawnRotation);

	StartBasicAttackCooldown();
}

void ABattlePlayerController::TriggerSkill()
{
	UE_LOG(LogTemp, Warning, TEXT("TriggerSkill Enter Ultimate=%d"), bIsUltimateAiming);

	UBattlefieldManagerSubsystem* battleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
	if (!battleSubsystem) return;

	const FBattleSkillData* skillData = bIsUltimateAiming
		? battleSubsystem->GetCurrentLegendarySkillData()
		: battleSubsystem->GetCurrentBasicSkillData();

	const FSkillInfo* skillInfo = bIsUltimateAiming
		? battleSubsystem->FindSkillInfoByTag(battleSubsystem->GetLegendaryUltimateSkillTag())
		: battleSubsystem->GetCurrentBasicSkillInfo();

	if (!skillData) return;
	if (!skillInfo) return;
	if (!skillData->SkillClass) return;

	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (!playerCharacter) return;
	UPoolingSubsystem* poolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>();
	if (!poolingSubsystem) return;
	UClass* skillActorClass = skillData->SkillClass.Get();
	if (!skillActorClass) return;

	USkillBase* skillObject = nullptr;
	if (SkillBaseInstances.Contains(skillInfo->SkillNameTag))
	{
		skillObject = *SkillBaseInstances.Find(skillInfo->SkillNameTag);
	}
	else
	{
		skillObject = NewObject<USkillBase>(this);
		SkillBaseInstances.Add(skillInfo->SkillNameTag, skillObject);
	}

	UE_LOG(LogTemp, Warning, TEXT("Skill SkillTag=%s SkillLevel=%d"),
		*skillData->SkillTag.ToString(),
		skillData->SkillLevel);


	//if (bIsUltimateAiming) return;

	FVector targetLocation = playerCharacter->GetUltimateLocation();
	skillObject->Initialize(playerCharacter, skillInfo, skillData->SkillLevel, skillInfo->TargetFilterTag, FDerivedStatModifier(), nullptr, targetLocation);

	const FTransform spawnTransform = playerCharacter->GetActorTransform();
	const FVector spawnLocation = spawnTransform.GetLocation();
	const FRotator spawnRotation = spawnTransform.Rotator();

	AActor* spawnedActor = poolingSubsystem->SpawnFromPool(skillActorClass, spawnTransform);
	ASkillActor* skillActor = Cast<ASkillActor>(spawnedActor);
	if (!skillActor) return;
	if (!SkillCostChecker(skillObject->GetSkillInfo().SkillNameTag, skillObject->GetCurrentLevelEntry()->Level, true)) return;
	UseSkillCost(skillObject->GetSkillInfo().SkillNameTag);

	RangeDecalOn(skillObject->GetSkillInfo().EnabledRange);

	skillActor->ActivateSkillActor(skillObject, playerCharacter, spawnLocation, spawnRotation);
	skillActor->SetTargetActor(nullptr);
	skillActor->SetTargetLocation(targetLocation);

	bIsUltimateAiming ? StartUltimateCooldown() : StartBasicSkillCooldown();
}


void ABattlePlayerController::InputBasicAttack()
{
	if (!TryExecuteBattleAction(EBattleActionType::BasicAttack)) return;

	if (UBattlefieldManagerSubsystem* battleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>())
	{
		const FGameplayTag skillTag = battleSubsystem->GetCurrentBasicAttackSkillTag();
		if (GetSkillCooldownRemaining(skillTag) > 0.0f)
		{
			return;
		}
	}

	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (playerCharacter)
	{
		playerCharacter->HandleBasicAttackInput();
	}
}

void ABattlePlayerController::InputSkill()
{
	if (!TryExecuteBattleAction(EBattleActionType::CommonSkill)) return;

	if (UBattlefieldManagerSubsystem* battleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>())
	{
		const FGameplayTag skillTag = battleSubsystem->GetCurrentBasicSkillTag();
		const int32 skillLevel = battleSubsystem->GetCurrentBasicSkillLevel();
		if (!SkillCostChecker(skillTag, skillLevel)) return;

		if (GetSkillCooldownRemaining(skillTag) > 0.0f)
		{
			return;
		}
	}

	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (playerCharacter)
	{
		playerCharacter->HandleCommonSkillInput();
	}
}

void ABattlePlayerController::OnWeaponSwapMontageEnded(UAnimMontage* InMontage, bool bInterrupted)
{
	bIsWeaponSwapping = false;
}

// ========================================================
// 궁극기
// ========================================================
void ABattlePlayerController::UltimateSkillEnd()
{
	EndUltimatePresentation();

	bIsUltimateAiming = false;

	UBattlefieldManagerSubsystem* battleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
	if (!battleSubsystem) return;

	battleSubsystem->EndLegendaryWeaponMode();

	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (playerCharacter)
	{
		playerCharacter->UpdateEquippedWeaponMesh();
	}
	if (UltimatePostProcessVolume)
	{
		UltimatePostProcessVolume->bUnbound = false;
	}
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->SetLegendaryButtonIcon(battleSubsystem->GetLegendaryWeaponIcon());
	}
	if (ActiveUltimateCameraShake)
	{
		ActiveUltimateCameraShake->StopShake(false);
		ActiveUltimateCameraShake = nullptr;
	}
}

void ABattlePlayerController::UltimateSkillPressed()
{
	if (!TryExecuteBattleAction(EBattleActionType::UltimatePress)) return;

	UBattlefieldManagerSubsystem* battleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
	if (!battleSubsystem) return;

	const FGameplayTag skillTag = battleSubsystem->GetLegendaryUltimateSkillTag();
	if (GetSkillCooldownRemaining(skillTag) > 0.0f) return;
	if (!skillTag.IsValid()) return;
	if (!SkillCostChecker(skillTag, battleSubsystem->GetLegendaryUltimateSkillLevel(), true)) return;

	const float castTime = battleSubsystem->GetInBattleData().BattleWeaponSkill.LegendaryUltimateSkill.CastTime;

	bIsUltimateAiming = true;
	battleSubsystem->BeginLegendaryWeaponMode();

	StartUltimatePresentation();

	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (playerCharacter)
	{
		const FVector actorLocation = playerCharacter->GetActorLocation();
		const FVector forwardDirection = playerCharacter->GetActorForwardVector();

		CurrentUltimatePreviewLocation = actorLocation + (forwardDirection * UltimatePreviewDistance);
		CurrentUltimatePreviewLocation.Z = actorLocation.Z;

		playerCharacter->UpdateEquippedWeaponMesh();
		playerCharacter->ShowUltimatePreview();
		playerCharacter->UpdateUltimatePreviewLocation(CurrentUltimatePreviewLocation);
		playerCharacter->PlayUltimatePressMontage();

		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->SetLegendaryButtonIcon(battleSubsystem->GetLegendaryUltimateSkillIcon());
		}
	}

	GetWorldTimerManager().ClearTimer(UltimateSkillTimerHandle);
	if (castTime > 0.0f)
	{
		GetWorldTimerManager().SetTimer(
			UltimateSkillTimerHandle,
			this,
			&ABattlePlayerController::ExecuteUltimateSkill,
			castTime,
			false
		);
	}
}

void ABattlePlayerController::UltimateSkillReleased()
{
	if (!TryExecuteBattleAction(EBattleActionType::UltimateRelease)) return;

	if (APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		if (playerCharacter->GetIsUltimateReleaseMontagePlaying())
		{
			return;
		}

		if (bIsUltimateAiming)
		{
			ExecuteUltimateSkill();
		}
	}
}

void ABattlePlayerController::ExecuteUltimateSkill()
{
	if (bIsUltimateAiming)
	{
		GetWorldTimerManager().ClearTimer(UltimateSkillTimerHandle);

		APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(GetPawn());
		if (playerCharacter)
		{
			playerCharacter->HideUltimatePreview();
		}

		UBattlefieldManagerSubsystem* battleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
		if (battleSubsystem)
		{
			const FGameplayTag skillTag = battleSubsystem->GetLegendaryUltimateSkillTag();
			const int32 skillLevel = battleSubsystem->GetLegendaryUltimateSkillLevel();

			UE_LOG(LogTemp, Warning, TEXT("UltimateSkill Tag=%s Level=%d"), *skillTag.ToString(), skillLevel);

		}

		PlayUltimateReleasePresentation();

		playerCharacter->PlayUltimateReleaseMontage();
	}

	if (ActiveUltimateCameraShake)
	{
		ActiveUltimateCameraShake->StopShake(false);
		ActiveUltimateCameraShake = nullptr;
	}
}

void ABattlePlayerController::StartUltimatePresentation()
{
	bIsUltimatePresentationActive = true;
	bIsUltimatePresentationRestoring = false;
	bIsUltimateReleaseZoomActive = false;
	UltimateTargetPostProcessBlendWeight = UltimatePressPostProcessBlendWeight;

	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (playerCharacter)
	{
		if (UCameraComponent* playerCamera = playerCharacter->GetCamera())
		{
			if (UltimateDefaultFOV <= 0.0f)
			{
				UltimateDefaultFOV = playerCamera->FieldOfView;
			}
		}
	}

	if (UltimateCameraShakeClass && PlayerCameraManager)
	{
		ActiveUltimateCameraShake = PlayerCameraManager->StartCameraShake(UltimateCameraShakeClass);
	}

	if (UltimatePostProcessVolume)
	{
		UltimatePostProcessVolume->bUnbound = true;
	}
}

void ABattlePlayerController::PlayUltimateReleasePresentation()
{
	bIsUltimatePresentationActive = true;
	bIsUltimatePresentationRestoring = false;
	bIsUltimateReleaseZoomActive = true;
	UltimateTargetPostProcessBlendWeight = UltimatePressPostProcessBlendWeight;

	if (UltimatePostProcessVolume)
	{
		UltimatePostProcessVolume->bUnbound = true;
	}
}

void ABattlePlayerController::EndUltimatePresentation()
{
	bIsUltimatePresentationActive = false;
	bIsUltimateReleaseZoomActive = false;
	bIsUltimatePresentationRestoring = true;
	UltimateTargetPostProcessBlendWeight = 0.0f;
}

void ABattlePlayerController::UpdateUltimatePresentation(float DeltaTime)
{
	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (!playerCharacter) return;

	UCameraComponent* playerCamera = playerCharacter->GetCamera();
	if (!playerCamera) return;

	const float targetFOV = bIsUltimatePresentationActive
		? (bIsUltimateReleaseZoomActive ? UltimateReleaseTargetFOV : UltimatePressTargetFOV)
		: UltimateDefaultFOV;

	const float currentInterpSpeed = bIsUltimatePresentationActive
		? (bIsUltimateReleaseZoomActive ? UltimateReleaseZoomInterpSpeed : UltimatePressZoomInterpSpeed)
		: UltimateRestoreInterpSpeed;

	playerCamera->SetFieldOfView(FMath::FInterpTo(
		playerCamera->FieldOfView,
		targetFOV,
		DeltaTime,
		currentInterpSpeed));

	bool bIsFOVRestored = FMath::IsNearlyEqual(playerCamera->FieldOfView, UltimateDefaultFOV, 0.1f);
	bool bIsPPRestored = true;

	if (UltimatePostProcessVolume)
	{
		UltimatePostProcessVolume->BlendWeight = FMath::FInterpTo(
			UltimatePostProcessVolume->BlendWeight,
			UltimateTargetPostProcessBlendWeight,
			DeltaTime,
			UltimatePostProcessInterpSpeed);

		bIsPPRestored = FMath::IsNearlyZero(UltimatePostProcessVolume->BlendWeight, 0.01f);

		if (bIsPPRestored)
		{
			UltimatePostProcessVolume->BlendWeight = 0.0f;
			UltimatePostProcessVolume->bUnbound = false;
		}
	}

	if (bIsUltimatePresentationRestoring && bIsFOVRestored && bIsPPRestored)
	{
		playerCamera->SetFieldOfView(UltimateDefaultFOV);
		bIsUltimatePresentationRestoring = false;
	}
}

void ABattlePlayerController::StartSkillCooldown(const FGameplayTag& InSkillTag, float InCooldown)
{
	if (!InSkillTag.IsValid()) return;
	if (InCooldown <= 0.0f) return;

	SkillCooldownMap.FindOrAdd(InSkillTag) = InCooldown;
	SkillCooldownRemainingMap.FindOrAdd(InSkillTag) = InCooldown;

	RefreshSkillCooldown();

	GetWorldTimerManager().SetTimer(
		SkillCooldownTimerHandle,
		this,
		&ABattlePlayerController::UpdateSkillCooldown,
		SkillCooldownTickInterval,
		true
	);
}

float ABattlePlayerController::GetSkillCooldownRemaining(const FGameplayTag& InSkillTag) const
{
	if (const float* foundRemaining = SkillCooldownRemainingMap.Find(InSkillTag))
	{
		return *foundRemaining;
	}
	return 0.0f;
}

float ABattlePlayerController::GetSkillCooldown(const FGameplayTag& InSkillTag) const
{
	if (const float* foundCooldown = SkillCooldownMap.Find(InSkillTag))
	{
		return *foundCooldown;
	}
	return 0.0f;
}

void ABattlePlayerController::UpdateSkillCooldown()
{
	TArray<FGameplayTag> expiredSkillTags;

	for (TPair<FGameplayTag, float>& skillCooldownPair : SkillCooldownRemainingMap)
	{
		if (skillCooldownPair.Value > 0.0f)
		{
			skillCooldownPair.Value = FMath::Max(0.0f, skillCooldownPair.Value - SkillCooldownTickInterval);
		}

		if (skillCooldownPair.Value <= 0.0f)
		{
			expiredSkillTags.Add(skillCooldownPair.Key);
		}
	}

	for (const FGameplayTag& expiredSkillTag : expiredSkillTags)
	{
		SkillCooldownRemainingMap.Remove(expiredSkillTag);
		SkillCooldownMap.Remove(expiredSkillTag);
	}

	RefreshSkillCooldown();

	if (SkillCooldownRemainingMap.IsEmpty())
	{
		GetWorldTimerManager().ClearTimer(SkillCooldownTimerHandle);
	}
}

void ABattlePlayerController::StartBasicAttackCooldown()
{
	if (UBattlefieldManagerSubsystem* battleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>())
	{
		const FGameplayTag skillTag = battleSubsystem->GetCurrentBasicAttackSkillTag();
		const float cooldown = battleSubsystem->GetCurrentBasicAttackCooldown();

		StartSkillCooldown(skillTag, cooldown);
	}
}

void ABattlePlayerController::StartBasicSkillCooldown()
{
	if (UBattlefieldManagerSubsystem* battleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>())
	{
		const FGameplayTag skillTag = battleSubsystem->GetCurrentBasicSkillTag();
		const float cooldown = battleSubsystem->GetCurrentBasicSkillCooldown();
		StartSkillCooldown(skillTag, cooldown);
	}
}

void ABattlePlayerController::StartUltimateCooldown()
{
	if (UBattlefieldManagerSubsystem* battleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>())
	{
		const FGameplayTag skillTag = battleSubsystem->GetLegendaryUltimateSkillTag();
		const float cooldown = battleSubsystem->GetLegendaryUltimateCooldown();
		StartSkillCooldown(skillTag, cooldown);
	}
}

void ABattlePlayerController::RefreshSkillCooldown()
{
	if (HUDWidgetInstance)
	{
		if (UBattlefieldManagerSubsystem* battleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>())
		{
			const FGameplayTag basicAttackSkillTag = battleSubsystem->GetCurrentBasicAttackSkillTag();
			const FGameplayTag basicSkillTag = battleSubsystem->GetCurrentBasicSkillTag();
			const FGameplayTag ultimateSkillTag = battleSubsystem->GetLegendaryUltimateSkillTag();

			float basicAttackPercent = 0.0f;
			const float basicAttackCooldown = GetSkillCooldown(basicAttackSkillTag);
			if (basicAttackCooldown > 0.0f)
			{
				basicAttackPercent = GetSkillCooldownRemaining(basicAttackSkillTag) / basicAttackCooldown;
			}

			float basicSkillPercent = 0.0f;
			const float basicSkillCooldown = GetSkillCooldown(basicSkillTag);
			if (basicSkillCooldown > 0.0f)
			{
				basicSkillPercent = GetSkillCooldownRemaining(basicSkillTag) / basicSkillCooldown;
			}

			float ultimatePercent = 0.0f;
			const float ultimateCooldown = GetSkillCooldown(ultimateSkillTag);
			if (ultimateCooldown > 0.0f)
			{
				ultimatePercent = GetSkillCooldownRemaining(ultimateSkillTag) / ultimateCooldown;
			}

			HUDWidgetInstance->SetBasicAttackCooldown(basicAttackPercent);
			HUDWidgetInstance->SetBasicSkillCooldown(basicSkillPercent);
			HUDWidgetInstance->SetUltimateCooldown(ultimatePercent);
		}
	}
}

void ABattlePlayerController::RefreshSkillCost()
{
	if (!HUDWidgetInstance) return;

	UBattlefieldManagerSubsystem* battleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
	if (!battleSubsystem) return;

	if (UBattleActionButtonWidget* basicAttackButton = HUDWidgetInstance->GetBasicAttack())
	{
		if (const FBattleSkillData* skillData = battleSubsystem->GetCurrentBasicAttackSkillData())
		{
			basicAttackButton->SetCostText(FText::AsNumber(FMath::RoundToInt(skillData->ManaCost)));
		}
		else
		{
			basicAttackButton->SetCostText(FText::GetEmpty());
		}
	}

	if (UBattleActionButtonWidget* basicSkillButton = HUDWidgetInstance->GetBasicSkill())
	{
		if (const FBattleSkillData* skillData = battleSubsystem->GetCurrentBasicSkillData())
		{
			basicSkillButton->SetCostText(FText::AsNumber(FMath::RoundToInt(skillData->ManaCost)));
		}
		else
		{
			basicSkillButton->SetCostText(FText::GetEmpty());
		}
	}

	if (UBattleActionButtonWidget* ultimateSkillButton = HUDWidgetInstance->GetUltimateSkill())
	{
		if (const FBattleSkillData* skillData = battleSubsystem->GetCurrentLegendarySkillData())
		{
			ultimateSkillButton->SetCostText(FText::AsNumber(FMath::RoundToInt(skillData->ManaCost)));
		}
		else
		{
			ultimateSkillButton->SetCostText(FText::GetEmpty());
		}
	}
}

void ABattlePlayerController::RefreshBattlePotion()
{
	if (!HUDWidgetInstance) return;

	UBattlefieldManagerSubsystem* battleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
	if (!battleSubsystem) return;

	TArray<FBattlePotionRuntimeSlotData>& battlePotionSlots = battleSubsystem->GetBattlePotionRuntimeSlots();

	if (battlePotionSlots.IsValidIndex(0))
	{
		HUDWidgetInstance->SetBattlePotionSlot(0, battlePotionSlots[0]);
	}
	else
	{
		HUDWidgetInstance->SetBattlePotionSlot(0, FBattlePotionRuntimeSlotData());
	}

	if (battlePotionSlots.IsValidIndex(1))
	{
		HUDWidgetInstance->SetBattlePotionSlot(1, battlePotionSlots[1]);
	}
	else
	{
		HUDWidgetInstance->SetBattlePotionSlot(1, FBattlePotionRuntimeSlotData());
	}
}

void ABattlePlayerController::UpdateBattlePotionCooldown(float InDeltaTime)
{
	if (UBattlefieldManagerSubsystem* battleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>())
	{
		TArray<FBattlePotionRuntimeSlotData>& battlePotionSlots = battleSubsystem->GetBattlePotionRuntimeSlots();

		for (FBattlePotionRuntimeSlotData& potionSlot : battlePotionSlots)
		{
			if (potionSlot.CurrentCooldown > 0.0f)
			{
				potionSlot.CurrentCooldown -= InDeltaTime;

				if (potionSlot.CurrentCooldown < 0.0f)
				{
					potionSlot.CurrentCooldown = 0.0f;
				}
			}
		}
	}

	RefreshBattlePotion();
}

void ABattlePlayerController::OnPotionMontageEnded(UAnimMontage* InMontage, bool bInterrupted)
{
	bIsPotionMontagePlaying = false;
}

bool ABattlePlayerController::TryExecuteBattleAction(EBattleActionType InActionType)
{
	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(GetPawn());

	// 궁극기 중
	if (bIsUltimateAiming)
	{
		if (InActionType != EBattleActionType::UltimateRelease)
		{
			return false;
		}
	}
	// 일반 스킬 몽타주 중
	if (playerCharacter && playerCharacter->GetIsCommonSkillMontagePlaying())
	{
		if (InActionType != EBattleActionType::ReadySpawnUnit)
		{
			return false;
		}
	}
	// 물약 몽타주 중
	if (bIsPotionMontagePlaying)
	{
		if (InActionType != EBattleActionType::Move &&
			InActionType != EBattleActionType::ReadySpawnUnit)
		{
			return false;
		}
	}
	// 스왑
	if (bIsWeaponSwapping)
	{
		return false;
	}

	return true;
}

void ABattlePlayerController::BindBuffUI()
{
	if (APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(GetPawn()))
	{
		if (UCharacterBattleStatsComponent* statComponent = playerCharacter->FindComponentByClass<UCharacterBattleStatsComponent>())
		{
			statComponent->OnBuffUpdated.RemoveAll(this);
			statComponent->OnBuffRemoved.RemoveAll(this);

			statComponent->OnBuffUpdated.AddUObject(this, &ABattlePlayerController::HandleBuffUpdated);
			statComponent->OnBuffRemoved.AddUObject(this, &ABattlePlayerController::HandleBuffRemoved);
		}
	}
}

void ABattlePlayerController::HandleBuffUpdated(const FGameplayTag& InBuffTag, float InPercent)
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->UpdateBuffSlot(InBuffTag, InPercent);
	}
}

void ABattlePlayerController::HandleBuffRemoved(const FGameplayTag& InBuffTag)
{
	if (HUDWidgetInstance)
	{
		HUDWidgetInstance->RemoveBuffSlot(InBuffTag);
	}
}
