#include "Object/GachaLevel/ResultStarChild.h"
#include "GameplayTags/ArcanumTags.h"
#include "UI/Gacha/ARGachaController.h"

#include "NiagaraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Animation/SkeletalMeshActor.h"
#include "Components/SkeletalMeshComponent.h"

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
    NiagaraComp->Deactivate();
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
}
void AResultStarChild::OnNotifyClicked(AActor* TouchedActor, FKey ButtonPressed)
{
    OpenStar();
}
void AResultStarChild::OpenStar(bool bIsSkip)
{
    if (IsClicked) return;
    APlayerCameraManager* CamManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
    if (CamManager) {
        FVector CameraLoc = CamManager->GetCameraLocation();
        FVector MyLoc = GetActorLocation();
        FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(MyLoc, CameraLoc);
        SetActorRotation(LookAtRot);
    }

    IsClicked = true;
    bIsSkiped = bIsSkip;
    OnStarClicked.Broadcast(this);
    PlayOpenAnimation(ResultData);
}
void AResultStarChild::EndOpenAnimation()
{
    if (!TryLoadAndSpawnResult()) return;

    HideStarParts();

    if (IsHighGrade() && !bIsSkiped) {
        SpawnHighGradeWidget();
        PlayResultFX();
    }
    else {
        PlayResultFX();
        AARGachaController* PC = Cast<AARGachaController>(GetWorld()->GetFirstPlayerController());
        if (PC) PC->HandleGachaFinished();
    }
}
void AResultStarChild::DeactivateResultNiagara()
{
    if (NiagaraComp) NiagaraComp->Deactivate();
}
void AResultStarChild::SpawnHighGradeWidget()
{
    AARGachaController* PC = Cast<AARGachaController>(GetWorld()->GetFirstPlayerController());
    if (PC) PC->ShowGachaHighgradeUI(this->ResultData, ResultDialog);
}
bool AResultStarChild::IsHighGrade() const
{
    for (const FGameplayTag& Tag : HighGradeTags) {
        if (ResultData.GradeTag.MatchesTag(Tag)) return true;
    }
    return false;
}
bool AResultStarChild::TryLoadAndSpawnResult()
{
    UDataTable* TargetTable = ResultData.SourceTable.LoadSynchronous();
    if (!TargetTable) return false;

    FName RowName = GetLeafNameFromTag(ResultData.ItemTag);
    static const FString Context(TEXT("GachaResultContext"));

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    if (ResultData.ItemTag.MatchesTag(Arcanum::Player::ID::Root)) { // 캐릭터 분기
        auto* Row = TargetTable->FindRow<FDTCharacterBaseInfoRow>(RowName, Context);
        if (Row && Row->BattleCharacterInfo.CharacterClass.LoadSynchronous()) {
            ResultActor = GetWorld()->SpawnActor<AActor>(Row->BattleCharacterInfo.CharacterClass.Get(), GetActorLocation(), GetActorRotation(), SpawnParams);
            ResultDialog = Row->BattleCharacterInfo.DefaultDialogue;
        }
    }
    else if (ResultData.ItemTag.MatchesTag(Arcanum::Items::Rarity::Root)) { // 장비 분기
        auto* Row = TargetTable->FindRow<FDTEquipmentInfoRow>(RowName, Context);
        if (Row && Row->SkeletalMesh.LoadSynchronous()) {
            auto* MeshActor = GetWorld()->SpawnActor<ASkeletalMeshActor>(ASkeletalMeshActor::StaticClass(), GetActorLocation(), GetActorRotation(), SpawnParams);
            if (MeshActor) {
                MeshActor->GetSkeletalMeshComponent()->SetSkeletalMesh(Row->SkeletalMesh.Get());
                ResultActor = MeshActor;
                ResultDialog = Row->Desc;
            }
        }
    }

    return ResultActor != nullptr;
}
void AResultStarChild::HideStarParts()
{
    if (LeftPart) LeftPart->SetHiddenInGame(true);
    if (RightPart) RightPart->SetHiddenInGame(true);
}
void AResultStarChild::PlayResultFX()
{
    if (NiagaraComp) {
        NiagaraComp->Activate(true);
        GetWorldTimerManager().SetTimer(ResultNiagaraTimer, this, &AResultStarChild::DeactivateResultNiagara, NiagaraTimerDelay, false);
    }
}