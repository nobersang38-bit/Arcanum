#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ARLoginController.generated.h"

class UARLoginHUD;

UCLASS()
class ARCANUM_API AARLoginController : public APlayerController
{
	GENERATED_BODY()
	
#pragma region 언리얼 기본 생성
public:
	AARLoginController();
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
#pragma endregion

#pragma region HUD 위젯 클래스
public:
	// Login HUD용 위젯
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UARLoginHUD> LoginHUDClass;
	// 현재 생성된 Login HUD 위젯 인스턴스
	UPROPERTY()
	TObjectPtr<UARLoginHUD> LoginHUD;
#pragma endregion
};
