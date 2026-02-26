// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Battle/BattlePlayerController.h"
#include "Blueprint/UserWidget.h"
#include "UI/Battle/Contents/InBattleHUDWidget.h"
#include "UI/Battle/SubLayout/BattleAllyUnitPanelWidget.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"

// ========================================================
// 언리얼 기본 생성
// ========================================================
void ABattlePlayerController::BeginPlay()
{
	Super::BeginPlay();

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
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ABattlePlayerController::Move);
		//EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &ABattlePlayerController::OnInputStarted);
		//EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &ABattlePlayerController::OnSetDestinationTriggered);
		//EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &ABattlePlayerController::OnSetDestinationReleased);
		//EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &ABattlePlayerController::OnSetDestinationReleased);

		//EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &ABattlePlayerController::OnInputStarted);
		//EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &ABattlePlayerController::OnTouchTriggered);
		//EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &ABattlePlayerController::OnTouchReleased);
		//EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &ABattlePlayerController::OnTouchReleased);

		//EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AArcanumPlayerController::OnMove);
		//EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Completed, this, &AArcanumPlayerController::OnMove);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
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
	HUDWidgetInstance->GetPlayerInfoPanel()->AddUnitSlot();
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
	HUDWidgetInstance->OnClickBasicAttack.AddDynamic(this, &ABattlePlayerController::ClickBasicAttack);
	HUDWidgetInstance->OnClickUltimateSkill.AddDynamic(this, &ABattlePlayerController::ClickUltimateSkill);
	HUDWidgetInstance->OnClickBasicSkill.AddDynamic(this, &ABattlePlayerController::ClickBasicSkill);
	HUDWidgetInstance->OnClickWeaponSwap.AddDynamic(this, &ABattlePlayerController::ClickWeaponSwap);
	HUDWidgetInstance->OnClickItem1.AddDynamic(this, &ABattlePlayerController::ClickItem1);
	HUDWidgetInstance->OnClickItem2.AddDynamic(this, &ABattlePlayerController::ClickItem2);
	HUDWidgetInstance->OnToggleAutoManualMode.AddDynamic(this, &ABattlePlayerController::ToggleAutoManualMode);
}

void ABattlePlayerController::ClickBasicAttack()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("ClickBasicAttack"));
	//Todo : 기본공격 버튼
}

void ABattlePlayerController::ClickUltimateSkill()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("ClickUltimateSkill"));
	//Todo : 궁극기 버튼
}

void ABattlePlayerController::ClickBasicSkill()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("ClickBasicSkill"));
	//Todo : 기본스킬 버튼
}

void ABattlePlayerController::ClickWeaponSwap()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("ClickWeaponSwap"));
	//Todo : 무기스왑 버튼
}

void ABattlePlayerController::ClickItem1()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("ClickItem1"));
	//Todo : 아이템1 버튼
}

void ABattlePlayerController::ClickItem2()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("ClickItem2"));
	//Todo : 아이템2 버튼
}

void ABattlePlayerController::ToggleAutoManualMode(bool bIsChecked)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("ToggleAutoManualMode"));
	//Todo : 수동,자동 전투 버튼
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
	SetInputMode(InputMode);
}


// ========================================================
// 입력 관련
// ========================================================
void ABattlePlayerController::Move(const FInputActionValue& InputValue)
{
	UE_LOG(LogTemp, Warning, TEXT("무야호야무야호"));
}