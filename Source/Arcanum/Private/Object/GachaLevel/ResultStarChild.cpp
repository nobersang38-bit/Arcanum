#include "Object/GachaLevel/ResultStarChild.h"

#include "NiagaraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"

AResultStarChild::AResultStarChild()
{
    PrimaryActorTick.bCanEverTick = false;

    DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = DefaultRoot;

    LeftPart = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftPart"));
    LeftPart->SetupAttachment(RootComponent);
    LeftPart->bForceDisableNanite = true;

    RightPart = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightPart"));
    RightPart->SetupAttachment(RootComponent);
    RightPart->bForceDisableNanite = true;

    NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
    NiagaraComp->SetupAttachment(RootComponent);
}

void AResultStarChild::BeginPlay()
{
    Super::BeginPlay();

    OnClicked.RemoveDynamic(this, &AResultStarChild::OnNotifyClicked);
    OnClicked.AddDynamic(this, &AResultStarChild::OnNotifyClicked);
}

void AResultStarChild::ApplyGradeMaterial(UMaterialInterface* InMat)
{
    if (!LeftPart && !RightPart) return;

    LeftPart->SetMaterial(0, InMat);
    RightPart->SetMaterial(0, InMat);
}

void AResultStarChild::SetResultData(const FGachaItemResult& InData)
{
    ResultData = InData;

    // Todo : Niagara 색상 변경등 셋팅 필요
}

void AResultStarChild::OnNotifyClicked(AActor* TouchedActor, FKey ButtonPressed)
{
    OnStarClicked.Broadcast(this);

    PlayOpenAnimation(ResultData);
}