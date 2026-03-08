#include "Object/GachaLevel/ASplinePathActor.h"

AASplinePathActor::AASplinePathActor()
{
	PrimaryActorTick.bCanEverTick = false;

	PathSpline = CreateDefaultSubobject<USplineComponent>(TEXT("PathSpline"));
	RootComponent = PathSpline;
}
float AASplinePathActor::GetSplineLength() const
{
	if (!PathSpline) return 0.f;
	return PathSpline->GetSplineLength();
}
FVector AASplinePathActor::GetLocationAtDistance(float Distance) const
{
	if (!PathSpline) return FVector::ZeroVector;
	return PathSpline->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
}
FRotator AASplinePathActor::GetRotationAtDistance(float Distance) const
{
	if (!PathSpline) return FRotator::ZeroRotator;
	return PathSpline->GetRotationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
}