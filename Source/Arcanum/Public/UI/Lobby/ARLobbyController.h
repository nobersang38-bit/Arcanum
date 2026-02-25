#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ARLobbyController.generated.h"

class ULobbyHUD;

UCLASS()
class ARCANUM_API AARLobbyController : public APlayerController
{
	GENERATED_BODY()

public:
	AARLobbyController();
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

#pragma region 로비 HUD 위젯 클래스
public:
	/* 로비 HUD 위젯 클래스 (BP 지정) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<ULobbyHUD> LobbyHUDClass;

	/* 현재 생성된 로비 HUD 위젯 인스턴스 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<ULobbyHUD> LobbyHUD;
#pragma endregion

};
