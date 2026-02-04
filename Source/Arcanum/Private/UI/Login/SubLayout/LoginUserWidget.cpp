#include "UI/Login/SubLayout/LoginUserWidget.h"

#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/Image.h"

void ULoginUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 로그인 버튼 이벤트 바인딩
    if (LoginButton) {
        LoginButton->OnClicked.RemoveDynamic(this, &ULoginUserWidget::OnLoginClicked);
        CancelButton->OnClicked.RemoveDynamic(this, &ULoginUserWidget::OnCancelButtonClicked);

        LoginButton->OnClicked.AddDynamic(this, &ULoginUserWidget::OnLoginClicked);
        CancelButton->OnClicked.AddDynamic(this, &ULoginUserWidget::OnCancelButtonClicked);
    }

    // 초기화: 텍스트박스 초기 내용 지우기
    if (IDTextBox) IDTextBox->SetText(FText::FromString(TEXT("")));
    if (PasswordTextBox) PasswordTextBox->SetText(FText::FromString(TEXT("")));
}

void ULoginUserWidget::OnLoginClicked()
{
    FString ID, Password;
    GetLoginInfo(ID, Password);

    OnOKClicked.Broadcast(ID, Password);
}
void ULoginUserWidget::OnCancelButtonClicked()
{
    OnCancelClicked.Broadcast();
}
void ULoginUserWidget::GetLoginInfo(FString& OutID, FString& OutPassword) const
{
    OutID = IDTextBox ? IDTextBox->GetText().ToString() : TEXT("");
    OutPassword = PasswordTextBox ? PasswordTextBox->GetText().ToString() : TEXT("");
}