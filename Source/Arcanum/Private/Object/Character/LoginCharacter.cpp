#include "Object/Character/LoginCharacter.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

ALoginCharacter::ALoginCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // Burst Effect 생성
    AppearEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("AppearEffect"));
    AppearEffect->SetupAttachment(GetRootComponent());
    AppearEffect->SetAutoActivate(false);

    // Loop Effect 생성
    HoloLoopEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("HoloLoopEffect"));
    HoloLoopEffect->SetupAttachment(GetRootComponent());
    HoloLoopEffect->SetAutoActivate(false);
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

void ALoginCharacter::PlayAppearEffect(bool bRequiredTick)
{
    if (bHasAppeared) return;
    if (bRequiredTick) SetActorTickEnabled(true);

    SetActorHiddenInGame(false);

    if (AppearEffectTemplate) {
        AppearEffect->SetAsset(AppearEffectTemplate);
        AppearEffect->Activate(true);
    }

    bHasAppeared = true;
    StartHoloLoop();
}

void ALoginCharacter::StartHoloLoop()
{
    if (HoloLoopTemplate) {
        HoloLoopEffect->SetAsset(HoloLoopTemplate);
        HoloLoopEffect->Activate(true);
    }
}
void ALoginCharacter::StopHoloLoop()
{
    if (HoloLoopEffect) HoloLoopEffect->Deactivate();
}

void ALoginCharacter::ResetCharacter()
{
    SetActorLocation(InitialLocation);
    SetActorRotation(InitialRotation);
    SetActorHiddenInGame(true);
    bHasAppeared = false;

    if (AppearEffect) AppearEffect->Deactivate();
    if (HoloLoopEffect) HoloLoopEffect->Deactivate();
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