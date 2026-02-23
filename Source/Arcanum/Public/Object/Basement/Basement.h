#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Basement.generated.h"

class UPlayerBattleStatsComponent;


/* DT구조
   1. 캐릭터(BP)
   2. 비용/적 캐릭터인 경우, 몇초마다 소환할 껀지(Cost/float)
   3. 해당 캐릭터가 있다면, TArray<FRegenStat> 얼마나 줄껀지.
*/

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBasementDestroyed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBasementHealthChanged, float, CurrentBasementHealth, float, MaxBasementHealth);


class UCapsuleComponent;
class UStaticMeshComponent;

UCLASS()
class ARCANUM_API ABasement : public AActor
{
	GENERATED_BODY()
	
#pragma region 델리게이트
public:
	UPROPERTY(BlueprintAssignable)
	FOnBasementDestroyed OnBasementDestroyed;
#pragma endregion

	
public:
	ABasement();
	FOnBasementHealthChanged OnBasementHealthChanged;
	
protected:
	virtual void BeginPlay() override;

	/** True면 내 기지, false면 적 기지*/
	UPROPERTY(EditAnywhere, Category = "Basement")
	bool bIsFriendly = true;
	/** 적 기지인 경우, 0.8s인 경우도 있을 수 있으므로,*/
	UPROPERTY(EditAnywhere, Category = "Spawn")
	float EnemySpawnCheckInterval = 0.1f;

	/** 돌릴 타이머*/
	FTimerHandle EnemySpawnTimerHandle;

	/** 기지 체력 */
	UPROPERTY(EditAnywhere, Category = "Health")
	float CurrentBasementHealth;

	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxBasementHealth;

protected: 
	/** 적 기지인 경우 해당 시간 마다 적 생성용 타이머 함수*/
	void EnemySpawnTick();

	void TrySpawnEnemyUnit();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UPlayerBattleStatsComponent> BattleStatsComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Basement|Spawn")
	TArray<TSubclassOf<APawn>> SpawnableUnits;

	UPROPERTY(EditAnywhere, Category = "Spawning|Common")
	float SpawnCommonInterval = 3.0f;

	UPROPERTY(EditAnywhere, Category = "Spawning|Common")
	TSubclassOf<ACharacter> CommonEnemyClass;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	FVector SpawnLocation = FVector(-129.0f, 2704.0f, 100.0f);

	FTimerHandle SpawnTimerHandle;

	int32 SpawnCommonCount = 0;
	int32 MaxSpawnCommonCount = 4;

public: // ===== Behavior =====
	UFUNCTION(BlueprintCallable)
	void TakeDamageToBasement(float Damage);

	UFUNCTION(BlueprintCallable)
	bool SpawnUnit(int32 Index);

	UFUNCTION(BlueprintCallable)
	void AddResource(int32 Amount);

	UFUNCTION(BlueprintCallable)
	bool ConsumeResource(int32 Amount);

	UFUNCTION(BlueprintPure)
	bool IsBaseDestroyed() const;

protected:
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	TObjectPtr<UCapsuleComponent> CapsuleComponent = nullptr;*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> MeshComponent = nullptr;
};