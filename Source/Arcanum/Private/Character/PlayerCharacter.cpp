// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayTagContainer.h"
#include "UI/Battle/BattlePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/OverlapResult.h"
#include "Interface/RuntimeUnitDataInterface.h"
#include "Core/SubSystem/BattlefieldManagerSubsystem.h"

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
	UBattlefieldManagerSubsystem* BattleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
	if (BattleSubsystem)
	{
		TeamTag = BattleSubsystem->AllyTeamTag;

		RegenStats.Empty();
		NonRegenStats.Empty();

		const TArray<FRegenStat>& RegenStatsBack = BattleSubsystem->GetInBattleData().PlayerBattleStat.RegenStats;
		for (int i = 0; i < RegenStatsBack.Num(); i++)
		{
			RegenStats.Add(RegenStatsBack[i].ParentTag, RegenStatsBack[i]);
		}

		const TArray<FNonRegenStat>& NonRegenStatsBack = BattleSubsystem->GetInBattleData().PlayerBattleStat.NonRegenStats;
		for (int i = 0; i < NonRegenStatsBack.Num(); i++)
		{
			NonRegenStats.Add(NonRegenStatsBack[i].TagName, NonRegenStatsBack[i]);
		}
	}
	FGameplayTag PlayerID = FGameplayTag::RequestGameplayTag(TEXT("Arcanum.Player.ID.Elara"));
	GameplayTags.AddTag(PlayerID);

	PrintIDTag();

	OnTakeAnyDamage.RemoveDynamic(this, &APlayerCharacter::RecievedDamage);
	OnTakeAnyDamage.AddDynamic(this, &APlayerCharacter::RecievedDamage);

	ABattlePlayerController* OwnerPC = Cast<ABattlePlayerController>(GetController());
	if (OwnerPC)
	{
		FRegenStat* HealthStat = RegenStats.Find(HealthTag);
		if (HealthStat)
		{
			OwnerPC->SetPlayerHealthProgress(HealthStat->Current, HealthStat->GetTotalMax());
		}
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
	FRegenStat* HealthStat = RegenStats.Find(HealthTag);
	if (HealthStat)
	{
		if (OwnerPC)
		{
			{
				HealthStat->Current -= Damage;
				HealthStat->Current = FMath::Clamp(HealthStat->Current, 0.0f, HealthStat->GetTotalMax());
				OwnerPC->SetPlayerHealthProgress(HealthStat->Current, HealthStat->GetTotalMax());
			}
		}
		if (HealthStat->Current <= 0.0f)
		{
			UBattlefieldManagerSubsystem* BattleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
			if (BattleSubsystem)
			{
				FMatchData MatchData;

				MatchData.EndTimeSecond = BattleSubsystem->GetCurrentMatchData().EndTimeSecond;

				MatchData.bIsVictory = false;
				MatchData.CurrentMatchState = EMatchState::Ended;
				BattleSubsystem->OnMatchEnded.Broadcast(MatchData);
			}
		}
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

void APlayerCharacter::PlayerBasicAttack()
{
	DrawDebugSphere(GetWorld(), GetActorLocation() + GetActorForwardVector() * 150.0f, 100.0f, 16, FColor::Red, false, 2.0f);
	TArray<FOverlapResult> OutOverlaps;
	FCollisionShape MySphere = FCollisionShape::MakeSphere(100.0f);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	// 특정 위치(GetActorLocation)에서 한 프레임 즉시 검사
	bool bHit = GetWorld()->OverlapMultiByChannel(
		OutOverlaps,
		GetActorLocation() + GetActorForwardVector() * 150.0f,
		FQuat::Identity,
		ECollisionChannel::ECC_Pawn, // 설정한 채널
		MySphere,
		Params
	);

	for (int i = 0; i < OutOverlaps.Num(); i++)
	{
		if (!OutOverlaps[i].GetActor()) continue;
		if (OutOverlaps[i].GetActor() == this) continue;					// 자신이면 제외
		if (!OutOverlaps[i].GetActor()->Implements<UTeamInterface>()) continue;	// 팀 아이디 인터페이스가 없으면 제외


		if (OutOverlaps[i].GetActor()->GetClass()->ImplementsInterface(URuntimeUnitDataInterface::StaticClass()))
		{
			auto Interface = Cast<IRuntimeUnitDataInterface>(OutOverlaps[i].GetActor());
			if (Interface && Interface->GetIsDead())
			{
				continue;
			}
		}

		// 같은 팀이면 제외
		if (OutOverlaps[i].GetActor()->GetClass()->ImplementsInterface(UTeamInterface::StaticClass()) &&
			this->GetClass()->ImplementsInterface(UTeamInterface::StaticClass()))
		{
			auto OtherInterface = Cast<ITeamInterface>(OutOverlaps[i].GetActor());
			auto OwnerInterface = Cast<ITeamInterface>(this);

			if (OtherInterface->GetTeamTag() == OwnerInterface->GetTeamTag())
			{
				continue;
			}
		}
		else
		{
			continue;
		}

		if (OutOverlaps[i].GetActor())
		{
			UGameplayStatics::ApplyDamage(OutOverlaps[i].GetActor(), 30.0f, GetController(), this, nullptr);
		}
	}
}

void APlayerCharacter::SetupStat()
{
	UBattlefieldManagerSubsystem* BattleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();

	RegenStats.Empty();

	if (BattleSubsystem)
	{
		const FGradeStatData& GradeStatData = BattleSubsystem->GetInBattleData().PlayerBattleStat;
		for (int i = 0; i < GradeStatData.RegenStats.Num(); i++)
		{
			RegenStats.Add(GradeStatData.RegenStats[i].ParentTag, GradeStatData.RegenStats[i]);
		}

		for (int i = 0; i < GradeStatData.RegenStats.Num(); i++)
		{
			NonRegenStats.Add(GradeStatData.NonRegenStats[i].TagName, GradeStatData.NonRegenStats[i]);
		}
	}
}

FRegenStat* APlayerCharacter::FindRegenStat(FGameplayTag InTag)
{
	if (FRegenStat* RegenStat = RegenStats.Find(InTag))
	{
		return RegenStat;
	}
	return nullptr;
}

FNonRegenStat* APlayerCharacter::FindNonRegenStat(FGameplayTag InTag)
{
	if (FNonRegenStat* NonRegenStat = NonRegenStats.Find(InTag))
	{
		return NonRegenStat;
	}
	return nullptr;
}

void APlayerCharacter::AddCurrentStat(FGameplayTag InTag, float InValue)
{

}

