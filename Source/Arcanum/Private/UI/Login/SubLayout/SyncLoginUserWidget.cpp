#include "UI/Login/SubLayout/SyncLoginUserWidget.h"

// ========================================================
// 델리게이트
// ========================================================
void USyncLoginUserWidget::FinishSyncVisual()
{
	OnSyncFinished.Broadcast();
}
void USyncLoginUserWidget::FailSyncVisual(const FString& ErrorMessage)
{
	OnSyncFailed.Broadcast(ErrorMessage);
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
void USyncLoginUserWidget::SetSyncPhase(ESyncPhase InPhase)
{
	CurrentPhase = InPhase;

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
	/// Todo : 뭔가를 할 예정이긴함
}
void USyncLoginUserWidget::StartPostLogin()
{
	/// Todo : 뭔가를 할 예정이긴함
}
void USyncLoginUserWidget::PlayWidgetAnimation()
{
	/// Todo : 뭔 애니메이션을 재생 할 예정이긴함
}