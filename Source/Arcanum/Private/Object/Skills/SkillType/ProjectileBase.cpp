#include "Object/Skills/SkillType/ProjectileBase.h"
#include "Object/Skills/SkillBase.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/StatModifierInterface.h"
#include "Interface/StatInterface.h"
#include "Component/Stats/CharacterBattleStatsComponent.h"
#include "Interface/TeamInterface.h"
#include "Engine/OverlapResult.h"

AProjectileBase::AProjectileBase()
{
    PrimaryActorTick.bCanEverTick = true;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("Collision");
    RootComponent = CollisionComponent;

    CollisionComponent->InitSphereRadius(20.f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    CollisionComponent->SetCollisionProfileName("BlockAll");

    CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnOverlap);
}

void AProjectileBase::BeginPlay()
{
    Super::BeginPlay();
}
void AProjectileBase::Tick(float Deltatime)
{
    Super::Tick(Deltatime);

    if (bIsActive)
    {
        FVector ResultVelocity;
        switch (ProjectileMode)
        {
        case EProjectileMode::Straight:
            ResultVelocity = GetActorForwardVector() * InitialSpeed;
            SetActorLocation(GetActorLocation() + (ResultVelocity * Deltatime));
            break;

        case EProjectileMode::Horming:
        {
            FRotator HormingDirection;
            if (TargetActor.IsValid())
            {
                HormingDirection = (TargetActor->GetActorLocation() - GetActorLocation()).Rotation();
            }
            else
            {
                HormingDirection = (TargetLocation - GetActorLocation()).Rotation();
            }

            FRotator ResultDirection = FMath::RInterpConstantTo(GetActorRotation(), HormingDirection, Deltatime, HormingSpeed);
            SetActorRotation(ResultDirection);
            ResultVelocity = GetActorForwardVector() * InitialSpeed;
            SetActorLocation(GetActorLocation() + (ResultVelocity * Deltatime));
        }
        break;

        case EProjectileMode::Howitzer:
            FVector TargetLocationIn;
            /*if (TargetActor.IsValid())
            {
                TargetLocationIn = TargetActor->GetActorLocation();
            }
            else*/
            {
                TargetLocationIn = TargetLocation;
            }
            float TotalLength2D = (FVector2D(HowitzerStartTransform.GetLocation()) - FVector2D(TargetLocationIn)).Length();
            float CurrentLenth2D = (FVector2D(GetActorLocation()) - FVector2D(TargetLocationIn)).Length();
            float DistanceNormalize = 1.0f - (CurrentLenth2D / TotalLength2D);
            DistanceNormalize = FMath::Clamp(DistanceNormalize, 0.0f, 1.0f);
            float HeightValue = (HowitzerHeightCurve.EditorCurveData.Eval(DistanceNormalize) * HowitzerHeight) + HowitzerStartTransform.GetLocation().Z;

            float SpeedValue = HowitzerSpeedCurve.EditorCurveData.Eval(DistanceNormalize);
            SpeedValue = (SpeedValue * InitialSpeed) * (TotalLength2D * 0.001f);

            FVector Direction = TargetLocationIn - HowitzerStartTransform.GetLocation();
            Direction = FVector(Direction).GetSafeNormal2D();
            FVector CurrentLocation2D = GetActorLocation();
            FVector ResultLocation = FVector(CurrentLocation2D.X, CurrentLocation2D.Y, 0.0f) + (HeightValue * FVector::UpVector) + (Direction * SpeedValue * Deltatime);

            FRotator ResultRotator = (ResultLocation - CurrentLocation2D).Rotation();
            SetActorRotation(ResultRotator);
            SetActorLocation(ResultLocation);

            float CurrentTotalLength2D = (FVector2D(HowitzerStartTransform.GetLocation()) - FVector2D(GetActorLocation())).Length();
            if (DistanceNormalize >= 1.0f || CurrentTotalLength2D >= TotalLength2D)
            {
                DeactivateSkillActor();
            }
            HowitzerTime += Deltatime;
            break;
        }
    }

}
void AProjectileBase::DeactivateSkillActor()
{
    GetWorld()->GetTimerManager().ClearTimer(LifeTimerHandle);
    GetWorld()->GetTimerManager().ClearTimer(DeactiveDelayTimerHandle);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    bIsActive = false;
    SetActorHiddenInGame(true);
    SetActorEnableCollision(false);
    DeactiveItem();
    Super::DeactivateSkillActor();
}
void AProjectileBase::ActivateSkillActor(USkillBase* InSkill, AActor* InOwner, const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
    Super::ActivateSkillActor(InSkill, InOwner, SpawnLocation, SpawnRotation);
    if (bIsFirstStart)
    {
        bIsFirstStart = false;
        FirstCollisionEnabled = CollisionComponent->GetCollisionEnabled();
    }
    CollisionComponent->OnComponentHit.RemoveDynamic(this, &AProjectileBase::OnHit);
    CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);

    CollisionComponent->OnComponentBeginOverlap.RemoveDynamic(this, &AProjectileBase::OnOverlap);
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnOverlap);

    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

    if (ProjectileMode == EProjectileMode::Howitzer)
    {
        HowitzerTime = 0.0f;
        HowitzerStartTransform = GetActorTransform();
    }
    CollisionComponent->SetCollisionEnabled(FirstCollisionEnabled);

    GetWorld()->GetTimerManager().SetTimer(LifeTimerHandle, this, &AProjectileBase::DeactivateSkillActor, LifeTime, false);
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!bIsActive) return;
    if (!TargetfilterCheck(OtherActor)) return;

    switch (CollisionMode)
    {
    case ECollisionMode::Immediately:
        CollisionProcess(OtherActor);
        DeactivateDelay(DeactivationDealay);
        break;
    case ECollisionMode::ActivateOnCollision:
        ActivateOnCollisionProcess(OtherActor);
        DeactivateDelay(DeactivationDealay);
        break;
    case ECollisionMode::Infinity:
        CollisionProcess(OtherActor);
        break;
    default:
        break;
    }
}

