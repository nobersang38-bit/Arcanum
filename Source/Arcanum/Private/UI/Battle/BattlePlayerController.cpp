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


// ========================================================
// 언리얼 기본 생성
// ========================================================
void ABattlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	/*FString AllyUnitClassPath = TEXT("/Game/Test/Battle/Unit/BP_AllyUnit.BP_AllyUnit_C");
	UClass* LoadAllyUnit = StaticLoadClass(UObject::StaticClass(), nullptr, *AllyUnitClassPath);

	UnitClass = LoadAllyUnit;*/

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

	MeatValue.BaseMax = 1000000.0f;
	MeatValue.Current = 1000000.0f;
	MeatValue.BaseTick = 2.0f;

	ManaValue.BaseMax = 100.0f;
	ManaValue.Current = 100.0f;
	ManaValue.BaseTick = 2.0f;

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
	UBattlefieldManagerSubsystem* BattleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();

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
	CoolTimeDelegate.BindUObject(this, &ABattlePlayerController::Internal_UnitsCoolTimeTick, CoolTImeTickInterval);
	GetWorld()->GetTimerManager().ClearTimer(CoolTimeTimer);
	GetWorld()->GetTimerManager().SetTimer(CoolTimeTimer, CoolTimeDelegate, CoolTImeTickInterval, true);

	SetBossHealthProgress(0.0f, 0.0f);

	GetWorld()->GetTimerManager().ClearTimer(PlayerLocationProgressTimeHandle);
	GetWorld()->GetTimerManager().SetTimer(PlayerLocationProgressTimeHandle, this, &ABattlePlayerController::UpdatePlayerLocationProgress, PlayerLocationProgressUpdateInterval, true, 0.0f);
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
		EnhancedInputComponent->BindAction(IA_BasicAttack, ETriggerEvent::Completed, this, &ABattlePlayerController::BasicAttack);
		EnhancedInputComponent->BindAction(IA_BasicSkill, ETriggerEvent::Completed, this, &ABattlePlayerController::BasicSkill);
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
					}
				}
			}
		}
	}
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

void ABattlePlayerController::DebugBossHealthBar(float CurrentHealth, float MaxHealth)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("DebugBossHealthBar"));
	HUDWidgetInstance->SetBossHealthBarProgress(CurrentHealth, MaxHealth);
}

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


// ========================================================
// 메인HUD
// ========================================================
void ABattlePlayerController::SetupMainHUDWidget()
{
	HUDWidgetInstance->OnClickBasicAttack.AddDynamic(this, &ABattlePlayerController::BasicAttack);
	HUDWidgetInstance->OnPressedUltimateSkill.AddDynamic(this, &ABattlePlayerController::UltimateSkillPressed);
	HUDWidgetInstance->OnReleasedUltimateSkill.AddDynamic(this, &ABattlePlayerController::UltimateSkillReleased);
	HUDWidgetInstance->OnClickBasicSkill.AddDynamic(this, &ABattlePlayerController::BasicSkill);
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
	if (UBattleAllyUnitPanelWidget* PlayerInfoPanel = HUDWidgetInstance->GetPlayerInfoPanel())
	{
		FRegenStat& InData = ManaValue;
		InData.Current += DeltaTime * InData.BaseTick;
		InData.Current = FMath::Clamp(InData.Current, 0.0f, InData.BaseMax);
		PlayerInfoPanel->SetManaCostProgress(InData.Current, InData.BaseMax);
	}
}

void ABattlePlayerController::SetPlayerHealthProgress(float CurrentHealth, float MaxHealth)
{
	HUDWidgetInstance->SetPlayerCharacterHealthBarProgress(CurrentHealth, MaxHealth);
}

void ABattlePlayerController::SetBossHealthProgress(float CurrentHealth, float MaxHealth)
{
	HUDWidgetInstance->SetBossHealthBarProgress(CurrentHealth, MaxHealth);
}

// ========================================================
// 메인
// ========================================================
void ABattlePlayerController::BasicAttack()
{
	if (UBattlefieldManagerSubsystem* battleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>())
	{
		const FGameplayTag skillTag = battleSubsystem->GetCurrentBasicAttackSkillTag();
		const int32 skillLevel = battleSubsystem->GetCurrentBasicAttackSkillLevel();

		UE_LOG(LogTemp, Warning, TEXT("BasicAttack Tag=%s Level=%d"), *skillTag.ToString(), skillLevel);
	}

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("BasicAttack"));
	//Todo : 기본공격

	//디버그
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (PlayerCharacter)
	{
		PlayerCharacter->PlayerBasicAttack();
	}
}

void ABattlePlayerController::BasicSkill()
{
	if (UBattlefieldManagerSubsystem* battleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>())
	{
		const FGameplayTag skillTag = battleSubsystem->GetCurrentBasicSkillTag();
		const int32 skillLevel = battleSubsystem->GetCurrentBasicSkillLevel();

		UE_LOG(LogTemp, Warning, TEXT("BasicSkill Tag=%s Level=%d"), *skillTag.ToString(), skillLevel);
	}

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("BasicSkill"));
	//Todo : 기본스킬
}

//void ABattlePlayerController::UltimateSkill()
//{
//	if (UBattlefieldManagerSubsystem* battleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>())
//	{
//		const FGameplayTag skillTag = battleSubsystem->GetLegendaryUltimateSkillTag();
//		const int32 skillLevel = battleSubsystem->GetLegendaryUltimateSkillLevel();
//
//		UE_LOG(LogTemp, Warning, TEXT("UltimateSkill Tag=%s Level=%d"), *skillTag.ToString(), skillLevel);
//	}
//
//	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("UltimateSkill"));
//	//Todo : 궁극기
//}

