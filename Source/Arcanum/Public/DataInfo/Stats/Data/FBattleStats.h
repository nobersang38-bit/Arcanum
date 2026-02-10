#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "FBattleStats.generated.h"

/**
 * @brief 개별 스탯의 "상태(State)"만을 보관하는 구조체
 *
 * 이 구조체는 계산 로직을 가지지 않으며,
 * 오직 현재 상태와 기본 정의값만 저장한다.
 */
 // 1. 리젠이 필요한 동적 스탯 (HP, MP 등)
USTRUCT(BlueprintType)
struct FRegenStat {
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

    UPROPERTY(EditAnywhere, BlueprintReadOnly, SaveGame)
    float BonusMax = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, SaveGame)
    float ModifierMax = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, SaveGame)
    float ModifierTick = 0.0f;

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
        /// Todo: 추후 빌드시 풀어야함.
        //Current = GetTotalMax();
    }

    float GetTotalMax() const { return BaseMax + BonusMax + ModifierMax; }
    float GetBaseMax() const { return BaseMax; }
    float GetApplyedMax() const { return BonusMax + ModifierMax; }
    float GetTotalTick() const { return BaseTick + ModifierTick; }

    void AddFrom(const FRegenStat& Other)
    {
        BaseMax += Other.BaseMax;
        BaseTick += Other.BaseTick;
        BonusMax += Other.BonusMax;
        ModifierMax += Other.ModifierMax;
        ModifierTick += Other.ModifierTick;
    }

    friend FArchive& operator<<(FArchive& Ar, FRegenStat& Data)
    {
        Ar << Data.ParentTag;
        Ar << Data.Current;
        Ar << Data.BaseMax;
        Ar << Data.BaseTick;
        Ar << Data.BonusMax;
        Ar << Data.ModifierMax;
        Ar << Data.ModifierTick;
        return Ar;
    }
};

/**
 * 2. 고정 스탯 (공격력, 방어력 등)
 * UI에서의 구조: a(Base) + (b(Bonus) + c(Modifier)) = d(Total)
 */
USTRUCT(BlueprintType)
struct FNonRegenStat {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, SaveGame)
    FGameplayTag TagName;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, SaveGame)
    float BaseValue = 0.0f;  // 원본
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, SaveGame)
    float BonusValue = 0.0f; // 추가 값
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, SaveGame)
    float ModifierValue = 0.0f; // 모디파이용

    float GetTotalValue() const { return BaseValue + BonusValue + ModifierValue; }
    float GetBaseValue() const { return BaseValue; }
    float GetApplyedValue() const { return BonusValue + ModifierValue; }

    void AddFrom(const FNonRegenStat& Other)
    {
        BaseValue += Other.BaseValue;
        BonusValue += Other.BonusValue;
        ModifierValue += Other.ModifierValue;
    }

    friend FArchive& operator<<(FArchive& Ar, FNonRegenStat& Data)
    {
        Ar << Data.TagName;
        Ar << Data.BaseValue;
        Ar << Data.BonusValue;
        Ar << Data.ModifierValue;

        return Ar;
    }
};