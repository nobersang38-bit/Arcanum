#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "VfxRow.generated.h"

class UNiagaraSystem;

/*
 * 추영호
 * Niagara VFX DataTable Row
 * 풀링 기반 VFX 스폰용 데이터
 */
USTRUCT(BlueprintType)
struct FVfxRow : public FTableRowBase
{
	GENERATED_BODY()

	/* 나이아가라 시스템 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX")
	TSoftObjectPtr<UNiagaraSystem> Niagara;

	/* 부착할 소켓 (None이면 월드 스폰) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX|Attach")
	FName SocketName = NAME_None;

	/* 소켓 기준 오프셋 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX|Transform")
	FVector LocationOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX|Transform")
	FRotator RotationOffset = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX|Transform")
	FVector Scale = FVector(1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX|Time")
	float LifeTime = 0.0f;

};

