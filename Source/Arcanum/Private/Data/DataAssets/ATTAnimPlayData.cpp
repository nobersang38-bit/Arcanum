// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/DataAssets/ATTAnimPlayData.h"

void UATTAnimPlayData::PlayAnim(TArray<UMaterialInstanceDynamic*> SetMaterials)
{
	for (int i = 0; i < SetMaterials.Num(); i++)
	{
		SetMaterials[i]->SetScalarParameterValue(StartFrameParameterName, FrameRange.X);
		SetMaterials[i]->SetScalarParameterValue(EndFrameParameterName, FrameRange.Y);
		// Todo : AnimToTexture테스트
		UE_LOG(LogTemp, Warning, TEXT("애님 실행 발동!!!!"));
	}
}
