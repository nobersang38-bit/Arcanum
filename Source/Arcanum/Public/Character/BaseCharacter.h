#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UPlayerBattleStatsComponent;
class UCharacterBattleStatsComponent;

UCLASS(Blueprintable)
class ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

#pragma region 테스트용
public:
	/// Debug
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UPlayerBattleStatsComponent> PlayerBattleStatsComp;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCharacterBattleStatsComponent> CharacterBattleStatsComp;
#pragma endregion

	
	
public:
	ABaseCharacter();
	int a = 0;
	virtual void Tick(float DeltaSeconds) override;

	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
};

