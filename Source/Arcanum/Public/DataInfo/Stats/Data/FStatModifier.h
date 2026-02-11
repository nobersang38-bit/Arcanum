#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "FStatModifier.generated.h"

UENUM(BlueprintType)
enum class EModifierStackPolicy : uint8
{
    Refresh UMETA(DisplayName = "Refresh"),   // 갱신
    Stack   UMETA(DisplayName = "Stack")      // 중첩
};

USTRUCT(BlueprintType)
struct FExternalStatModifier
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
    float Flat = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
    float Mul = 1.f;

    friend FArchive& operator<<(FArchive& Ar, FExternalStatModifier& Data)
    {
        Ar << Data.Flat;
        Ar << Data.Mul;
        return Ar;
    }
};

/**
 * @brief PrimaryStat의 모디파이용 구조체
 *        일종의 GE의 Instance를 구현한 것과 같음
 */
USTRUCT(BlueprintType)
struct FDerivedStatModifier
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGuid ModifierId; // 아이템, 개별 버프용 (중복 허용)

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGameplayTag SourceTag; // 트레잇, 스킬, 중복불가 버프용 (식별 키)

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGameplayTag StatTag; // 대상 태그 (예: Stat.Combat.HP.Max)

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FExternalStatModifier Value; // Flat과 Mul 정보를 담은 구조체

    /** true면 Duration 무시하고 영구 적용 (아이템, 트레잇 등) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsPermanent = false;

    /** bIsPermanent가 false일 때만 작동하는 지속 시간 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Duration = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EModifierStackPolicy ModifierPolicy = EModifierStackPolicy::Refresh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxStack = 5;

    // 생성자에서 ID 발급 및 초기화
    FDerivedStatModifier()
        : ModifierId(0, 0, 0, 0)
    {
    }
    FDerivedStatModifier(FGameplayTag InTag, FExternalStatModifier InValue, float InDuration)
        : StatTag(InTag), Value(InValue), Duration(InDuration)
    {
        ModifierId = FGuid::NewGuid();
        bIsPermanent = (InDuration <= 0.0f);
    }

    friend FArchive& operator<<(FArchive& Ar, FDerivedStatModifier& Data)
    {
        Ar << Data.ModifierId;
        Ar << Data.SourceTag;
        Ar << Data.StatTag;
        Ar << Data.Value;
        Ar << Data.bIsPermanent;
        Ar << Data.Duration;
        Ar << Data.ModifierPolicy;
        Ar << Data.MaxStack;

        return Ar;
    }
};