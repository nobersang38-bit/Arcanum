#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LoginCharacter.generated.h"

class USceneCaptureComponent2D;

/*
 * Version : 1.0.0.0 2026/02/04
 * 클래스 역할 :
 *  1. 로그인 화면에서 보여줄 메인 캐릭터 한마리
 *  2. AppearCharacter() 호출 시 캐릭터 Visible, PlayAnimation(Blueprint)
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
    // Appearance State
    // ============================================
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Appearance")
    bool bHasAppeared = false;

protected:
    /** @brief 플레이어 초상화 생성을 위한 3D 캡처 카메라 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portrait")
    TObjectPtr<USceneCaptureComponent2D> CaptureComp;

    FVector InitialLocation;
    FRotator InitialRotation;

    // ============================================
    // Functions
    // ============================================
public:

    /**  */
    UFUNCTION(BlueprintCallable, Category = "Appearance")
    void AppearCharacter(bool bRequiredTick = false);

    /** 초기화 */
    UFUNCTION(BlueprintCallable, Category = "Appearance")
    void ResetCharacter();

    /** 카메라 바라보기 */
    UFUNCTION(BlueprintCallable, Category = "Appearance")
    void LookAtCamera();
};
