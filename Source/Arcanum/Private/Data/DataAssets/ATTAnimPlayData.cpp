// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/DataAssets/ATTAnimPlayData.h"

void UATTAnimPlayData::PlayAnim(TArray<UMaterialInstanceDynamic*> InMaterials)
{
	const FMaterialParameterInfo& StartFrameParameterInfo = FMaterialParameterInfo(StartFrameParameterName, EMaterialParameterAssociation::LayerParameter, -1);
	const FMaterialParameterInfo& EndFrameParameterInfo = FMaterialParameterInfo(EndFrameParameterName, EMaterialParameterAssociation::LayerParameter, -1);

	for (int i = 0; i < InMaterials.Num(); i++)
	{
		/*InMaterials[i]->SetScalarParameterValue(StartFrameParameterName, FrameRange.X);
		InMaterials[i]->SetScalarParameterValue(EndFrameParameterName, FrameRange.Y);*/


		InMaterials[i]->SetScalarParameterValueByInfo(StartFrameParameterInfo, FrameRange.X);
		InMaterials[i]->SetScalarParameterValueByInfo(EndFrameParameterInfo, FrameRange.Y);
		// Todo : AnimToTexture테스트
		UE_LOG(LogTemp, Warning, TEXT("애님 실행 발동!!!!"));
	}
}
