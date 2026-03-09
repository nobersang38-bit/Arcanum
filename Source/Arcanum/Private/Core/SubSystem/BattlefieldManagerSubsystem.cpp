// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SubSystem/BattlefieldManagerSubsystem.h"
#include "GameFramework/Character.h"
#include "Object/Actor/BattlefieldManagerActor.h"
#include "Kismet/GameplayStatics.h"
#include "Core/ARGameInstance.h"
#include "Data/Rows/UnitsDataRow.h"
#include "Core/SubSystem/GameTimeSubsystem.h"
#include "DataInfo/BattleCharacter/BattleStats/DataTable/DTBattleStats.h"

#include "Core/ARPlayerAccountService.h"

void UBattlefieldManagerSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	OnMatchEnded.AddUObject(this, &UBattlefieldManagerSubsystem::MatchEnded);

	/// 02/26 수정 : 서비스레이어 거치도록
	SetInBattleData(FPlayerAccountService::GetPlayerDataCopy(this), InBattleData);

	SetupUnits();
	DebugBasementSet();
	DebugSetUsingAllyUnits();
	//DebugPlayerCharacterSet();

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
		UE_LOG(LogTemp, Error, TEXT("같은 팀인 기지가 하나 더 있습니다"));
	}
	else
	{
		Basements.Add(InTeamTag, InNexus);
	}

	if (InTeamTag.MatchesTag(AllyTeamTag))
	{

	}
	else if (InTeamTag.MatchesTag(EnemyTeamTag))
	{

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

const TArray<FUnitInfoSetting>& UBattlefieldManagerSubsystem::GetUsingAllyUnitData()
{
	return InBattleData.AllyUnits;
}

FUnitInfoSetting UBattlefieldManagerSubsystem::GetAllyUnitData(FGameplayTag InUnitTag, bool& OutResult) const
{
	if (AllyUnitDatas.Contains(InUnitTag))
	{
		OutResult = true;
		return AllyUnitDatas.FindRef(InUnitTag);
	}
	else
	{
		OutResult = false;
		return FUnitInfoSetting();
	}
}

FUnitInfoSetting UBattlefieldManagerSubsystem::GetEnemyUnitData(FGameplayTag InUnitTag, bool& OutResult) const
{
	if (EnemyUnitDatas.Contains(InUnitTag))
	{
		OutResult = true;
		return EnemyUnitDatas.FindRef(InUnitTag);
	}
	else
	{
		OutResult = false;
		return FUnitInfoSetting();
	}
}

void UBattlefieldManagerSubsystem::StartTime()
{
	
	UGameTimeSubsystem* TimeSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGameTimeSubsystem>();
	if (TimeSubsystem)
	{
		TimeSubsystem->StartStage(GetInBattleData().BattleStageInfo.StageLimitTime);

		TimeSubsystem->OnStageSecondChanged.RemoveDynamic(this, &UBattlefieldManagerSubsystem::CheckMatchEnded);
		TimeSubsystem->OnStageSecondChanged.AddDynamic(this, &UBattlefieldManagerSubsystem::CheckMatchEnded);
		TimeSubsystem->OnStageSecondChanged.AddDynamic(this, &UBattlefieldManagerSubsystem::OnTimeChange);
	}
}

void UBattlefieldManagerSubsystem::StopTime()
{
	UGameTimeSubsystem* TimeSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGameTimeSubsystem>();
	if (TimeSubsystem)
	TimeSubsystem->StopStage();
}

void UBattlefieldManagerSubsystem::OnTimeChange(int32 Time)
{
	CurrentMatchData.EndTimeSecond = Time;
}

void UBattlefieldManagerSubsystem::DebugSetUsingAllyUnits()
{
	InBattleData.AllyUnits.Empty();
	bool Result = false;
	InBattleData.AllyUnits.Add(GetAllyUnitData(Arcanum::Unit::Faction::Ally::Army::Root, Result));
	InBattleData.AllyUnits.Add(GetAllyUnitData(Arcanum::Unit::Faction::Ally::Bard::Root, Result));
	InBattleData.AllyUnits.Add(GetAllyUnitData(Arcanum::Unit::Faction::Ally::Maid::Root, Result));
	InBattleData.AllyUnits.Add(GetAllyUnitData(Arcanum::Unit::Faction::Ally::Tinker::Root, Result));
	InBattleData.AllyUnits.Add(GetAllyUnitData(Arcanum::Unit::Faction::Ally::PunchMan::Root, Result));
}

void UBattlefieldManagerSubsystem::CheckMatchEnded(int32 Time)
{
	if (Time <= 0)
	{
		FMatchData MatchData;

		MatchData.EndTimeSecond = CurrentMatchData.EndTimeSecond;

		MatchData.bIsVictory = false;
		MatchData.CurrentMatchState = EMatchState::Ended;
		OnMatchEnded.Broadcast(MatchData);
	}
}

void UBattlefieldManagerSubsystem::SetupUnits()
{
	// 유닛의 모든 정보를 다 가져옴
	if (UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(GetWorld()))
	{
		UGameDataSubsystem* GameDataSubsystem = GameInstance->GetSubsystem<UGameDataSubsystem>();
		const TMap<FGameplayTag, UDataTable*> DataTables = GameDataSubsystem->MasterDataTables;
		//UE_LOG(LogTemp, Warning, TEXT("데이터 테이블 길이 %d"), DataTables.Num());
		if (const UDataTable* AllyUnitDataTable = DataTables.FindRef(Arcanum::DataTable::AllyUnitInfo))
		{
			TArray<FUnitsDataRow*> AllyUnitsDatArray;
			AllyUnitDataTable->GetAllRows<FUnitsDataRow>(TEXT(""), AllyUnitsDatArray);
			for (auto Row : AllyUnitsDatArray)
			{
				AllyUnitDatas.Add(Row->UnitData.Tag, Row->UnitData);
				FString Result = FString::Printf(TEXT("AllyUnitDatas : %s"), *Row->UnitData.Tag.ToString());
				//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, *Result);
				//UE_LOG(LogTemp, Warning, TEXT("AllyUnitDatas : %s"), *Row->UnitData.Tag.ToString());
			}
		}

		if (const UDataTable* EnemyUnitDataTable = DataTables.FindRef(Arcanum::DataTable::EnemyUnitInfo))
		{
			TArray<FUnitsDataRow*> EnemyUnitsDatArray;
			EnemyUnitDataTable->GetAllRows<FUnitsDataRow>(TEXT(""), EnemyUnitsDatArray);
			for (auto Row : EnemyUnitsDatArray)
			{
				EnemyUnitDatas.Add(Row->UnitData.Tag, Row->UnitData);
				//UE_LOG(LogTemp, Warning, TEXT("EnemyUnitDatas : %s"), *Row->UnitData.Tag.ToString());
			}
		}
	}
}

void UBattlefieldManagerSubsystem::DebugBasementSet()
{
	FBasementStat PlayerBasementStat;
	FBasementStat EnemyBasementStat;


	PlayerBasementStat.CommandCenterHP.BaseMax = 1000.0f;
	EnemyBasementStat.CommandCenterHP.BaseMax = 1000.0f;

	BasementStats.Add(AllyTeamTag, PlayerBasementStat);
	BasementStats.Add(EnemyTeamTag, EnemyBasementStat);
}

void UBattlefieldManagerSubsystem::DebugEndedMessage(const FMatchData& MatchData)
{
	if (MatchData.bIsVictory)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("승리"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("패배"));
	}

}

