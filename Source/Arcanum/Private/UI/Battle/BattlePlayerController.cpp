// Fill out your copyright notice in the Description page of Project Settings.


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
#include "UI/Battle/SubLayout/BattleAllyUnitSlotWidget.h"
#include "Character/BaseUnitCharacter.h"
#include "Core/SubSystem/PoolingSubsystem.h"
#include "Character/PlayerCharacter.h"

// ========================================================
// 언리얼 기본 생성
// ========================================================
void ABattlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	FString AllyUnitClassPath = TEXT("/Game/Test/Battle/Unit/BP_AllyUnit.BP_AllyUnit_C");
	UClass* LoadAllyUnit = StaticLoadClass(UObject::StaticClass(), nullptr, *AllyUnitClassPath);

	AllyUnitClass = LoadAllyUnit;

	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UInBattleHUDWidget>(this, HUDWidgetClass);
		if (HUDWidgetInstance)
		{
			HUDWidgetInstance->AddToViewport();
		}
	}
	SetupMainHUDWidget();
	SetupInputMode();

	MeatValue.BaseMax = 100.0f;
	MeatValue.Current = 100.0f;
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
		UsinAllyUnits.Empty();
		for (int i = 0; i < BattleSubsystem->GetUsingAllyUnitData().Num(); i++)
		{
			const FUnitData& UnitDataE = BattleSubsystem->GetUsingAllyUnitData()[i];
			UsinAllyUnits.Add(UnitDataE.Info.InfoSetting.Tag, UnitDataE);
		}
		//UsinAllyUnits = BattleSubsystem->GetUsingAllyUnitData();
		/*for (int i = 0; i < UsinAllyUnits.Num(); i++)
		{
			UE_LOG(LogTemp, Error, TEXT("UsinAllyUnits : %s"), *UsinAllyUnits[i].Info.InfoSetting.Tag.ToString());
		}*/
		SetAllyUsingWidget();
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
		EnhancedInputComponent->BindAction(IA_UltimateSkill, ETriggerEvent::Completed, this, &ABattlePlayerController::UltimateSkill);
		EnhancedInputComponent->BindAction(IA_Item1, ETriggerEvent::Completed, this, &ABattlePlayerController::Item1);
		EnhancedInputComponent->BindAction(IA_Item2, ETriggerEvent::Completed, this, &ABattlePlayerController::Item2);
		EnhancedInputComponent->BindAction(IA_WeaponSwap, ETriggerEvent::Completed, this, &ABattlePlayerController::WeaponSwap);
		EnhancedInputComponent->BindAction(IA_AutoManual, ETriggerEvent::Completed, this, &ABattlePlayerController::AutoManualModePC);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ABattlePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FHitResult HitResult;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
	UE_LOG(LogTemp, Error, TEXT("마우스 위치 : %.0f, %.0f, %.0f"), HitResult.ImpactPoint.X, HitResult.ImpactPoint.Y, HitResult.ImpactPoint.Z);
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
	HUDWidgetInstance->OnClickUltimateSkill.AddDynamic(this, &ABattlePlayerController::UltimateSkill);
	HUDWidgetInstance->OnClickBasicSkill.AddDynamic(this, &ABattlePlayerController::BasicSkill);
	HUDWidgetInstance->OnClickWeaponSwap.AddDynamic(this, &ABattlePlayerController::WeaponSwap);
	HUDWidgetInstance->OnClickItem1.AddDynamic(this, &ABattlePlayerController::Item1);
	HUDWidgetInstance->OnClickItem2.AddDynamic(this, &ABattlePlayerController::Item2);
	HUDWidgetInstance->OnToggleAutoManualMode.AddDynamic(this, &ABattlePlayerController::AutoManualModeMobile);
}

