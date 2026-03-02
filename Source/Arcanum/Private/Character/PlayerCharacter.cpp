// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayTagContainer.h"
#include "UI/Battle/BattlePlayerController.h"

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
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// 기본 캐릭터 ID 태그
	FGameplayTag PlayerID = FGameplayTag::RequestGameplayTag(TEXT("Arcanum.Player.ID.Elara"));
	GameplayTags.AddTag(PlayerID);

	PrintIDTag();

	OnTakeAnyDamage.RemoveDynamic(this, &APlayerCharacter::RecievedDamage);
	OnTakeAnyDamage.AddDynamic(this, &APlayerCharacter::RecievedDamage);

	ABattlePlayerController* OwnerPC = Cast<ABattlePlayerController>(GetController());
	if (OwnerPC)
	{
		OwnerPC->SetPlayerHealthProgress(CurrentHealth, MaxHealth);
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

FGameplayTag APlayerCharacter::GetTeamTag()
{
	return TeamTag;
}

void APlayerCharacter::RecievedDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	ABattlePlayerController* OwnerPC = Cast<ABattlePlayerController>(GetController());
	if (OwnerPC)
	{
		CurrentHealth -= Damage;
		CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, MaxHealth);
		OwnerPC->SetPlayerHealthProgress(CurrentHealth, MaxHealth);
	}
}

void APlayerCharacter::SetIDTag(FGameplayTag NewIDTag)
{
	if (!NewIDTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("태그가 없음!!!"));
		return;
	}

	FGameplayTagContainer RemoveTags;

	FGameplayTag IDRootTag = FGameplayTag::RequestGameplayTag(TEXT("Arcanum.Player.ID"));

	// 플레이어가 가지고 있는 태그에서 ID 태그만 가져와서 RemoveTags 안에 넣기
	for (const FGameplayTag& Tag : GameplayTags)
	{
		if (Tag.MatchesTag(IDRootTag))
		{
			RemoveTags.AddTag(Tag);
		}
	}

	// 태그에서 ID 태그만 삭제
	for (const FGameplayTag& Tag : RemoveTags)
	{
		GameplayTags.RemoveTag(Tag);
	}
	GameplayTags.AddTag(NewIDTag);
	PrintIDTag();
}

void APlayerCharacter::PrintIDTag()
{

	if (GameplayTags.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayTags 비어있음"));
		return;
	}

	for (const FGameplayTag& Tag : GameplayTags)
	{
		UE_LOG(LogTemp, Log, TEXT("Tag: %s"), *Tag.ToString());
	}
}

