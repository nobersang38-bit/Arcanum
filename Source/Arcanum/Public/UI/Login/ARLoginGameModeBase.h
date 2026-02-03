#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ARLoginGameModeBase.generated.h"

UCLASS()
class ARCANUM_API AARLoginGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
#pragma region 언리얼 기본생성
public:
	AARLoginGameModeBase();
protected:
	virtual void BeginPlay() override;
#pragma endregion
};
