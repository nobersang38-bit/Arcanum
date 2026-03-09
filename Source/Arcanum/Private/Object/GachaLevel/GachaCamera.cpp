#include "Object/GachaLevel/GachaCamera.h"

#include "Camera/CameraComponent.h"
#include "Camera/CameraShakeBase.h"

#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h"

AGachaCamera::AGachaCamera()
{
    PrimaryActorTick.bCanEverTick = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
    CameraComp->SetupAttachment(RootComponent);

    CameraComp->bUsePawnControlRotation = false;
}

void AGachaCamera::BeginPlay()
{
    Super::BeginPlay();

    if (TargetActor) LastTargetLocation = TargetActor->GetActorLocation();
}

void AGachaCamera::Tick(float DeltaTime)
{    
    Super::Tick(DeltaTime);
    if (!TargetActor) return;

    FVector CurrentTargetLoc = TargetActor->GetActorLocation();
    FVector MoveDir = (CurrentTargetLoc - LastTargetLocation).GetSafeNormal();
    if (MoveDir.IsNearlyZero()) MoveDir = TargetActor->GetActorForwardVector();

    FVector DesiredLocation = CurrentTargetLoc + (MoveDir * RelativeOffset.X) + (FVector::UpVector * RelativeOffset.Z);
    SetActorLocation(FMath::VInterpTo(GetActorLocation(), DesiredLocation, DeltaTime, FollowSpeed));
    FRotator TargetRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), CurrentTargetLoc);
    SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRot, DeltaTime, RotationSpeed));

    LastTargetLocation = CurrentTargetLoc;
}
// ========================================================
// 카메라 진동
// ========================================================
void AGachaCamera::PlayFlyingShake()
{
    if (FlyingShakeInstance) return;

    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC && PC->PlayerCameraManager && FlyingShakeClass) {
        FlyingShakeInstance = PC->PlayerCameraManager->StartCameraShake(FlyingShakeClass);
    }
}
void AGachaCamera::StopFlyingShake()
{
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC && PC->PlayerCameraManager && FlyingShakeClass) {
        PC->PlayerCameraManager->StopAllInstancesOfCameraShake(FlyingShakeClass, true);
        FlyingShakeInstance = nullptr;
        UE_LOG(LogTemp, Warning, TEXT("!!! All Flying Shakes Forced Stopped !!!"));
    }

}
void AGachaCamera::PlayImpactShake()
{
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC && PC->PlayerCameraManager && ImpactShakeClass) {
        PC->PlayerCameraManager->StartCameraShake(ImpactShakeClass, 1.0f);
    }
}