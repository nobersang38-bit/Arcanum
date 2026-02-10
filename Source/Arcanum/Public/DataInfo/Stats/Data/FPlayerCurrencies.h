#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "FPlayerCurrencies.generated.h"

/**
 * @brief 플레이어의 재화를 보관하는 구조체
 *        
 */
USTRUCT(BlueprintType)
struct FRegenCurrencies
{
    GENERATED_BODY()
    
    // --- [A] JSON에 나타나고 세이브도 됨 ---
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FGameplayTag ParentTag;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, SaveGame)
    float Current = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, SaveGame)
    float BaseMax = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, SaveGame)
    float BaseTick = 0.0f;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, SaveGame)
    float BonusMax = 0.0f;

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, SaveGame)
    float BonusTick = 0.0f;

    // --- [C] JSON에도 안 나오고 세이브도 안 됨 ---
    FGameplayTag Child_Current;
    FGameplayTag Child_Max;
    FGameplayTag Child_Tick;

    void InitializeTags() {
        if (!ParentTag.IsValid()) return;

        TArray<FGameplayTag> Children;
        UGameplayTagsManager::Get().RequestGameplayTagChildren(ParentTag).GetGameplayTagArray(Children);

        for (const FGameplayTag& Tag : Children) {
            FString TagName = Tag.ToString();
            if (TagName.EndsWith(TEXT("Max"))) Child_Max = Tag;
            else if (TagName.EndsWith(TEXT("Tick"))) Child_Tick = Tag;
            else Child_Current = Tag; // 순수 Health, Mana 등
        }
        Current = GetTotalMax();
    }

    float GetTotalMax() const { return BaseMax + BonusMax; }
    float GetBaseMax() const { return BaseMax; }
    float GetApplyedMax() const { return BonusMax + BonusMax; }
    float GetTotalTick() const { return BaseTick + BonusTick; }

    friend FArchive& operator<<(FArchive& Ar, FRegenCurrencies& Data)
    {
        Ar << Data.ParentTag;
        Ar << Data.Current;
        Ar << Data.BaseMax;
        Ar << Data.BaseTick;
        Ar << Data.BonusMax;
        Ar << Data.BonusTick;
        return Ar;
    }
};

/**
 * 2. 고정 재화 (돈, 조각, 소울 등)
 *    따라서 데이터테이블 필요 없음
 */
USTRUCT(BlueprintType)
struct FNoneRegenCurrencies {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, SaveGame)
    FGameplayTag TagName;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, SaveGame)
    float BaseValue = 0.0f;  // 원본
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, SaveGame)
    float BonusValue = 0.0f; // 추가 값

    float GetTotalValue() const { return BaseValue + BonusValue; }
    float GetBaseValue() const { return BaseValue; }
    float GetApplyedValue() const { return BonusValue; }

    friend FArchive& operator<<(FArchive& Ar, FNoneRegenCurrencies& Data)
    {
        Ar << Data.TagName;
        Ar << Data.BaseValue;
        Ar << Data.BonusValue;

        return Ar;
    }
};