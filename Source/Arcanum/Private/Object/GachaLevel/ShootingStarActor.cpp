#include "Object/GachaLevel/ShootingStarActor.h"
#include "Object/GachaLevel/ASplinePathActor.h"

#include "NiagaraComponent.h"
#include "Components/SplineComponent.h"

AShootingStarActor::AShootingStarActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	StarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StarMesh"));
	StarMesh->SetupAttachment(RootComponent);

	StarMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StarMesh->SetCastShadow(false);

	IntroSparkleComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("IntroSparkleComp"));
	IntroSparkleComp->SetupAttachment(RootComponent);
	IntroSparkleComp->bAutoActivate = false;
	TrailNiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailNiagaraComp"));
	TrailNiagaraComp->SetupAttachment(RootComponent);
	TrailNiagaraComp->bAutoActivate = false;
	ImpactExplosionComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ImpactExplosionComp"));
	ImpactExplosionComp->SetupAttachment(RootComponent);
	ImpactExplosionComp->bAutoActivate = false;
}
void AShootingStarActor::SetSplineActor(AASplinePathActor* InSpline)
{
	SplineActor = InSpline;
	Spline = SplineActor->GetPathSpline();
}
void AShootingStarActor::BeginPlay()
{
	Super::BeginPlay();
}
void AShootingStarActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!SplineActor || bIsShrinking) {
		if (bIsShrinking) {
			FVector TargetScale = FVector::ZeroVector;
			SetActorScale3D(FMath::VInterpTo(GetActorScale3D(), TargetScale, DeltaTime, ShrinkSpeed));
		}
		return;
	}
	Spline = SplineActor->GetPathSpline();
	if (!Spline) return;

	const float SplineLength = Spline->GetSplineLength();
	const float RemainingDistance = SplineLength - CurrentDistance;
	float TargetSpeed = MoveSpeed;
	if (RemainingDistance < BrakingZone) {
		float SpeedAlpha = FMath::Clamp(RemainingDistance / BrakingZone, 0.1f, 1.0f);
		TargetSpeed *= SpeedAlpha;
	}

	CurrentDistance += TargetSpeed * DeltaTime;
	if (CurrentDistance >= SplineLength) {
		CurrentDistance = SplineLength;
		SetActorLocation(Spline->GetLocationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World));
		EndShootingStar();
		return;
	}

	FVector NewLocation = Spline->GetLocationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);
	SetActorLocation(NewLocation);
}
void AShootingStarActor::StartShootingStar()
{
	CurrentDistance = 0.0f;

	if (SplineActor && SplineActor->GetPathSpline()) {
		FVector StartPos = SplineActor->GetPathSpline()->GetLocationAtDistanceAlongSpline(0.0f, ESplineCoordinateSpace::World);
		SetActorLocation(StartPos);
	}
	if (IntroSparkleComp) IntroSparkleComp->Activate(true);

	FTimerHandle LaunchTimer;
	GetWorldTimerManager().SetTimer(LaunchTimer, this, &AShootingStarActor::MoveShootingStar, 1.0f, false);
}
void AShootingStarActor::MoveShootingStar()
{
	if (IntroSparkleComp) IntroSparkleComp->Deactivate();
	if (TrailNiagaraComp) TrailNiagaraComp->Activate(true);

	SetActorTickEnabled(true);
}
void AShootingStarActor::EndShootingStar()
{
	bIsShrinking = true;
	if (TrailNiagaraComp)	TrailNiagaraComp->Deactivate();
	if (ImpactExplosionComp)ImpactExplosionComp->Activate(true);

	FTimerHandle PostExplosionTimer;
	GetWorldTimerManager().SetTimer(PostExplosionTimer, this, &AShootingStarActor::FinishSequenceAndBroadcast, ExplosionDelay, false);
}
void AShootingStarActor::FinishSequenceAndBroadcast()
{
	SetActorTickEnabled(false);
	SetActorHiddenInGame(true);
	OnShootingStarFinished.Broadcast();
	SetLifeSpan(2.0f);
}