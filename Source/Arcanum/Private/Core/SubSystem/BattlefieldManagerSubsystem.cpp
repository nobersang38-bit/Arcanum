// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SubSystem/BattlefieldManagerSubsystem.h"
#include "GameFramework/Character.h"
#include "Object/Actor/BattlefieldManagerActor.h"
#include "Kismet/GameplayStatics.h"
#include "Core/ARGameInstance.h"
#include "Data/Rows/UnitsDataRow.h"
#include "Core/SubSystem/GameTimeSubsystem.h"
#include "DataInfo/BattleCharacter/BattleStats/DataTable/DTBattleStats.h"
#include "DataInfo/SkillData/Data/FSkillInfo.h"
#include "DataInfo/SkillData/DataTable/DTSkillsData.h"
#include "GameFramework/GameMode.h"
#include "Object/Actor/SpawnCheckDecal.h"
#include "Core/ARPlayerAccountService.h"

void UBattlefieldManagerSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	OnMatchEnded.AddUObject(this, &UBattlefieldManagerSubsystem::MatchEnded);

	/// 02/26 수정 : 서비스레이어 거치도록
	SetInBattleData(FPlayerAccountService::GetPlayerDataCopy(this), InBattleData);

	SetupUnits();
	//DebugBasementSet();
	//DebugSetUsingAllyUnits();
	//DebugPlayerCharacterSet();
}

void UBattlefieldManagerSubsystem::AddAllyBasement(AActor* InNexus)
{
	if (!InNexus) return;

	AllyBasement = InNexus;
}

void UBattlefieldManagerSubsystem::AddEnemyBasement(AActor* InNexus)
{
	if (!InNexus) return;

	EnemyBasement = InNexus;
}

AActor* UBattlefieldManagerSubsystem::GetAllyBasement() const
{
	return AllyBasement.Get();
}

AActor* UBattlefieldManagerSubsystem::GetEnemyBasement() const
{
	return EnemyBasement.Get();
}

ASpawnCheckDecal* UBattlefieldManagerSubsystem::GetSpawnCheckDecal() const
{
	return CachedSpawnCheckDecal.Get();
}

void UBattlefieldManagerSubsystem::SetSpawnCheckDecal(ASpawnCheckDecal* InCheckDecal)
{
	CachedSpawnCheckDecal = InCheckDecal;
}

const FEnemyBasement& UBattlefieldManagerSubsystem::GetAllyBasementStat() const
{
	return AllyBasementStat;
}

const FEnemyBasement& UBattlefieldManagerSubsystem::GetEnemyBasementStat() const
{
	return EnemyBasementStat;
}

void UBattlefieldManagerSubsystem::SetABattlefieldManagerActor(ABattlefieldManagerActor* InBattlefieldManagerActor)
{
	BattlefieldManagerActor = InBattlefieldManagerActor;
}

