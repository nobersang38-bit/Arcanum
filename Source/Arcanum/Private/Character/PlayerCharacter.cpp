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
#include "Component/Stats/CharacterBattleStatsComponent.h"
#include "Component/StatusActionComponent.h"
#include "Components/DecalComponent.h"

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

	StatComponent = CreateDefaultSubobject<UCharacterBattleStatsComponent>(TEXT("StatComponent"));

	StatusActionComponent = CreateDefaultSubobject<UStatusActionComponent>(TEXT("StatusActionComponent"));

	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshComponent"));
	WeaponMeshComponent->SetupAttachment(GetMesh(), WeaponAttachSocketName);
	WeaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMeshComponent->SetReceivesDecals(false);

	UltimatePreviewDecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("UltimatePreviewDecalComponent"));
	UltimatePreviewDecalComponent->SetupAttachment(RootComponent);
	UltimatePreviewDecalComponent->DecalSize = UltimatePreviewDecalSize;
	UltimatePreviewDecalComponent->SetVisibility(false);
	UltimatePreviewDecalComponent->SetHiddenInGame(true);
	UltimatePreviewDecalComponent->SetUsingAbsoluteRotation(true);

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
		StatComponent->SetData(BattleSubsystem->GetInBattleData().PlayerBattleStat);
		StatComponent->OnCharacterRegenStatChanged.AddUObject(this, &APlayerCharacter::OnPlayerRegenStatChanged);
		StatusActionComponent->SetupAction();
	}
	FGameplayTag PlayerID = FGameplayTag::RequestGameplayTag(TEXT("Arcanum.Player.ID.Elara"));
	GameplayTags.AddTag(PlayerID);

	PrintIDTag();

	OnTakeAnyDamage.RemoveDynamic(this, &APlayerCharacter::RecievedDamage);
	OnTakeAnyDamage.AddDynamic(this, &APlayerCharacter::RecievedDamage);

	ABattlePlayerController* OwnerPC = Cast<ABattlePlayerController>(GetController());
	if (OwnerPC)
	{
		if (const FRegenStat* RegenStat = StatComponent->FindRegenStat(HealthTag))
		{
			OwnerPC->SetPlayerHealthProgress(RegenStat->Current, RegenStat->GetBaseMax());
		}
	}

	UpdateEquippedWeaponMesh();
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
	const FRegenStat* HealthStat = StatComponent->FindRegenStat(HealthTag);
	if (HealthStat)
	{
		if (OwnerPC)
		{
			StatComponent->ChangeStatValue(HealthTag, -Damage, DamageCauser);
			OwnerPC->SetPlayerHealthProgress(HealthStat->Current, HealthStat->GetTotalMax());
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

void APlayerCharacter::AddCurrentStat(FGameplayTag InTag, float InValue)
{

}

void APlayerCharacter::UpdateEquippedWeaponMesh()
{

	UBattlefieldManagerSubsystem* battleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
	if (!battleSubsystem)
	{
		ClearWeaponMesh();
		return;
	}

	USkeletalMesh* weaponMesh = battleSubsystem->GetCurrentWeaponMesh();
	if (!weaponMesh)
	{
		ClearWeaponMesh();
		return;
	}

	const FGameplayTag slotTypeTag = battleSubsystem->GetCurrentWeaponSlotTypeTag();
	if (!slotTypeTag.IsValid())
	{
		ClearWeaponMesh();
		return;
	}

	FName attachSocketName;

	if (slotTypeTag.MatchesTagExact(Arcanum::Items::ItemSlot::Weapon::RightHand))
	{
		attachSocketName = TEXT("Weapon_R");
	}
	else if (slotTypeTag.MatchesTagExact(Arcanum::Items::ItemSlot::Weapon::LeftHand))
	{
		attachSocketName = TEXT("Weapon_L");
	}
	else if (slotTypeTag.MatchesTagExact(Arcanum::Items::ItemSlot::Weapon::TwoHand))
	{
		attachSocketName = TEXT("Weapon_R");
	}
	else
	{
		ClearWeaponMesh();
		return;
	}

	WeaponAttachSocketName = attachSocketName;
	AttachWeaponMesh(weaponMesh);
}

void APlayerCharacter::AttachWeaponMesh(USkeletalMesh* InWeaponMesh)
{
	if (WeaponMeshComponent)
	{
		WeaponMeshComponent->SetSkeletalMesh(InWeaponMesh);
		WeaponMeshComponent->AttachToComponent(
			GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			WeaponAttachSocketName);
	}
}

void APlayerCharacter::ClearWeaponMesh()
{
	if (WeaponMeshComponent)
	{
		WeaponMeshComponent->SetSkeletalMesh(nullptr);
	}
}

void APlayerCharacter::ShowUltimatePreview()
{
	if (UltimatePreviewDecalComponent)
	{
		UltimatePreviewDecalComponent->SetVisibility(true);
		UltimatePreviewDecalComponent->SetHiddenInGame(false);
	}
}

void APlayerCharacter::HideUltimatePreview()
{
	if (UltimatePreviewDecalComponent)
	{
		UltimatePreviewDecalComponent->SetVisibility(false);
		UltimatePreviewDecalComponent->SetHiddenInGame(true);
	}
}

void APlayerCharacter::UpdateUltimatePreviewLocation(const FVector& InWorldLocation)
{
	if (UltimatePreviewDecalComponent)
	{
		FVector traceStart = InWorldLocation + FVector(0.f, 0.f, 500.f);
		FVector traceEnd = InWorldLocation - FVector(0.f, 0.f, 1000.f);

		FHitResult hitResult;
		FCollisionQueryParams params;
		params.AddIgnoredActor(this);

		if (GetWorld()->LineTraceSingleByChannel(hitResult, traceStart, traceEnd, ECC_WorldStatic, params))
		{
			FVector decalLocation = hitResult.ImpactPoint + (hitResult.ImpactNormal * 2.0f);
			UltimatePreviewDecalComponent->SetWorldLocation(decalLocation);
			UltimatePreviewDecalComponent->SetWorldRotation(FRotator(-90.0f, 0.0f, 0.0f));
		}
	}
}

void APlayerCharacter::OnPlayerRegenStatChanged(const FRegenStat& InRegenStat)
{
	if (InRegenStat.ParentTag == Arcanum::BattleStat::Character::Regen::Health::Root)
	{
		ABattlePlayerController* ownerPC = GetController<ABattlePlayerController>();
		if (ownerPC)
		{
			UE_LOG(LogTemp, Warning, TEXT("Current Health = %.2f / %.2f"), InRegenStat.Current, InRegenStat.GetTotalMax());
			ownerPC->SetPlayerHealthProgress(InRegenStat.Current, InRegenStat.GetTotalMax());
		}
	}
}

void APlayerCharacter::HandleBasicAttackInput()
{
	UBattlefieldManagerSubsystem* battleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
	if (!battleSubsystem) return;
	const FBattleSkillData* currentBasicAttackSkill = battleSubsystem->GetCurrentBasicAttackSkillData();
	if (!currentBasicAttackSkill) return;
	USkeletalMeshComponent* meshComp = GetMesh();
	if (!meshComp) return;
	UAnimInstance* animInstance = meshComp->GetAnimInstance();
	if (!animInstance) return;

	if (bIsBasicAttackMontagePlaying)
	{
		if (bCanNextComboInput)
		{
			bHasNextComboInput = true;
		}
		return;
	}

	if (!currentBasicAttackSkill->ComboMontages.IsValidIndex(BasicAttackComboIndex))
	{
		ResetBasicAttackCombo();
		return;
	}

	UAnimMontage* montage = currentBasicAttackSkill->ComboMontages[BasicAttackComboIndex];
	if (!montage)
	{
		ResetBasicAttackCombo();
		return;
	}

	bCanNextComboInput = false;
	bHasNextComboInput = false;
	bIsBasicAttackMontagePlaying = true;

	FOnMontageEnded montageEndedDelegate;
	montageEndedDelegate.BindUObject(this, &APlayerCharacter::OnBasicAttackMontageEnded);

	animInstance->Montage_Play(montage);
	animInstance->Montage_SetEndDelegate(montageEndedDelegate, montage);
}

void APlayerCharacter::OnBasicAttackMontageEnded(UAnimMontage* InMontage, bool bInterrupted)
{
	bIsBasicAttackMontagePlaying = false;

	if (bInterrupted)
	{
		ResetBasicAttackCombo();
		return;
	}

	ProceedBasicAttackCombo();
}

void APlayerCharacter::OnCommonSkillMontageEnded(UAnimMontage* InMontage, bool bInterrupted)
{
	bIsCommonSkillMontagePlaying = false;
}

void APlayerCharacter::EnableNextComboInput()
{
	bCanNextComboInput = true;
}
void APlayerCharacter::DisableNextComboInput()
{
	bCanNextComboInput = false;
}

void APlayerCharacter::ProceedBasicAttackCombo()
{
	if (bHasNextComboInput)
	{
		bHasNextComboInput = false;
		bCanNextComboInput = false;
		BasicAttackComboIndex++;

		HandleBasicAttackInput();

	}
	else
	{
		ResetBasicAttackCombo();
	}
}
void APlayerCharacter::ResetBasicAttackCombo()
{
	BasicAttackComboIndex = 0;
	bCanNextComboInput = false;
	bHasNextComboInput = false;
	bIsBasicAttackMontagePlaying = false;
}

void APlayerCharacter::HandleCommonSkillInput()
{
	if (bIsCommonSkillMontagePlaying) return;
	UBattlefieldManagerSubsystem* battleSubsystem = GetWorld()->GetSubsystem<UBattlefieldManagerSubsystem>();
	if (!battleSubsystem) return;
	const FBattleSkillData* currentBasicSkill = battleSubsystem->GetCurrentBasicSkillData();
	if (!currentBasicSkill)	return;
	USkeletalMeshComponent* meshComp = GetMesh();
	if (!meshComp) return;
	UAnimInstance* animInstance = meshComp->GetAnimInstance();
	if (!animInstance) return;
	UAnimMontage* montage = currentBasicSkill->CastMontage;
	if (!montage) return;

	bIsCommonSkillMontagePlaying = true;

	FOnMontageEnded montageEndedDelegate;
	montageEndedDelegate.BindUObject(this, &APlayerCharacter::OnCommonSkillMontageEnded);

	animInstance->Montage_Play(montage);
	animInstance->Montage_SetEndDelegate(montageEndedDelegate, montage);
}

