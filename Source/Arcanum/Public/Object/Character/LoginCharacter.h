#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LoginCharacter.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

/*
 * Version : 1.0.0.0 2026/02/04
 * 클래스 역할 :
 *  1. 로그인 화면에서 보여줄 메인 캐릭터 한마리
 *  2. PlayAppearEffect() 호출 시 나이아가라로 일부 지지직 연출 후 Visible
 */

UCLASS()
class ARCANUM_API ALoginCharacter : public ACharacter
{
	GENERATED_BODY()

	public:
	ALoginCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

    // ============================================
    // Components
    // ============================================
    /** 등장 시 Burst 효과 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
    TObjectPtr<UNiagaraComponent> AppearEffect;

    /** 지속 홀로그램 루프 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
    TObjectPtr<UNiagaraComponent> HoloLoopEffect;

    // ============================================
    // Niagara Assets
    // ============================================
    /** 등장 나이아가라 시스템 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
    UNiagaraSystem* AppearEffectTemplate;

    /** 루핑 홀로그램 나이아가라 시스템 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
    UNiagaraSystem* HoloLoopTemplate;

    // ============================================
    // Appearance State
    // ============================================
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance")
    bool bHasAppeared = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance")
    float AppearDuration = 1.0f;

    FVector InitialLocation;
    FRotator InitialRotation;

    // ============================================
    // Functions
    // ============================================
public:
    float GetAppearDuration() { return AppearDuration; }

    /** Burst 등장 효과 + Visible */
    UFUNCTION(BlueprintCallable, Category = "Appearance")
    void PlayAppearEffect(bool bRequiredTick = false);
    
    /** Looping 홀로그램 시작 */
    UFUNCTION(BlueprintCallable, Category = "Appearance")
    void StartHoloLoop();

    /** Looping 홀로그램 정지 */
    UFUNCTION(BlueprintCallable, Category = "Appearance")
    void StopHoloLoop();

    /** 초기화 */
    UFUNCTION(BlueprintCallable, Category = "Appearance")
    void ResetCharacter();

    /** 카메라 바라보기 */
    UFUNCTION(BlueprintCallable, Category = "Appearance")
    void LookAtCamera();
};
