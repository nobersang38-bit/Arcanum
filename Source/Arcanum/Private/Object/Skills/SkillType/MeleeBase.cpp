#include "Object/Skills/SkillType/MeleeBase.h"
#include "Components/BoxComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "DataInfo/SkillData/Data/FSkillInfo.h"
#include "Object/Skills/SkillBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayTags/ArcanumTags.h"
#include "Interface/TeamInterface.h"

AMeleeBase::AMeleeBase()
{
	PrimaryActorTick.bCanEverTick = false;

	HitBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBoxComponent"));
	RootComponent = HitBoxComponent;

	HitBoxComponent->SetBoxExtent(BoxExtent);
	HitBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HitBoxComponent->SetCollisionObjectType(ECC_WorldDynamic);
	HitBoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	HitBoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	HitBoxComponent->SetGenerateOverlapEvents(true);

	HitBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AMeleeBase::OnHit);
}

void AMeleeBase::ActivateSkillActor(USkillBase* InSkill, AActor* InOwner, const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
	Super::ActivateSkillActor(InSkill, InOwner, SpawnLocation, SpawnRotation);

	UE_LOG(LogTemp, Warning, TEXT("Melee ActivateSkillActor"));

	HitActors.Reset();

	const FVector forward = SpawnRotation.Vector();
	const FVector meleeLocation = SpawnLocation + (forward * ForwardOffset.X);

	SetActorLocation(meleeLocation);
	SetActorRotation(SpawnRotation);

	HitBoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	HitBoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	HitBoxComponent->SetGenerateOverlapEvents(true);
	HitBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	UE_LOG(LogTemp, Warning, TEXT("Melee Collision After Set = %d"), (int32)HitBoxComponent->GetCollisionEnabled());
	HitBoxComponent->UpdateOverlaps();


	if (OwnerSkill)
	{
		if (const FLevelModifierEntry* currentEntry = OwnerSkill->GetCurrentLevelEntry())
		{
			if (currentEntry->Effect)
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(
					GetWorld(),
					currentEntry->Effect,
					GetActorLocation(),
					GetActorRotation());
			}

			GetWorldTimerManager().ClearTimer(ActiveTimerHandle);
			GetWorldTimerManager().SetTimer(
				ActiveTimerHandle,
				this,
				&AMeleeBase::DeactivateSkillActor,
				2,
				false);
		}
	}
}

void AMeleeBase::DeactivateSkillActor()
{
	GetWorldTimerManager().ClearTimer(ActiveTimerHandle);

	HitBoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HitActors.Reset();

	Super::DeactivateSkillActor();
}

void AMeleeBase::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Melee Box GenerateOverlap=%d CollisionEnabled=%d"), HitBoxComponent->GetGenerateOverlapEvents(), (int32)HitBoxComponent->GetCollisionEnabled());
}

void AMeleeBase::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Melee OnHit Other=%s"), OtherActor ? *OtherActor->GetName() : TEXT("None"));

	if (!bIsActive) return;
	if (!OtherActor || OtherActor == InstigatorActor.Get()) return;
	if (HitActors.Contains(OtherActor)) return;
	if (!InstigatorActor.IsValid()) return;
	if (!OwnerSkill) return;

	//ITeamInterface* myTeamInterface = Cast<ITeamInterface>(InstigatorActor.Get());
	//ITeamInterface* otherTeamInterface = Cast<ITeamInterface>(OtherActor);
	//if (!myTeamInterface || !otherTeamInterface) return;

	//const FGameplayTag myTeamTag = myTeamInterface->GetTeamTag();
	//const FGameplayTag otherTeamTag = otherTeamInterface->GetTeamTag();

	//if (const FSkillInfo* skillInfo = OwnerSkill->GetSkillInfo())
	//{
	//	if (skillInfo->TargetFilterTag == Arcanum::Unit::Faction::Enemy::Root && myTeamTag == otherTeamTag)	return;
	//	if (skillInfo->TargetFilterTag == Arcanum::Unit::Faction::Ally::Root && myTeamTag != otherTeamTag) return;
	//}

	HitActors.Add(OtherActor);

	float damage = 0.f; // 테스트

	if (const FItemDefinition* equipmentData = OwnerSkill->GetEquipmentData())
	{
		for (const FDerivedStatModifier& modifier : equipmentData->OnHitTargetStats)
		{
			if (modifier.StatTag == Arcanum::BattleStat::Character::Regen::Health::Value)
			{
				damage += modifier.Value.Flat;
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Melee Damage=%f"), damage);

	UGameplayStatics::ApplyDamage(
		OtherActor,
		damage,
		nullptr,
		nullptr,
		UDamageType::StaticClass());

	DeactivateSkillActor();
}


