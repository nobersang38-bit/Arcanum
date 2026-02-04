// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayTagContainer.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetUsingAbsoluteRotation(true);
	SpringArm->TargetArmLength = 800.f;
	SpringArm->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	SpringArm->bDoCollisionTest = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//FGameplayTag PlayerID = FGameplayTag::RequestGameplayTag(TEXT("Arcanum.Player.ID.Elara"));
	//GameplayTags.AddTag(PlayerID);

	if (ASC)
	{
		// 기존 ID 태그 제거 (ID 하위 태그만)
		ASC->RemoveLooseGameplayTag(Arcanum::Player::ID::Root);

		// 기본 플레이어 ID 태그 추가(Elara)
		FGameplayTag PlayerID = FGameplayTag::RequestGameplayTag(TEXT("Arcanum.Player.ID.Elara"));
		ASC->AddLooseGameplayTag(PlayerID);
		
		FGameplayTagContainer ASC_Tags;
		ASC->GetOwnedGameplayTags(ASC_Tags);

		PrintIDTag();
	}
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerCharacter::SetIDTag(FGameplayTag NewIDTag)
{
	if (!ASC || !NewIDTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("ASC가 없거나 태그가 없음!!!"));
		return;
	}

	//// ID 태그 루트 제거
	//const FGameplayTag PlayerIDRoot = Arcanum::Player::ID::Root;
	//ASC->RemoveLooseGameplayTag(PlayerIDRoot);
	//UE_LOG(LogTemp, Warning, TEXT("ID 태그 루트 제거: %s"), *PlayerIDRoot.ToString());

	//// 기존 태그들을 모두 제거하고, 새 태그만 추가
	FGameplayTagContainer ASC_Tags;
	ASC->GetOwnedGameplayTags(ASC_Tags);

	// ASC_Tags에서 "ID."로 시작하는 태그들을 제거
	for (const FGameplayTag& Tag : ASC_Tags)
	{
		if (Tag.ToString().Contains(TEXT("ID.")))  // ID 태그들만 삭제
		{
			ASC->RemoveLooseGameplayTag(Tag);
			//UE_LOG(LogTemp, Warning, TEXT("기존 ID 태그 제거: %s"), *Tag.ToString());
		}
	}

	// 새 ID 태그 추가
	ASC->AddLooseGameplayTag(NewIDTag);
	//UE_LOG(LogTemp, Warning, TEXT("새 ID 태그 추가: %s"), *NewIDTag.ToString());

	PrintIDTag();
}

void APlayerCharacter::PrintIDTag()
{
	if (!ASC) return;
	FGameplayTagContainer ASC_Tags;
	ASC->GetOwnedGameplayTags(ASC_Tags);

	FString tagString;

	tagString.Empty();
	for (const FGameplayTag& Tag : ASC_Tags)
	{
		if (Tag.ToString().Contains(TEXT("ID.")))
		{
			tagString += Tag.ToString() + TEXT(", ");
		}
	}

	// 태그 상태 출력
	if (tagString.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("캐릭터 ID 태그 없음"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("캐릭터 ID 태그: %s"), *tagString);
	}
}