void ABattlePlayerController::WeaponSwap()
{
	if (UBattlefieldManagerSubsystem* battleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>())
	{
		const FGameplayTag currentSlotTag = battleSubsystem->GetCurrentWeaponSlotTag();

		if (currentSlotTag == Arcanum::Items::ItemSlot::Weapon::Slot1)
		{
			battleSubsystem->SetCurrentWeaponSlotTag(Arcanum::Items::ItemSlot::Weapon::Slot2);
		}
		else if (currentSlotTag == Arcanum::Items::ItemSlot::Weapon::Slot2)
		{
			battleSubsystem->SetCurrentWeaponSlotTag(Arcanum::Items::ItemSlot::Weapon::Slot1);
		}

		UE_LOG(LogTemp, Warning, TEXT("WeaponSwap CurrentSlot=%s"), *battleSubsystem->GetCurrentWeaponSlotTag().ToString());

		APlayerCharacter* playerCharacter = Cast<APlayerCharacter>(GetPawn());
		if (playerCharacter)
		{
			playerCharacter->UpdateEquippedWeaponMesh();
		}
	}

	UBattlefieldManagerSubsystem* battleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
	if (battleSubsystem && HUDWidgetInstance)
	{
		HUDWidgetInstance->RefreshWeaponSkillIcons(
			battleSubsystem->GetCurrentWeaponIcon(),
			battleSubsystem->GetCurrentBasicSkillIcon(),
			battleSubsystem->GetLegendaryWeaponIcon());
	}

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("WeaponSwap"));
	//Todo : 무기스왑
}

void ABattlePlayerController::Item1()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Item1"));
	//Todo : 아이템1
}

void ABattlePlayerController::Item2()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Item2"));
	//Todo : 아이템2
}

void ABattlePlayerController::AutoManualModeMobile(bool bIsChecked)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("AutoManualMode"));
	bIsAutoManual = bIsChecked;
	//Todo : 수동,자동 전투
}

void ABattlePlayerController::AutoManualModePC()
{
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
}

void ABattlePlayerController::CommonButton()
{
	Internal_SpawnUnit();
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

ABaseUnitCharacter* ABattlePlayerController::Internal_SpawnUnit()
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
				Transform.SetLocation(ResultLocation);
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
	if (ManaValue.Current >= Value)
	{
		ManaValue.Current -= Value;
		ManaValue.Current = FMath::Clamp(ManaValue.Current, 0.0f, ManaValue.BaseMax);
		return true;
	}

	return false;
}

bool ABattlePlayerController::UseCoolTime(FGameplayTag InTag)
{
	FUnitInfoSetting* UnitData = UsingAllyUnits.Find(InTag);
	if (!UnitData) return false;

	UnitData->CurrentCoolTime = UnitData->CoolTime;

	/*FTimerDelegate CoolTimeDelegate;
	float TickInterval = 0.01f;
	CoolTimeDelegate.BindUObject(this, &ABattlePlayerController::Internal_UnitCoolTimeTick, UnitData->Tag, TickInterval);

	FTimerHandle* CoolTimeHandle = nullptr;
	if (FTimerHandle* InternalCoolTimeHandle = CoolTimeHandles.Find(InTag))
	{
		CoolTimeHandle = InternalCoolTimeHandle;
	}
	else
	{
		CoolTimeHandle = &CoolTimeHandles.Add(InTag, FTimerHandle());
	}

	if (CoolTimeHandle)
	{
		GetWorld()->GetTimerManager().ClearTimer(*CoolTimeHandle);
		GetWorld()->GetTimerManager().SetTimer(*CoolTimeHandle, CoolTimeDelegate, TickInterval, true);
		return true;
	}
	else
	{
		return false;
	}*/
	return true;
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
				float Result = static_cast<float>(MatchData.EndTimeSecond) / static_cast<float>(BattleSubsystem->GetInBattleData().BattleStageInfo.StageLimitTime);

				Result *= 3.0f;
				UE_LOG(LogTemp, Error, TEXT("Result : %f"), Result);
				BattleEndWidget->SetStar(FMath::Clamp<int32>(FMath::RoundToInt(Result), 0, 3));
			}
			else
			{
				BattleEndWidget->SetStar(3);
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

void ABattlePlayerController::Internal_UnitsCoolTimeTick(float DeltaTime)
{
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
		// 2. 컨트롤러의 회전 방향을 가져와서 Yaw(좌우 회전) 값만 추출
		const FRotator Rotation = GetControlRotation();
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
			ControlledPawn->AddMovementInput(-ForwardDirection, MovementVector.Y);
			ControlledPawn->AddMovementInput(-RightDirection, MovementVector.X);
		}
	}
}

// ========================================================
// 궁극기
// ========================================================
void ABattlePlayerController::UltimateSkillEnd()
{
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
}

void ABattlePlayerController::UltimateSkillPressed()
{
	if (bIsUltimateAiming) return;

	UBattlefieldManagerSubsystem* battleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
	if (!battleSubsystem) return;

	const FGameplayTag skillTag = battleSubsystem->GetLegendaryUltimateSkillTag();
	if (!skillTag.IsValid()) return;

	const float castTime = battleSubsystem->GetInBattleData().BattleWeaponSkill.LegendaryUltimateSkill.CastTime;

	bIsUltimateAiming = true;

	battleSubsystem->BeginLegendaryWeaponMode();

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
	if (bIsUltimateAiming)
	{
		ExecuteUltimateSkill();
	}
}

void ABattlePlayerController::ExecuteUltimateSkill()
{
	if (bIsUltimateAiming)
	{
		bIsUltimateAiming = false;

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

		// Todo : 현재 조준 위치로 궁극기 실제 발사

		UltimateSkillEnd();
	}
}