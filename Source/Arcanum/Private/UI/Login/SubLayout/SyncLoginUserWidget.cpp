#include "UI/Login/SubLayout/SyncLoginUserWidget.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"

#include "Core/ARGameInstance.h"

// ========================================================
// 델리게이트
// ========================================================
void USyncLoginUserWidget::FinishSyncVisual(bool bIsSuccess, const FString& ErrorMessage)
{
	OnSyncFinished.Broadcast(bIsSuccess, ErrorMessage);
}
// ========================================================
// 언리얼 기본 생성
// ========================================================
void USyncLoginUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
}
// ========================================================
// 상태 설정
// ========================================================
void USyncLoginUserWidget::SetSyncPhase(ESyncPhase InPhase, FText InMsg)
{
	CurrentPhase = InPhase;
	Msg->SetText(InMsg);

	switch (CurrentPhase)
	{
	case ESyncPhase::PreLogin:
		StartPreLogin();
		break;
	case ESyncPhase::PostLogin:
		StartPostLogin();
		break;
	}
}
void USyncLoginUserWidget::StartPreLogin()
{
	/// Info : 현재 서버가 없으니 우선 만들어 놓음. 
	///        만약 서버 생길 시, 서버에서 점검중 같은것을 띄울 예정.
	///        현재는 타이머 후 무조건 true 반환.

	UWorld* World = GetWorld();
	if (World) {
		FTimerHandle TimerHandle;
		World->GetTimerManager().SetTimer(TimerHandle,
			[this]() {FinishSyncVisual(true);}, 3.0f, false);
	}
}
void USyncLoginUserWidget::StartPostLogin()
{
	/// Todo : 뭔가를 할 예정이긴함
}
void USyncLoginUserWidget::PlayWidgetAnimation()
{
	/// Todo : 뭔 애니메이션을 재생 할 예정이긴함
}
