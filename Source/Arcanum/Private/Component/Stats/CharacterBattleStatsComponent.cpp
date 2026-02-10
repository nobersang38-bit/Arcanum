#include "Component/Stats/CharacterBattleStatsComponent.h"

UCharacterBattleStatsComponent::UCharacterBattleStatsComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharacterBattleStatsComponent::BeginPlay()
{
	Super::BeginPlay();

}

