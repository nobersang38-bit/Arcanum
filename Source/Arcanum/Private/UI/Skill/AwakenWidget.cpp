#include "UI/Skill/AwakenWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"

void UAwakenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BindButtonEvents();

	InitGaugeMaterial();
}

void UAwakenWidget::NativeDestruct()
{
	if (ButtonAwaken)
	{
		ButtonAwaken->OnClicked.RemoveDynamic(this, &UAwakenWidget::HandleAwakenClicked);
	}

	Super::NativeDestruct();
}

void UAwakenWidget::BindButtonEvents()
{
	if (ButtonAwaken)
	{
		ButtonAwaken->OnClicked.RemoveDynamic(this, &UAwakenWidget::HandleAwakenClicked);
		ButtonAwaken->OnClicked.AddDynamic(this, &UAwakenWidget::HandleAwakenClicked);
	}
}

void UAwakenWidget::InitGaugeMaterial()
{
	if (ImageAwakenGauge)
	{
		GaugeMID = ImageAwakenGauge->GetDynamicMaterial();

		if (!GaugeMID)
		{
			UObject* resourceObject = ImageAwakenGauge->Brush.GetResourceObject();

			if (resourceObject)
			{
				UMaterialInterface* gaugeMaterial = Cast<UMaterialInterface>(resourceObject);

				if (gaugeMaterial)
				{
					GaugeMID = UMaterialInstanceDynamic::Create(gaugeMaterial, this);

					if (GaugeMID)
					{
						ImageAwakenGauge->SetBrushFromMaterial(GaugeMID);
					}
				}
			}
		}
	}
}

void UAwakenWidget::SetGaugePercent(float InPercent)
{
	if (GaugeMID)
	{
		GaugeMID->SetScalarParameterValue(TEXT("Percentage"), InPercent);
	}

	UpdateAwakenButtonVisual(InPercent);
}

void UAwakenWidget::SetAwakenState(bool bInAwakenActive)
{
	if (ButtonAwaken)
	{
		ButtonAwaken->SetIsEnabled(!bInAwakenActive);
	}

	if (!bInAwakenActive)
	{
		if (GaugeMID)
		{
			GaugeMID->SetScalarParameterValue(TEXT("Percentage"), 0.f);
		}
	}
}

void UAwakenWidget::RefreshAwakenVisual(float InGaugePercent)
{
	SetGaugePercent(InGaugePercent);
}

void UAwakenWidget::UpdateAwakenButtonVisual(float InGaugePercent)
{
	if (ButtonAwaken)
	{
		const bool bCanAwaken = (InGaugePercent >= 1.f);

		ButtonAwaken->SetIsEnabled(bCanAwaken);
		ButtonAwaken->SetRenderOpacity(bCanAwaken ? 1.f : 0.4f);
	}
}

void UAwakenWidget::HandleAwakenClicked()
{
	OnAwakenClickedRequested.Broadcast();
}
