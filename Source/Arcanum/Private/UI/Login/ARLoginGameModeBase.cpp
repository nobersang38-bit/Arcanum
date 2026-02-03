#include "UI/Login/ARLoginGameModeBase.h"
#include "UI/Login/ARLoginController.h"
// ========================================================
// 언리얼 기본생성
// ========================================================
AARLoginGameModeBase::AARLoginGameModeBase()
{
	PlayerControllerClass = AARLoginGameModeBase::StaticClass();

}
void AARLoginGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}