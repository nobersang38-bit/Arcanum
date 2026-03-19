#pragma once

#include "CoreMinimal.h"
#include "Object/Skills/SkillActor.h"
#include "MeleeBase.generated.h"

class UBoxComponent;
class USkillBase;
class UPrimitiveComponent;

/**
 * 추영호
 * 근접 베이스
 */

UCLASS()
class ARCANUM_API AMeleeBase : public ASkillActor
{
	GENERATED_BODY()

public:
	AMeleeBase();

public:
	virtual void ActivateSkillActor(USkillBase* InSkill, AActor* InOwner, const FVector& SpawnLocation, const FRotator& SpawnRotation) override;

	virtual void DeactivateSkillActor() override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

protected:
	/* 근접 판정 박스 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Melee")
	TObjectPtr<UBoxComponent> HitBoxComponent;

	/* 전방 오프셋 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee")
	FVector ForwardOffset = FVector(100.f, 0.f, 0.f);

	/* 판정 박스 크기 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee")
	FVector BoxExtent = FVector(100.f, 100.f, 100.f);

	/* 중복 타격 방지 */
	UPROPERTY()
	TSet<TWeakObjectPtr<AActor>> HitActors;

	FTimerHandle ActiveTimerHandle;
};