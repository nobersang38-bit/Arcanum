#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "Interface/PoolingInterface.h"
#include "SkillActor.generated.h"

class USkillBase;

UCLASS(Abstract)
class ARCANUM_API ASkillActor : public AActor, public IPoolingInterface
{
    GENERATED_BODY()

public:
    ASkillActor();

    /** 풀에서 꺼냈을 때 초기화 */
    UFUNCTION(BlueprintCallable)
    virtual void ActivateSkillActor(
        USkillBase* InSkill,
        AActor* InOwner,
        const FVector& SpawnLocation,
        const FRotator& SpawnRotation);

    /** 스킬 종료 (히트 후, 지속시간 끝 등) */
    virtual void DeactivateSkillActor();

protected:
    virtual void BeginPlay() override;

    /** 이 액터를 생성한 스킬 UObject */
    UPROPERTY()
    TObjectPtr<USkillBase> OwnerSkill;

    /** 시전자 */
    UPROPERTY()
    TWeakObjectPtr<AActor> InstigatorActor;

    /** 현재 활성화 여부 (풀링용) */
    UPROPERTY(VisibleAnywhere)
    bool bIsActive = false;

    /** 풀로 되돌릴 때 호출 */
    virtual void ReturnToPool();

    void ActivateItem() override;
    void DeactiveItem() override;

public:
    bool IsActive() const { return bIsActive; }
};