void AProjectileBase::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!bIsActive) return;
    if (!TargetfilterCheck(OtherActor)) return;

    switch (CollisionMode)
    {
    case ECollisionMode::Immediately:
        CollisionProcess(OtherActor);
        DeactivateDelay(DeactivationDealay);
        break;
    case ECollisionMode::ActivateOnCollision:
        ActivateOnCollisionProcess(OtherActor);
        DeactivateDelay(DeactivationDealay);
        break;
    case ECollisionMode::Infinity:
        CollisionProcess(OtherActor);
        break;
    default:
        break;
    }
}

void AProjectileBase::CollisionProcess(AActor* OtherActor)
{
    if (OwnerSkill)
    {
        if (const FLevelModifierEntry* LevelModifierEntry = OwnerSkill->GetCurrentLevelEntry())
        {

            /*if (InstigatorActor->GetClass()->ImplementsInterface(UStatModifierInterface::StaticClass()))
            {
                auto Interface = Cast<IStatModifierInterface>(InstigatorActor);
                Interface->AddLevelModifierEntry(*LevelModifierEntry);
            }*/

            if (OtherActor->GetClass()->ImplementsInterface(UStatModifierInterface::StaticClass()))
            {
                auto Interface = Cast<IStatModifierInterface>(OtherActor);
                if (LevelModifierEntry->OtherCharacterModifiers.Num() > 0)
                {
                    // 계산 부분
                    FDerivedStatModifier StatModifier = LevelModifierEntry->OtherCharacterModifiers[0];

                    if (OwnerSkill->GetDerivedStatModifier().StatTag.MatchesTag(StatModifier.StatTag))
                    {
                        StatModifier.Value.Flat = StatModifier.Value.Flat + OwnerSkill->GetDerivedStatModifier().Value.Flat;
                    }

                    if (StatModifier.Duration <= 0.0f && !StatModifier.bIsPermanent) // 체인지 스탯함수 실행
                    {
                        Interface->ChangeStat(StatModifier.StatTag, (StatModifier.Value.Flat * StatModifier.Value.Mul));
                    }
                    else // 모디파이어 추가
                    {
                        Interface->AddDerivedStatModifier(StatModifier);
                    }
                }
            }
        }
    }
}

void AProjectileBase::ActivateOnCollisionProcess(AActor* OtherActor)
{
    TArray<FOverlapResult> OverlapResults;
    FCollisionShape CollisionShape;
    CollisionShape.MakeSphere(SphereCollisionRadius);
    GetWorld()->OverlapMultiByChannel(OverlapResults, GetActorLocation(), FQuat::Identity, ECollisionChannel::ECC_Visibility, CollisionShape);

    for (int i = 0; i < OverlapResults.Num(); i++)
    {
        if (!bIsActive) continue;
        if (!OverlapResults[i].GetActor()) continue;
        if (!TargetfilterCheck(OverlapResults[i].GetActor())) continue;
        CollisionProcess(OverlapResults[i].GetActor());
    }
}

bool AProjectileBase::TargetfilterCheck(AActor* OtherActor)
{
    if (OwnerSkill)
    {
        if (InstigatorActor->GetClass()->ImplementsInterface(UTeamInterface::StaticClass()))
        {
            if (OtherActor->GetClass()->ImplementsInterface(UTeamInterface::StaticClass()))
            {
                auto OtherActorInterface = Cast<ITeamInterface>(OtherActor);
                auto InstigatorActorInterface = Cast<ITeamInterface>(InstigatorActor);

                FGameplayTag OtherActorTag = OtherActorInterface->GetTeamTag();
                FGameplayTag InstigatorTag = InstigatorActorInterface->GetTeamTag();

                const FSkillInfo& SkillInfo = OwnerSkill->GetSkillInfo();
                if (SkillInfo.TargetFilterTag == AllyTag)
                {
                    if (InstigatorTag != OtherActorTag) return false;
                }
                else if (SkillInfo.TargetFilterTag == EnemyTag)
                {
                    if (InstigatorTag == OtherActorTag) return false;
                }
                else if (SkillInfo.TargetFilterTag == SelfTag)
                {
                    if (OtherActor != InstigatorActor) return false;
                }
                else if (SkillInfo.TargetFilterTag == NoneTag)
                {
                    return false;
                }
            }
        }
    }

    return true;
}

void AProjectileBase::DeactivateDelay(float InDelay)
{
    if (InDelay <= 0.0f)
    {
        DeactivateSkillActor();
        return;
    }
    FTimerDelegate TimerDelegate;
    TimerDelegate.BindWeakLambda(this, [this]()
        {
            DeactivateSkillActor();
        });
    GetWorld()->GetTimerManager().ClearTimer(LifeTimerHandle);
    GetWorld()->GetTimerManager().ClearTimer(DeactiveDelayTimerHandle);
    GetWorld()->GetTimerManager().SetTimer(DeactiveDelayTimerHandle, TimerDelegate, InDelay, false, InDelay);
}
