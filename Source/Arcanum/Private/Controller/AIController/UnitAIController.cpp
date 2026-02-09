// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/AIController/UnitAIController.h"
#include "Components/SphereComponent.h"
#include "Data/DataAssets/DATargetPriorityWeight.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Core/SubSystem/BattlefieldManagerSubsystem.h"
#include "GameplayTags/ArcanumTags.h"

AUnitAIController::AUnitAIController()
{
	PrimaryActorTick.bCanEverTick = false;
}