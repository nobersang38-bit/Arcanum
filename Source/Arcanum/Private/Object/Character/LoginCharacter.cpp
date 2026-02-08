#include "Object/Character/LoginCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Components/SceneCaptureComponent2D.h"

ALoginCharacter::ALoginCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bTickEvenWhenPaused = false;

    CaptureComp = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("PortraitCapture"));
    CaptureComp->SetupAttachment(RootComponent);
    CaptureComp->bCaptureEveryFrame = false;
    CaptureComp->bAlwaysPersistRenderingState = true;
    CaptureComp->CaptureSource = ESceneCaptureSource::SCS_SceneColorHDR;
}

void ALoginCharacter::BeginPlay()
{
    Super::BeginPlay();

    InitialLocation = GetActorLocation();
    InitialRotation = GetActorRotation();

    SetActorHiddenInGame(true);
    bHasAppeared = false;
}

void ALoginCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ALoginCharacter::AppearCharacter(bool bRequiredTick)
{
    if (bHasAppeared) return;
    SetActorHiddenInGame(false);

    if (CaptureComp) {
        CaptureComp->bCaptureEveryFrame = true;
        CaptureComp->CaptureScene();
    }
}

void ALoginCharacter::ResetCharacter()
{
    SetActorLocation(InitialLocation);
    SetActorRotation(InitialRotation);
    SetActorHiddenInGame(true);
    bHasAppeared = false;

    if (CaptureComp) {
        CaptureComp->bCaptureEveryFrame = false;
        CaptureComp->CaptureScene();
    }
}

void ALoginCharacter::LookAtCamera()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (!PC || !PC->PlayerCameraManager) return;

    FVector CameraLocation = PC->PlayerCameraManager->GetCameraLocation();
    FVector Direction = CameraLocation - GetActorLocation();
    FRotator NewRotation = Direction.Rotation();

    SetActorRotation(FRotator(GetActorRotation().Pitch, NewRotation.Yaw, GetActorRotation().Roll));
}