void UBattlefieldManagerSubsystem::DebugPlayerCharacterSet()
{
	//struct FBattleCharacterData
	//{
	//	GENERATED_BODY()

	//	// --- 기본 고유 정보 ---
	//	/** 플레이어 가능한 캐릭터 중 선택이 되었는가? => 스테이지에서 조정 */
	//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	bool bSelection = false;
	//	/** 캐릭터 식별용 */
	//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	FGameplayTag Character;

	//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	FBattleCharacterInfo CharacterInfo;

	//	// --- 장비 데이터(캐릭터가 현재 장착하고 있는) ---
	//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	TMap<FGameplayTag, FGuid> Weapons;
	//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	TMap<FGameplayTag, FGuid> ArmorSlots;
	//};
	FBattleCharacterData One;
	One.bSelection = false;
	One.Character = Arcanum::Player::ID::Aiden;

	FBattleCharacterData Two;
	Two.bSelection = true;
	Two.Character = Arcanum::Player::ID::Arna;

	FBattleCharacterData Three;
	Three.bSelection = false;
	Three.Character = Arcanum::Player::ID::Celestia;


	TArray<FBattleCharacterData> OwnedCharacters = { One , Two, Three };
	InBattleData.PlayerData.OwnedCharacters = OwnedCharacters;

	for (int i = 0; i < InBattleData.PlayerData.OwnedCharacters.Num(); i++)
	{
		const FBattleCharacterData& OwnedCharacter = InBattleData.PlayerData.OwnedCharacters[i];
		if (OwnedCharacter.bSelection)
		{
			//Arcanum.DataTable.BattleStats
			UGameDataSubsystem* GameDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGameDataSubsystem>();
			if (GameDataSubsystem)
			{
				FString CharacterName = OwnedCharacter.Character.GetTagName().ToString();
				int32 LastDot;
				if (CharacterName.FindLastChar('.', LastDot))
				{
					// 찾은 인덱스 다음(+1)부터 끝까지 남기고 앞은 다 자릅니다.
					CharacterName = CharacterName.RightChop(LastDot + 1);
				}
				UE_LOG(LogTemp, Error, TEXT("%s"), *CharacterName);

				//GameDataSubsystem->GetRow<Arcanum::DataTable::BattleStats, FName()>
			}
			break;
		}
	}
}

