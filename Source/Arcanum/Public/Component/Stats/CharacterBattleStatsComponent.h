#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterBattleStatsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANUM_API UCharacterBattleStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCharacterBattleStatsComponent();
protected:
	virtual void BeginPlay() override;


		
};
