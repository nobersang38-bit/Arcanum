// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SubSystem/BattlefieldManagerSubsystem.h"
#include "GameFramework/Character.h"
#include "Object/Actor/BattlefieldManagerActor.h"
#include "GameplayTags/ArcanumTags.h"
#include "Kismet/GameplayStatics.h"
#include "Core/ARGameInstance.h"
#include "Data/Rows/AllyUnitsDataRow.h"
#include "Data/Rows/EnemyUnitsDataRow.h"

#include "Core/ARPlayerAccountService.h"

void UBattlefieldManagerSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	OnMatchEnded.AddUObject(this, &UBattlefieldManagerSubsystem::SetCurrentMatchData);

	/// 02/26 수정 : 서비스레이어 거치도록
	SetInBattleData(FPlayerAccountService::GetPlayerDataCopy(this), InBattleData);

	SetupUnits();
	DebugBasementSet();
	DebugSetUsingAllyUnits();
	//UARGameInstance* GameInstance = nullptr;
	//GameInstance = Cast<UARGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//if (GameInstance)
	//{
	//	SetPlayerData(GameInstance->GetPlayerData(), InBattleData);
	//}
}

void UBattlefieldManagerSubsystem::AddBasement(AActor* InNexus, FGameplayTag InTeamTag)
{
	if (!InNexus || !InTeamTag.IsValid()) return;

	if (Basements.Contains(InTeamTag))
	{
		UE_LOG(LogTemp, Error, TEXT("같은 팀의 기지가 하나 더 있습니다"));
	}
	else
	{
		Basements.Add(InTeamTag, InNexus);
	}
}

AActor* UBattlefieldManagerSubsystem::GetBasement(FGameplayTag InTeamTag) const
{
	return Basements.FindRef(InTeamTag);
}

FBasementStat UBattlefieldManagerSubsystem::GetBasementStat(FGameplayTag InTeamTag) const
{
	if (const FBasementStat* TempBasementStat = BasementStats.Find(InTeamTag))
	{
		return *TempBasementStat;
	}

	return FBasementStat();
}

void UBattlefieldManagerSubsystem::SetABattlefieldManagerActor(ABattlefieldManagerActor* InBattlefieldManagerActor)
{
	BattlefieldManagerActor = InBattlefieldManagerActor;
}

const TArray<FUnitData>& UBattlefieldManagerSubsystem::GetUsingAllyUnitData()
{
	return InBattleData.AllyUnits;
}

FUnitData UBattlefieldManagerSubsystem::GetAllyUnitData(FGameplayTag InUnitTag, bool& OutResult) const
{
	if (AllyUnitDatas.Contains(InUnitTag))
	{
		OutResult = true;
		return AllyUnitDatas.FindRef(InUnitTag);
	}
	else
	{
		OutResult = false;
		return FUnitData();
	}
}

FUnitData UBattlefieldManagerSubsystem::GetEnemyUnitData(FGameplayTag InUnitTag, bool& OutResult) const
{
	if (EnemyUnitDatas.Contains(InUnitTag))
	{
		OutResult = true;
		return EnemyUnitDatas.FindRef(InUnitTag);
	}
	else
	{
		OutResult = false;
		return FUnitData();
	}
}

void UBattlefieldManagerSubsystem::DebugSetUsingAllyUnits()
{
	InBattleData.AllyUnits.Empty();
	bool Result = false;
	InBattleData.AllyUnits.Add(GetAllyUnitData(Arcanum::Unit::Faction::Ally::Army::Root, Result));
}

void UBattlefieldManagerSubsystem::SetCurrentMatchData(const FMatchData& InData)
{
	CurrentMatchData = InData;
}

void UBattlefieldManagerSubsystem::SetupUnits()
{
	// 유닛의 모든 정보를 다 가져옴
	if (UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld()))
	{
		UGameDataSubsystem* GameDataSubsystem = GameInstance->GetSubsystem<UGameDataSubsystem>();
		const TMap<FGameplayTag, UDataTable*> DataTables = GameDataSubsystem->MasterDataTables;
		UE_LOG(LogTemp, Warning, TEXT("데이터 테이블 길이 %d"), DataTables.Num());
		if (const UDataTable* AllyUnitDataTable = DataTables.FindRef(Arcanum::DataTable::AllyUnitInfo))
		{
			TArray<FAllyUnitsDataRow*> AllyUnitsDatArray;
			AllyUnitDataTable->GetAllRows<FAllyUnitsDataRow>(TEXT(""), AllyUnitsDatArray);
			for (auto Row : AllyUnitsDatArray)
			{
				AllyUnitDatas.Add(Row->UnitData.Info.InfoSetting.Tag, Row->UnitData);
				UE_LOG(LogTemp, Warning, TEXT("AllyUnitDatas : %s"), *Row->UnitData.Info.InfoSetting.Tag.ToString());
			}
		}

		if (const UDataTable* EnemyUnitDataTable = DataTables.FindRef(Arcanum::DataTable::EnemyUnitInfo))
		{
			TArray<FEnemyUnitsDataRow*> EnemyUnitsDatArray;
			EnemyUnitDataTable->GetAllRows<FEnemyUnitsDataRow>(TEXT(""), EnemyUnitsDatArray);
			for (auto Row : EnemyUnitsDatArray)
			{
				EnemyUnitDatas.Add(Row->UnitData.Info.InfoSetting.Tag, Row->UnitData);
				UE_LOG(LogTemp, Warning, TEXT("EnemyUnitDatas : %s"), *Row->UnitData.Info.InfoSetting.Tag.ToString());
			}
		}
	}
}

void UBattlefieldManagerSubsystem::DebugBasementSet()
{
	FBasementStat PlayerBasementStat;
	FBasementStat EnemyBasementStat;

	PlayerBasementStat.CommandCenterCurrentHP.BaseValue = 1000.0f;
	EnemyBasementStat.CommandCenterCurrentHP.BaseValue = 1000.0f;

	BasementStats.Add(Arcanum::Unit::Faction::Ally::Root, PlayerBasementStat);
	BasementStats.Add(Arcanum::Unit::Faction::Enemy::Root, EnemyBasementStat);
}

void UBattlefieldManagerSubsystem::SetInBattleData(const FPlayerData& InPlayerData, FInBattleData& OutInBattleData)
{
	for (int i = 0; i < InPlayerData.OwnedCharacters.Num(); i++)
	{
		if (InPlayerData.OwnedCharacters[i].bSelection)
		{
			OutInBattleData.BattleCharacterData = InPlayerData.OwnedCharacters[i];
			break;
		}
	}

	OutInBattleData.PlayerBattleData = InPlayerData.PlayerBattleData;
}