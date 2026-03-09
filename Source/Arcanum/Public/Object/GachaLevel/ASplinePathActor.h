#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "ASplinePathActor.generated.h"

UENUM(BlueprintType)
enum class ESplinePathType : uint8
{
	ShootingStar,
	ResultStar
};

UCLASS()
class ARCANUM_API AASplinePathActor : public AActor
{
	GENERATED_BODY()
	
public:
	AASplinePathActor();

	FORCEINLINE USplineComponent* GetPathSpline() const { return PathSpline; }
	UFUNCTION(BlueprintCallable) float GetSplineLength() const;
	UFUNCTION(BlueprintCallable) FVector GetLocationAtDistance(float Distance) const;
	UFUNCTION(BlueprintCallable) FRotator GetRotationAtDistance(float Distance) const;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USplineComponent> PathSpline;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "00-Spline")
	ESplinePathType PathType = ESplinePathType::ShootingStar;
};