const TMap<FGameplayTag, FUnitInfoSetting>& UBattlefieldManagerSubsystem::GetUsingAllyUnitData()
{
	return AllyUnitDatas;
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
	bool Result = false;
	//AllyUnitDatas.Add(Arcanum::Unit::Faction::Ally::Army::Root, GetAllyUnitData(Arcanum::Unit::Faction::Ally::Army::Root, Result));
	//AllyUnitDatas.Add(Arcanum::Unit::Faction::Ally::Bard::Root, GetAllyUnitData(Arcanum::Unit::Faction::Ally::Bard::Root, Result));
	//AllyUnitDatas.Add(Arcanum::Unit::Faction::Ally::Maid::Root, GetAllyUnitData(Arcanum::Unit::Faction::Ally::Maid::Root, Result));
	//AllyUnitDatas.Add(Arcanum::Unit::Faction::Ally::Tinker::Root, GetAllyUnitData(Arcanum::Unit::Faction::Ally::Tinker::Root, Result));
	//AllyUnitDatas.Add(Arcanum::Unit::Faction::Ally::PunchMan::Root, GetAllyUnitData(Arcanum::Unit::Faction::Ally::PunchMan::Root, Result));
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
	AllyBasementStat.CommandCenterHP.BaseValue = 1000.0f;
	EnemyBasementStat.CommandCenterHP.BaseValue = 1000.0f;
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
	OutInBattleData.PlayerBattleData = OutInBattleData.PlayerData.PlayerBattleData;

	// 플레이어블 캐릭터 가져오기
	for (int i = 0; i < InBattleData.PlayerData.OwnedCharacters.Num(); i++)
	{
		const FBattleCharacterData& OwnedCharacter = InBattleData.PlayerData.OwnedCharacters[i];
		if (OwnedCharacter.bSelection)
		{
			if (UClass* DefaultCharacter = OwnedCharacter.CharacterInfo.BattleCharacterInitData.CharacterClass.LoadSynchronous())
			{
				UE_LOG(LogTemp, Error, TEXT("%s"), *DefaultCharacter->GetName());
				APlayerController* PC = GetWorld()->GetFirstPlayerController();
				if (PC)
				{
					if (PC->GetPawn())
					{
						FTransform Transform = PC->GetPawn()->GetActorTransform();
						PC->GetPawn()->Destroy();
						APawn* PlayerCharacterF = GetWorld()->SpawnActor<APawn>(DefaultCharacter, Transform);
						PC->Possess(PlayerCharacterF);
					}
				}
			}

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
						if (OwnedCharacter.CharacterInfo.CurrStarLevel <= 0)
						{
							UE_LOG(LogTemp, Error, TEXT("선택된 캐릭터가 존재하지 않습니다"));
							break;
						}

						FGradeStatData GradeStatData = DTBattleStatsContainerRow->GradeDataSteps[OwnedCharacter.CharacterInfo.CurrStarLevel - 1];
						OutInBattleData.PlayerBattleStat = GradeStatData;


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

	//스테이지 데이터 설정 부분
	UGameDataSubsystem* GameDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGameDataSubsystem>();
	if (GameDataSubsystem)
	{
		// Todo KDH : 나중에 현재 스테이지 태그가 들어와야함
		FGameplayTag StageTag = Arcanum::BattleStage::Normal::Stage1;

		if (UDataTable** DTStageData = GameDataSubsystem->MasterDataTables.Find(Arcanum::DataTable::StageInfo))
		{
			TArray<FDTStageDataRow*> StageData;
			(*DTStageData)->GetAllRows<FDTStageDataRow>(FString(), StageData);
			for (int i = 0; i < StageData.Num(); i++)
			{
				if (StageData[i] && StageData[i]->StageData.StageTag == StageTag)
				{
					OutInBattleData.StageData = StageData[i]->StageData;
					EnemyBasementStat = OutInBattleData.StageData.EnemyBasement;
					break;
				}
			}
		}
	}
	for (const auto& PlayerNonRegen : OutInBattleData.PlayerBattleData.PlayerBattleNonRegenStat)
	{
		if (PlayerNonRegen.TagName == AllyBaseStaminaTag)
		{
			AllyBasementStat.CommandCenterHP = PlayerNonRegen;
			break;
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

	BuildBattleWeaponSkillCache(OutInBattleData);
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


// ========================================================
// 스킬 캐시
// ========================================================
void UBattlefieldManagerSubsystem::BuildBattleWeaponSkillCache(FInBattleData& OutInBattleData)
{
	OutInBattleData.BattleWeaponSkill = FBattleWeaponSkillData();

	FPlayerData& playerData = OutInBattleData.PlayerData;

	// 현재 선택된 플레이어 캐릭터 찾기
	FBattleCharacterData* selectedCharacter = nullptr;
	for (FBattleCharacterData& characterData : playerData.OwnedCharacters)
	{
		if (characterData.bSelection)
		{
			selectedCharacter = &characterData;
			break;
		}
	}
	if (!selectedCharacter)
	{
		return;
	}

	// 일반 무기 기본공격/기본스킬 캐시
	auto cacheNormalWeaponSkill = [this](const FEquipmentInfo* InEquipInfo, FBattleSkillData& OutBasicAttackSkill, FBattleSkillData& OutBasicSkill)
		{
			if (InEquipInfo)
			{
				OutBasicAttackSkill.SkillTag = InEquipInfo->Equipment.BasicAttackSkillTag;
				OutBasicAttackSkill.SkillLevel = InEquipInfo->CurrUpgradeLevel;
				OutBasicAttackSkill.SkillIcon = FindSkillIcon(OutBasicAttackSkill.SkillTag);

				if (const FSkillInfo* basicAttackSkillInfo = FindSkillInfoByTag(OutBasicAttackSkill.SkillTag))
				{
					OutBasicAttackSkill.ComboMontages.Empty();

					for (const TSoftObjectPtr<UAnimMontage>& comboMontage : basicAttackSkillInfo->ComboMontages)
					{
						OutBasicAttackSkill.ComboMontages.Add(comboMontage.LoadSynchronous());
						OutBasicAttackSkill.SkillClass = basicAttackSkillInfo->SkillClass.LoadSynchronous();
					}
				}

				for (const TPair<FGameplayTag, int32>& skillPair : InEquipInfo->Equipment.Skills)
				{
					if (!skillPair.Key.IsValid()) continue;
					if (skillPair.Key == Arcanum::Skills::SkillName::None) continue;
					if (skillPair.Value <= 0) continue;

					OutBasicSkill.SkillTag = skillPair.Key;
					OutBasicSkill.SkillLevel = InEquipInfo->CurrUpgradeLevel;
					OutBasicSkill.CastTime = FindSkillCastTime(OutBasicSkill.SkillTag, OutBasicSkill.SkillLevel);
					OutBasicSkill.SkillIcon = FindSkillIcon(OutBasicSkill.SkillTag);

					if (const FSkillInfo* basicSkillInfo = FindSkillInfoByTag(OutBasicSkill.SkillTag))
					{
						OutBasicSkill.CastMontage = basicSkillInfo->CastMontage.LoadSynchronous();
						OutBasicSkill.SkillClass = basicSkillInfo->SkillClass.LoadSynchronous();
					}

					break;
				}
			}
		};

	// 전설 무기 궁극기 스킬 캐시
	auto cacheLegendaryWeaponSkill = [this](const FEquipmentInfo* InEquipInfo, FBattleSkillData& OutUltimateSkill)
		{
			if (InEquipInfo)
			{
				for (const TPair<FGameplayTag, int32>& skillPair : InEquipInfo->Equipment.Skills)
				{
					if (!skillPair.Key.IsValid()) continue;
					if (skillPair.Key == Arcanum::Skills::SkillName::None) continue;
					if (skillPair.Value <= 0) continue;

					OutUltimateSkill.SkillTag = skillPair.Key;
					OutUltimateSkill.SkillLevel = InEquipInfo->CurrUpgradeLevel;
					OutUltimateSkill.CastTime = FindSkillCastTime(OutUltimateSkill.SkillTag, OutUltimateSkill.SkillLevel);
					OutUltimateSkill.SkillIcon = FindSkillIcon(OutUltimateSkill.SkillTag);

					if (const FSkillInfo* ultimateSkillInfo = FindSkillInfoByTag(OutUltimateSkill.SkillTag))
					{
						OutUltimateSkill.CastMontage = ultimateSkillInfo->CastMontage.LoadSynchronous();
						OutUltimateSkill.SkillClass = ultimateSkillInfo->SkillClass.LoadSynchronous();
					}

					break;
				}
			}
		};

	// 일반 무기 슬롯 1 캐시
	if (const FGuid* slot1Guid = selectedCharacter->WeaponSlots.Find(Arcanum::Items::ItemSlot::Weapon::Slot1))
	{
		cacheNormalWeaponSkill(
			FindEquipmentByGuid(playerData, *slot1Guid),
			OutInBattleData.BattleWeaponSkill.WeaponSlot1BasicAttackSkill,
			OutInBattleData.BattleWeaponSkill.WeaponSlot1BasicSkill);
	}

	// 일반 무기 슬롯 2 캐시
	if (const FGuid* slot2Guid = selectedCharacter->WeaponSlots.Find(Arcanum::Items::ItemSlot::Weapon::Slot2))
	{
		cacheNormalWeaponSkill(
			FindEquipmentByGuid(playerData, *slot2Guid),
			OutInBattleData.BattleWeaponSkill.WeaponSlot2BasicAttackSkill,
			OutInBattleData.BattleWeaponSkill.WeaponSlot2BasicSkill);
	}

	// 전설 무기 캐시
	if (const FGuid* legendaryGuid = selectedCharacter->LegendaryWeaponSlots.Find(Arcanum::Items::ItemSlot::Weapon::Legendary))
	{
		cacheLegendaryWeaponSkill(
			FindEquipmentByGuid(playerData, *legendaryGuid),
			OutInBattleData.BattleWeaponSkill.LegendaryUltimateSkill);
	}

	// 현재 기본 무기 슬롯 초기값 설정
	if (OutInBattleData.BattleWeaponSkill.WeaponSlot1BasicAttackSkill.SkillTag.IsValid())
	{
		OutInBattleData.BattleWeaponSkill.CurrentWeaponSlotTag = Arcanum::Items::ItemSlot::Weapon::Slot1;
	}
	else if (OutInBattleData.BattleWeaponSkill.WeaponSlot2BasicAttackSkill.SkillTag.IsValid())
	{
		OutInBattleData.BattleWeaponSkill.CurrentWeaponSlotTag = Arcanum::Items::ItemSlot::Weapon::Slot2;
	}
	else
	{
		OutInBattleData.BattleWeaponSkill.CurrentWeaponSlotTag = FGameplayTag();
	}
}

const FEquipmentInfo* UBattlefieldManagerSubsystem::FindEquipmentByGuid(const FPlayerData& InPlayerData, const FGuid& InItemGuid) const
{
	if (InItemGuid.IsValid())
	{
		for (const FEquipmentInfo& equip : InPlayerData.Inventory)
		{
			if (equip.ItemGuid == InItemGuid)
			{
				return &equip;
			}
		}
	}

	return nullptr;
}

const FSkillInfo* UBattlefieldManagerSubsystem::FindSkillInfoByTag(const FGameplayTag& InSkillTag) const
{
	if (!InSkillTag.IsValid()) return nullptr;

	UGameDataSubsystem* gameDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGameDataSubsystem>();
	if (!gameDataSubsystem) return nullptr;

	if (const FDTSkillsDataRow* skillRow = gameDataSubsystem->GetRow<FDTSkillsDataRow>(Arcanum::DataTable::SkillData, GetLeafNameFromTag(InSkillTag)))
	{
		return &skillRow->SkillData; 
	}

	return nullptr;
}

const FBattleCharacterData* UBattlefieldManagerSubsystem::GetSelectedCharacterData() const
{
	for (const FBattleCharacterData& characterData : InBattleData.PlayerData.OwnedCharacters)
	{
		if (characterData.bSelection)
		{
			return &characterData;
		}
	}

	return nullptr;
}

FGameplayTag UBattlefieldManagerSubsystem::GetCurrentWeaponSlotTag() const
{
	return InBattleData.BattleWeaponSkill.CurrentWeaponSlotTag;
}

FGameplayTag UBattlefieldManagerSubsystem::GetCurrentBasicAttackSkillTag() const
{
	const FBattleWeaponSkillData& battleWeaponSkill = InBattleData.BattleWeaponSkill;

	if (battleWeaponSkill.CurrentWeaponSlotTag == Arcanum::Items::ItemSlot::Weapon::Slot1)
	{
		return battleWeaponSkill.WeaponSlot1BasicAttackSkill.SkillTag;
	}

	if (battleWeaponSkill.CurrentWeaponSlotTag == Arcanum::Items::ItemSlot::Weapon::Slot2)
	{
		return battleWeaponSkill.WeaponSlot2BasicAttackSkill.SkillTag;
	}

	return FGameplayTag();
}

int32 UBattlefieldManagerSubsystem::GetCurrentBasicAttackSkillLevel() const
{

	const FBattleWeaponSkillData& battleWeaponSkill = InBattleData.BattleWeaponSkill;

	if (battleWeaponSkill.CurrentWeaponSlotTag == Arcanum::Items::ItemSlot::Weapon::Slot1)
	{
		return battleWeaponSkill.WeaponSlot1BasicAttackSkill.SkillLevel;
	}

	if (battleWeaponSkill.CurrentWeaponSlotTag == Arcanum::Items::ItemSlot::Weapon::Slot2)
	{
		return battleWeaponSkill.WeaponSlot2BasicAttackSkill.SkillLevel;
	}

	return 0;
}

FGameplayTag UBattlefieldManagerSubsystem::GetCurrentBasicSkillTag() const
{

	const FBattleWeaponSkillData& battleWeaponSkill = InBattleData.BattleWeaponSkill;

	if (battleWeaponSkill.CurrentWeaponSlotTag == Arcanum::Items::ItemSlot::Weapon::Slot1)
	{
		return battleWeaponSkill.WeaponSlot1BasicSkill.SkillTag;
	}

	if (battleWeaponSkill.CurrentWeaponSlotTag == Arcanum::Items::ItemSlot::Weapon::Slot2)
	{
		return battleWeaponSkill.WeaponSlot2BasicSkill.SkillTag;
	}

	return FGameplayTag();
}

int32 UBattlefieldManagerSubsystem::GetCurrentBasicSkillLevel() const
{

	const FBattleWeaponSkillData& battleWeaponSkill = InBattleData.BattleWeaponSkill;

	if (battleWeaponSkill.CurrentWeaponSlotTag == Arcanum::Items::ItemSlot::Weapon::Slot1)
	{
		return battleWeaponSkill.WeaponSlot1BasicSkill.SkillLevel;
	}

	if (battleWeaponSkill.CurrentWeaponSlotTag == Arcanum::Items::ItemSlot::Weapon::Slot2)
	{
		return battleWeaponSkill.WeaponSlot2BasicSkill.SkillLevel;
	}

	return 0;
}

FGameplayTag UBattlefieldManagerSubsystem::GetLegendaryUltimateSkillTag() const
{

	return InBattleData.BattleWeaponSkill.LegendaryUltimateSkill.SkillTag;
}

int32 UBattlefieldManagerSubsystem::GetLegendaryUltimateSkillLevel() const
{
	return InBattleData.BattleWeaponSkill.LegendaryUltimateSkill.SkillLevel;
}

UTexture2D* UBattlefieldManagerSubsystem::GetCurrentWeaponIcon() const
{
	const FBattleCharacterData* selectedCharacter = GetSelectedCharacterData();
	if (!selectedCharacter) return nullptr;

	const FGameplayTag currentWeaponSlotTag = InBattleData.BattleWeaponSkill.CurrentWeaponSlotTag;
	if (!currentWeaponSlotTag.IsValid()) return nullptr;

	const FGuid* weaponGuid = selectedCharacter->WeaponSlots.Find(currentWeaponSlotTag);
	if (!weaponGuid || !weaponGuid->IsValid()) return nullptr;

	const FEquipmentInfo* foundEquip = FindEquipmentByGuid(InBattleData.PlayerData, *weaponGuid);
	if (!foundEquip) return nullptr;

	const FDTItemCatalogRow* catalogRow = FPlayerAccountService::FindItemCatalogRowByTag(this, foundEquip->ItemTag);
	if (!catalogRow) return nullptr;

	return catalogRow->Icon.LoadSynchronous();
}

UTexture2D* UBattlefieldManagerSubsystem::GetLegendaryWeaponIcon() const
{
	const FBattleCharacterData* selectedCharacter = GetSelectedCharacterData();
	if (!selectedCharacter) return nullptr;

	const FGuid* weaponGuid = selectedCharacter->LegendaryWeaponSlots.Find(Arcanum::Items::ItemSlot::Weapon::Legendary);
	if (!weaponGuid || !weaponGuid->IsValid()) return nullptr;

	const FEquipmentInfo* foundEquip = FindEquipmentByGuid(InBattleData.PlayerData, *weaponGuid);
	if (!foundEquip) return nullptr;

	const FDTItemCatalogRow* catalogRow = FPlayerAccountService::FindItemCatalogRowByTag(this, foundEquip->ItemTag);
	if (!catalogRow) return nullptr;

	return catalogRow->Icon.LoadSynchronous();
}

UTexture2D* UBattlefieldManagerSubsystem::GetCurrentBasicSkillIcon() const
{
	const FBattleWeaponSkillData& battleWeaponSkill = InBattleData.BattleWeaponSkill;

	if (battleWeaponSkill.CurrentWeaponSlotTag == Arcanum::Items::ItemSlot::Weapon::Slot1)
	{
		return battleWeaponSkill.WeaponSlot1BasicSkill.SkillIcon;
	}
	if (battleWeaponSkill.CurrentWeaponSlotTag == Arcanum::Items::ItemSlot::Weapon::Slot2)
	{
		return battleWeaponSkill.WeaponSlot2BasicSkill.SkillIcon;
	}

	return nullptr;
}

UTexture2D* UBattlefieldManagerSubsystem::GetLegendaryUltimateSkillIcon() const
{
	return InBattleData.BattleWeaponSkill.LegendaryUltimateSkill.SkillIcon;
}

FGameplayTag UBattlefieldManagerSubsystem::GetEquippedSetBonusTag() const
{
	if (HasEquippedFullSet(Arcanum::Items::Rarity::SetItem::Talasha::Armor::Root))
	{
		return Arcanum::Items::SetBonus::Surge;
	}

	if (HasEquippedFullSet(Arcanum::Items::Rarity::SetItem::Sigon::Armor::Root))
	{
		return Arcanum::Items::SetBonus::Avarice;
	}

	return Arcanum::Skills::SkillName::None;
}

void UBattlefieldManagerSubsystem::SetCurrentWeaponSlotTag(const FGameplayTag& InWeaponSlotTag)
{
	InBattleData.BattleWeaponSkill.CurrentWeaponSlotTag = InWeaponSlotTag;
}

void UBattlefieldManagerSubsystem::BeginLegendaryWeaponMode()
{
	if (!bUsingLegendaryWeapon)
	{
		PreviousWeaponSlotTag = InBattleData.BattleWeaponSkill.CurrentWeaponSlotTag;
	}

	InBattleData.BattleWeaponSkill.CurrentWeaponSlotTag = Arcanum::Items::ItemSlot::Weapon::Legendary;
	bUsingLegendaryWeapon = true;
}

void UBattlefieldManagerSubsystem::EndLegendaryWeaponMode()
{
	InBattleData.BattleWeaponSkill.CurrentWeaponSlotTag = PreviousWeaponSlotTag;
	bUsingLegendaryWeapon = false;
}

USkeletalMesh* UBattlefieldManagerSubsystem::GetCurrentWeaponMesh() const
{
	const FBattleCharacterData* selectedCharacter = GetSelectedCharacterData();
	if (!selectedCharacter) return nullptr;

	const FGameplayTag currentWeaponSlotTag = InBattleData.BattleWeaponSkill.CurrentWeaponSlotTag;
	if (!currentWeaponSlotTag.IsValid()) return nullptr;

	const FGuid* weaponGuid = nullptr;
	if (currentWeaponSlotTag == Arcanum::Items::ItemSlot::Weapon::Legendary)
	{
		weaponGuid = selectedCharacter->LegendaryWeaponSlots.Find(Arcanum::Items::ItemSlot::Weapon::Legendary);
	}
	else
	{
		weaponGuid = selectedCharacter->WeaponSlots.Find(currentWeaponSlotTag);
	}
	if (!weaponGuid || !weaponGuid->IsValid()) return nullptr;

	const FEquipmentInfo* foundEquip = FindEquipmentByGuid(InBattleData.PlayerData, *weaponGuid);
	if (!foundEquip) return nullptr;

	const FDTItemCatalogRow* catalogRow = FPlayerAccountService::FindItemCatalogRowByTag(this, foundEquip->ItemTag);
	if (!catalogRow) return nullptr;
	if (catalogRow->DetailRowName.IsNone()) return nullptr;

	UGameDataSubsystem* gameDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGameDataSubsystem>();
	if (!gameDataSubsystem) return nullptr;

	const FDTEquipmentInfoRow* equipRow = gameDataSubsystem->GetRow<FDTEquipmentInfoRow>(Arcanum::DataTable::Equipment, catalogRow->DetailRowName);
	if (!equipRow) return nullptr;

	return equipRow->SkeletalMesh.LoadSynchronous();
}

USkeletalMesh* UBattlefieldManagerSubsystem::GetLegendaryWeaponMesh() const
{
	const FBattleCharacterData* selectedCharacter = GetSelectedCharacterData();
	if (!selectedCharacter) return nullptr;

	const FGuid* weaponGuid = selectedCharacter->LegendaryWeaponSlots.Find(Arcanum::Items::ItemSlot::Weapon::Legendary);
	if (!weaponGuid || !weaponGuid->IsValid()) return nullptr;

	const FEquipmentInfo* foundEquip = FindEquipmentByGuid(InBattleData.PlayerData, *weaponGuid);
	if (!foundEquip) return nullptr;

	const FDTItemCatalogRow* catalogRow = FPlayerAccountService::FindItemCatalogRowByTag(this, foundEquip->ItemTag);
	if (!catalogRow) return nullptr;

	if (catalogRow->DetailRowName.IsNone()) return nullptr;

	UGameDataSubsystem* gameDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGameDataSubsystem>();
	if (!gameDataSubsystem) return nullptr;

	const FDTEquipmentInfoRow* equipRow = gameDataSubsystem->GetRow<FDTEquipmentInfoRow>(Arcanum::DataTable::Equipment, catalogRow->DetailRowName);
	if (!equipRow) return nullptr;

	return equipRow->SkeletalMesh.LoadSynchronous();
}

FGameplayTag UBattlefieldManagerSubsystem::GetCurrentWeaponSlotTypeTag() const
{
	const FBattleCharacterData* selectedCharacter = GetSelectedCharacterData();
	if (!selectedCharacter) return FGameplayTag();

	const FGameplayTag currentWeaponSlotTag = InBattleData.BattleWeaponSkill.CurrentWeaponSlotTag;
	if (!currentWeaponSlotTag.IsValid()) return FGameplayTag();

	const FGuid* weaponGuid = nullptr;
	if (currentWeaponSlotTag == Arcanum::Items::ItemSlot::Weapon::Legendary)
	{
		weaponGuid = selectedCharacter->LegendaryWeaponSlots.Find(Arcanum::Items::ItemSlot::Weapon::Legendary);
	}
	else
	{
		weaponGuid = selectedCharacter->WeaponSlots.Find(currentWeaponSlotTag);
	}
	if (!weaponGuid || !weaponGuid->IsValid()) return FGameplayTag();

	const FEquipmentInfo* foundEquip = FindEquipmentByGuid(InBattleData.PlayerData, *weaponGuid);
	if (!foundEquip) return FGameplayTag();

	const FDTItemCatalogRow* catalogRow = FPlayerAccountService::FindItemCatalogRowByTag(this, foundEquip->ItemTag);
	if (!catalogRow) return FGameplayTag();
	if (catalogRow->DetailRowName.IsNone())	return FGameplayTag();

	UGameDataSubsystem* gameDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGameDataSubsystem>();
	if (!gameDataSubsystem) return FGameplayTag();

	const FDTEquipmentInfoRow* equipRow = gameDataSubsystem->GetRow<FDTEquipmentInfoRow>(Arcanum::DataTable::Equipment, catalogRow->DetailRowName);
	if (!equipRow) return FGameplayTag();

	return equipRow->SlotTag;
}

const FSkillInfo* UBattlefieldManagerSubsystem::GetCurrentBasicAttackSkillInfo() const
{
	const FGameplayTag skillTag = GetCurrentBasicAttackSkillTag();
	if (!skillTag.IsValid()) return nullptr;

	UGameDataSubsystem* gameDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGameDataSubsystem>();
	if (!gameDataSubsystem) return nullptr;

	const FDTSkillsDataRow* skillRow = gameDataSubsystem->GetRow<FDTSkillsDataRow>(Arcanum::DataTable::SkillData, GetLeafNameFromTag(skillTag));
	if (!skillRow) return nullptr;

	return &skillRow->SkillData;
}

const FSkillInfo* UBattlefieldManagerSubsystem::GetCurrentBasicSkillInfo() const
{
	const FGameplayTag skillTag = GetCurrentBasicSkillTag();
	if (!skillTag.IsValid()) return nullptr;

	UGameDataSubsystem* gameDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGameDataSubsystem>();
	if (!gameDataSubsystem) return nullptr;

	const FDTSkillsDataRow* skillRow = gameDataSubsystem->GetRow<FDTSkillsDataRow>(Arcanum::DataTable::SkillData, GetLeafNameFromTag(skillTag));
	if (!skillRow) return nullptr;

	return &skillRow->SkillData;
}

const FBattleSkillData* UBattlefieldManagerSubsystem::GetCurrentBasicAttackSkillData() const
{
	const FBattleWeaponSkillData& battleWeaponSkill = InBattleData.BattleWeaponSkill;

	if (battleWeaponSkill.CurrentWeaponSlotTag == Arcanum::Items::ItemSlot::Weapon::Slot1)
	{
		return &battleWeaponSkill.WeaponSlot1BasicAttackSkill;
	}

	if (battleWeaponSkill.CurrentWeaponSlotTag == Arcanum::Items::ItemSlot::Weapon::Slot2)
	{
		return &battleWeaponSkill.WeaponSlot2BasicAttackSkill;
	}

	return nullptr;
}

const FBattleSkillData* UBattlefieldManagerSubsystem::GetCurrentBasicSkillData() const
{
	const FBattleWeaponSkillData& battleWeaponSkill = InBattleData.BattleWeaponSkill;

	if (battleWeaponSkill.CurrentWeaponSlotTag == Arcanum::Items::ItemSlot::Weapon::Slot1)
	{
		return &battleWeaponSkill.WeaponSlot1BasicSkill;
	}

	if (battleWeaponSkill.CurrentWeaponSlotTag == Arcanum::Items::ItemSlot::Weapon::Slot2)
	{
		return &battleWeaponSkill.WeaponSlot2BasicSkill;
	}

	return nullptr;
}

const FBattleSkillData* UBattlefieldManagerSubsystem::GetCurrentLegendarySkillData() const
{
	return &InBattleData.BattleWeaponSkill.LegendaryUltimateSkill;
}

bool UBattlefieldManagerSubsystem::HasEquippedFullSet(const FGameplayTag& InSetRootTag) const
{
	if (!InSetRootTag.IsValid()) return false;

	const FPlayerData& playerData = InBattleData.PlayerData;
	const FBattleCharacterData* selectedCharacter = GetSelectedCharacterData();
	if (!selectedCharacter) return false;

	int32 matchCount = 0;

	for (const TPair<FGameplayTag, FGuid>& armorPair : selectedCharacter->ArmorSlots)
	{
		const FGuid& itemGuid = armorPair.Value;
		if (itemGuid.IsValid())
		{
			const FEquipmentInfo* foundEquip = FindEquipmentByGuid(playerData, itemGuid);
			if (foundEquip)
			{
				if (foundEquip->ItemTag.MatchesTag(InSetRootTag))
				{
					matchCount++;
				}
			}
		}
	}

	return matchCount >= 4;
}

float UBattlefieldManagerSubsystem::FindSkillCastTime(const FGameplayTag& InSkillTag, int32 InSkillLevel) const
{
	if (!InSkillTag.IsValid()) return 0.0f;

	UGameDataSubsystem* gameDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGameDataSubsystem>();
	if (!gameDataSubsystem) return 0.0f;

	const FDTSkillsDataRow* skillRow = gameDataSubsystem->GetRow<FDTSkillsDataRow>(Arcanum::DataTable::SkillData, GetLeafNameFromTag(InSkillTag));
	if (!skillRow) return 0.0f;

	const FSkillInfo& skillInfo = skillRow->SkillData;

	for (const FLevelModifierEntry& levelModifier : skillInfo.LevelModifiers)
	{
		if (levelModifier.Level == InSkillLevel)
		{
			return levelModifier.CastTime;
		}
	}

	return 0.0f;
}

float UBattlefieldManagerSubsystem::FindSkillCooldown(const FGameplayTag& InSkillTag, int32 InSkillLevel) const
{
	if (!InSkillTag.IsValid()) return 0.0f;

	UGameDataSubsystem* gameDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGameDataSubsystem>();
	if (!gameDataSubsystem) return 0.0f;

	const FDTSkillsDataRow* skillRow = gameDataSubsystem->GetRow<FDTSkillsDataRow>(Arcanum::DataTable::SkillData, GetLeafNameFromTag(InSkillTag));
	if (!skillRow) return 0.0f;

	const FSkillInfo& skillInfo = skillRow->SkillData;

	for (const FLevelModifierEntry& levelModifier : skillInfo.LevelModifiers)
	{
		if (levelModifier.Level == InSkillLevel)
		{
			return levelModifier.Cooldown;
		}
	}

	return 0.0f;
}

UTexture2D* UBattlefieldManagerSubsystem::FindSkillIcon(const FGameplayTag& InSkillTag) const
{
	if (!InSkillTag.IsValid()) return nullptr;

	UGameDataSubsystem* gameDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UGameDataSubsystem>();
	if (!gameDataSubsystem) return nullptr;

	if (const FDTSkillsDataRow* skillRow = gameDataSubsystem->GetRow<FDTSkillsDataRow>(
		Arcanum::DataTable::SkillData,
		GetLeafNameFromTag(InSkillTag)))
	{
		return skillRow->SkillData.Icon.LoadSynchronous();
	}

	return nullptr;
}