void ABattlePlayerController::UpdatePlayerLocationProgress()
{
	UBattlefieldManagerSubsystem* BattleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
	if (!AllyBasement.IsValid() && BattleSubsystem)
	{
		AllyBasement = BattleSubsystem->GetBasement(Arcanum::Unit::Faction::Ally::Root);
	}

	if (!EnemyBasement.IsValid() && BattleSubsystem)
	{
		EnemyBasement = BattleSubsystem->GetBasement(Arcanum::Unit::Faction::Enemy::Root);
	}

	if (!AllyBasement.IsValid() && !EnemyBasement.IsValid()) return;

	HUDWidgetInstance->SetPlayerLocationProgress(AllyBasement->GetActorLocation(), EnemyBasement->GetActorLocation(), GetPawn()->GetActorLocation());
}

void ABattlePlayerController::UpdateStageTime(int32 TimeSecond)
{
	HUDWidgetInstance->SetTime(TimeSecond);
}

void ABattlePlayerController::SetAllyUsingWidget()
{
	UBattleAllyUnitPanelWidget* PlayerInfoPanel = HUDWidgetInstance->GetPlayerInfoPanel();
	for (auto UsingAllyUnit : UsinAllyUnits)
	{
		UBattleAllyUnitSlotWidget* UnitSlot = PlayerInfoPanel->AddUnitSlot(UsingAllyUnit.Value);
		if (UnitSlot)
		{
			UnitSlot->SetUnitInfo(UsingAllyUnit.Value.Info.InfoSetting.MeatCost, UsingAllyUnit.Value.Info.InfoSetting.Icon, UsingAllyUnit.Value.Info.InfoSetting.Tag);
			UnitSlot->SetActivateCost(false);
			UnitSlot->SetCoolTimeProgress(0.0f, 0.0f);
			UnitSlot->OnPressUnitSlot.AddDynamic(this, &ABattlePlayerController::ReadySpawnUnit);
			UnitSlot->OnReleasedUnitSlot.AddDynamic(this, &ABattlePlayerController::SpawnUnit);
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
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("BasicAttack"));
	//Todo : 기본공격

	//디버그
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
	if (PlayerCharacter)
	{
		PlayerCharacter->PlayerBasicAttack();
	}
}

void ABattlePlayerController::UltimateSkill()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("UltimateSkill"));
	//Todo : 궁극기
}

void ABattlePlayerController::BasicSkill()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("BasicSkill"));
	//Todo : 기본스킬
}

void ABattlePlayerController::WeaponSwap()
{
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

void ABattlePlayerController::ReadySpawnUnit(FGameplayTag InTag)
{
	UE_LOG(LogTemp, Error, TEXT("눌렀다"));
	SpawnTag = InTag;
}

void ABattlePlayerController::SpawnUnit(FGameplayTag InTag)
{
	UE_LOG(LogTemp, Error, TEXT("땟다"));
	if (FUnitData* UnitData = UsinAllyUnits.Find(SpawnTag))
	{
		//UE_LOG(LogTemp, Error, TEXT("SpawnUnit : %s"), *SpawnTag.ToString());
		UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>();
		if (PoolingSubsystem)
		{
			FHitResult HitResult;
			GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);

			FTransform Transform;
			Transform.SetLocation(HitResult.ImpactPoint);
			AActor* EnemyUnitInstance = PoolingSubsystem->SpawnFromPool(AllyUnitClass, Transform);
			ABaseUnitCharacter* EnemyUnitCharacter = Cast<ABaseUnitCharacter>(EnemyUnitInstance);
			if (EnemyUnitCharacter)
			{
				EnemyUnitCharacter->SetUnit(*UnitData);
			}
			//UE_LOG(LogTemp, Error, TEXT("HitResult : %s"), *HitResult.ImpactPoint.ToString());
		}
	}
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

	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
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

		// 4. Pawn에 이동 입력 반영
		ControlledPawn->AddMovementInput(-ForwardDirection, MovementVector.Y); // 앞/뒤
		ControlledPawn->AddMovementInput(-RightDirection, MovementVector.X);   // 좌/우
	}
}
