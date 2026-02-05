#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Core/ArcanumSaveGame.h"
#include "ARGameInstance.generated.h"

UCLASS()
class ARCANUM_API UARGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
    // 초기화 함수 오버라이드 (여기서 데이터를 로드합니다)
    virtual void Init() override;

protected:
    // 포인터로 선언하고 UPROPERTY를 붙여 메모리 관리를 맡깁니다.
    UPROPERTY()
    TObjectPtr<UArcanumSaveGame> ArSaveGame;

    // 저장 파일 이름 고정
    const FString SaveSlotName = TEXT("MainSaveSlot");

#pragma region ID/PW용
public:
    bool AddIDPW(FString ID, FString PW);
    bool CheckLogin(FString ID, FString PW);
    void SaveGameData(); // 저장 실행 함수
#pragma endregion

};
