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

	IntroSparkleComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Intro"));
	IntroSparkleComp->SetupAttachment(RootComponent);
	IntroSparkleComp->bAutoActivate = false;

	TrailNiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Trail"));
	TrailNiagaraComp->SetupAttachment(RootComponent);
	TrailNiagaraComp->bAutoActivate = false;

	ImpactExplosionComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Impact"));
	ImpactExplosionComp->SetupAttachment(RootComponent);
	ImpactExplosionComp->bAutoActivate = false;
}
void AShootingStarActor::BeginPlay()
{
	Super::BeginPlay();
}
// ========================================================
// Spline
// ========================================================
void AShootingStarActor::SetSplineActor(AASplinePathActor* InSpline)
{
	SplineActor = InSpline;
	Spline = InSpline ? InSpline->GetPathSpline() : nullptr;
}
// ========================================================
// Start
// ========================================================
void AShootingStarActor::StartShootingStar()
{
	CurrentDistance = 0.f;

	if (Spline) SetActorLocation(Spline->GetLocationAtDistanceAlongSpline(0.f, ESplineCoordinateSpace::World));
	if (IntroSparkleComp) IntroSparkleComp->Activate(true);
	GetWorldTimerManager().SetTimer(LaunchTimerHandle, this, &AShootingStarActor::MoveShootingStar, MoveTimerDuration, false);
}
// ========================================================
// Move Start
// ========================================================
void AShootingStarActor::MoveShootingStar()
{
	if (IntroSparkleComp) IntroSparkleComp->Deactivate();
	if (TrailNiagaraComp) TrailNiagaraComp->Activate(true);

	SetActorTickEnabled(true);
}
// ========================================================
// Tick
// ========================================================
void AShootingStarActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Spline) return;

	if (bIsShrinking) {
		SetActorScale3D(FMath::VInterpTo(GetActorScale3D(), FVector::ZeroVector, DeltaTime, ShrinkSpeed));
		return;
	}

	const float SplineLength = Spline->GetSplineLength();
	const float Remaining = SplineLength - CurrentDistance;
	float Speed = MoveSpeed;
	if (Remaining < BrakingZone) Speed *= FMath::Clamp(Remaining / BrakingZone, 0.1f, 1.f);
	CurrentDistance += Speed * DeltaTime;

	if (CurrentDistance >= SplineLength) {
		CurrentDistance = SplineLength;
		SetActorLocation(Spline->GetLocationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World));

		EndShootingStar();
		return;
	}

	SetActorLocation(Spline->GetLocationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World));
}
// ========================================================
// End
// ========================================================
void AShootingStarActor::EndShootingStar()
{
	bIsShrinking = true;
	OnShootingStarShrink.Broadcast();

	if (TrailNiagaraComp) TrailNiagaraComp->Deactivate();
	if (ImpactExplosionComp) ImpactExplosionComp->Activate(true);

	GetWorldTimerManager().SetTimer(ExplosionTimerHandle, this, &AShootingStarActor::FinishSequenceAndBroadcast, ExplosionDelay, false);
}
// ========================================================
// Finish
// ========================================================
void AShootingStarActor::FinishSequenceAndBroadcast()
{
	SetActorTickEnabled(false);
	SetActorHiddenInGame(true);

	OnShootingStarFinished.Broadcast();

	SetLifeSpan(2.0f);
}
// ========================================================
// Skip
// ========================================================
void AShootingStarActor::ForceFinish()
{
	GetWorldTimerManager().ClearTimer(LaunchTimerHandle);
	GetWorldTimerManager().ClearTimer(ExplosionTimerHandle);

	SetActorTickEnabled(false);

	if (IntroSparkleComp) IntroSparkleComp->Deactivate();
	if (TrailNiagaraComp) TrailNiagaraComp->Deactivate();
	if (ImpactExplosionComp) ImpactExplosionComp->Deactivate();

	SetActorHiddenInGame(true);
}