void UBattlefieldManagerSubsystem::SetInBattleData(const FPlayerData& InPlayerData, FInBattleData& OutInBattleData)
{
	OutInBattleData.PlayerData = InPlayerData;

	// 플레이어블 캐릭터 가져오기
	for (int i = 0; i < InBattleData.PlayerData.OwnedCharacters.Num(); i++)
	{
		const FBattleCharacterData& OwnedCharacter = InBattleData.PlayerData.OwnedCharacters[i];
		if (OwnedCharacter.bSelection)
		{
			//Arcanum.DataTable.BattleStats
			UGameDataSubsystem* GameDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGameDataSubsystem>();
			if (GameDataSubsystem)
			{
				FString CharacterName = OwnedCharacter.Character.GetTagName().ToString();
				int32 LastDot;
				if (CharacterName.FindLastChar('.', LastDot))
				{
					// 찾은 인덱스 다음(+1)부터 끝까지 남기고 앞은 다 자릅니다.
					CharacterName = CharacterName.RightChop(LastDot + 1);

					FDTBattleStatsContainerRow* DTBattleStatsContainerRow = GameDataSubsystem->GetRow<FDTBattleStatsContainerRow>(Arcanum::DataTable::BattleStats, FName(CharacterName));
					if (DTBattleStatsContainerRow)
					{
						if (OwnedCharacter.CharacterInfo.CurrentGrade <= 0)
						{
							UE_LOG(LogTemp, Error, TEXT("선택된 캐릭터가 존재하지 않습니다"));
							break;
						}

						FGradeStatData GradeStatData = DTBattleStatsContainerRow->GradeDataSteps[OwnedCharacter.CharacterInfo.CurrentGrade - 1];
						OutInBattleData.PlayerBattleStat = GradeStatData;

						UE_LOG(LogTemp, Error, TEXT("%s"), *CharacterName);

						//로그 부분 나중에 삭제
						UE_LOG(LogTemp, Warning, TEXT("플레이어 스탯 리젠"));
						for (int j = 0; j < OutInBattleData.PlayerBattleStat.RegenStats.Num(); j++)
						{
							const FRegenStat& RegenStat = OutInBattleData.PlayerBattleStat.RegenStats[j];
							UE_LOG(LogTemp, Warning, TEXT("Tag : %s\tCurrent : %.02f\tMax : %.02f"), *RegenStat.ParentTag.ToString(), RegenStat.Current, RegenStat.GetBaseMax());
						}

						for (int j = 0; j < OutInBattleData.PlayerBattleStat.NonRegenStats.Num(); j++)
						{
							const FNonRegenStat& RegenStat = OutInBattleData.PlayerBattleStat.NonRegenStats[j];
							UE_LOG(LogTemp, Warning, TEXT("Tag : %s\tCurrent : %.02f\tMax : %.02f"), *RegenStat.TagName.ToString(), RegenStat.GetBaseValue(), RegenStat.GetTotalValue());
						}
						break;
					}
				}

			}
		}
	}

	//for (int i = 0; i < OutInBattleData.PlayerData.OwnedCharacters.Num(); i++)
	//{
	//	const FBattleCharacterData& OwnedCharacter = OutInBattleData.PlayerData.OwnedCharacters[i];
	//	if (OwnedCharacter.bSelection)
	//	{
	//		//Arcanum.DataTable.BattleStats
	//		UGameDataSubsystem* GameDataSubsystem = GetWorld()->GetSubsystem<UGameDataSubsystem>();
	//		if (GameDataSubsystem)
	//		{
	//			UE_LOG(LogTemp, Error, TEXT("%s"), *OwnedCharacter.Character.GetTagName().ToString());
	//			
	//			//GameDataSubsystem->GetRow<Arcanum::DataTable::BattleStats, FName()>
	//		}
	//		break;
	//	}
	//}

	//OutInBattleData.PlayerBattleStat = 
}

void UBattlefieldManagerSubsystem::MatchEnded(const FMatchData& MatchData)
{
	UGameInstance* GI = GetWorld()->GetGameInstance();
	if (GI)
	{
		UGameTimeSubsystem* GameTimeSubsystem = GI->GetSubsystem<UGameTimeSubsystem>();
		if (GameTimeSubsystem)
		{
			GameTimeSubsystem->StopStage();
			OnMatchEnded.Clear();
		}
	}
}
