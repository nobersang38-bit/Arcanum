#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ARGachaGameModeBase.generated.h"

class AShootingStarActor;
class AASplinePathActor;
class AResultStarActor;
class AGachaCamera;
struct FGachaItemResult;
enum class ESplinePathType : uint8;

UCLASS()
class ARCANUM_API AARGachaGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
#pragma region 가챠 결과 데이터
public:
	AARGachaGameModeBase();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gacha")
	TArray<FGachaItemResult> GachaItemResult;
protected:
	virtual void BeginPlay() override;
	FName GetSplinePathTypeName(ESplinePathType InType);
private:
	int32 ActiveResultStars;
#pragma endregion


#pragma region 가챠 연출 시작
public:
	void SetupGachaCamera(AActor* Target);

	UFUNCTION() void StartGachaSequence();
	UFUNCTION(BlueprintImplementableEvent) void PlayGachaSequence();
	
	UFUNCTION() void OnShootingStarImpact();
	UFUNCTION() void OnSingleResultFinished(AResultStarActor* FinishedActor);
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "00-Intro") TSubclassOf<AShootingStarActor> ShootingStarClass;	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "00-Intro") TSubclassOf<AResultStarActor> ResultStarClass;	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "00-Intro") TSoftClassPtr<AGachaCamera> GachaCameraClass;
private:
	UPROPERTY() TMap<ESplinePathType, AASplinePathActor*> SplineMap;
	UPROPERTY() TObjectPtr<AGachaCamera> ActiveGachaCamera;
#pragma endregion


#pragma region 별 낙하 이후 결과 표시
public:
	UFUNCTION() void ShowResult();
	UFUNCTION(BlueprintImplementableEvent) void StartResultSequence();
protected:

#pragma endregion

#pragma region 로비 복귀
public:
	UFUNCTION() void ReturnLobby();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "00-Map")
	TSoftObjectPtr<UWorld> LobbyMap;
#pragma endregion
};
