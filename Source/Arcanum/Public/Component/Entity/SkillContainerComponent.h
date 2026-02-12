#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTags/ArcanumTags.h"
#include "DataInfo/Skills/Data/FSkillInfo.h"
#include "DataInfo/Skills/DataTable/DTSkillMapping.h"
#include "SkillContainerComponent.generated.h"

class USkillBase;
class UDataTable;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCANUM_API USkillContainerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USkillContainerComponent();
protected:
	virtual void BeginPlay() override;
	
    /** 아이템 장착 시 호출: 태그를 받아 스킬 객체를 생성하고 추가함 */
    UFUNCTION(BlueprintCallable, Category = "Skill")
    void AddSkill(FGameplayTag SkillTag);

    /** 아이템 해제 시 호출: 스킬 객체를 제거함 */
    UFUNCTION(BlueprintCallable, Category = "Skill")
    void RemoveSkill(FGameplayTag SkillTag);

    /** 특정 태그의 스킬 실행 */
    UFUNCTION(BlueprintCallable, Category = "Skill")
    void ExecuteSkill(FGameplayTag SkillTag);

protected:
    /** 스킬 태그 -> 실행용 BP 클래스 매핑 테이블 (공장 역할) */
    UPROPERTY(EditDefaultsOnly, Category = "Skill|Data")
    TObjectPtr<UDataTable> SkillMappingTable;

    /** 실제 스킬 정보가 담긴 테이블 (FSkillInfo 보관용) */
    UPROPERTY(EditDefaultsOnly, Category = "Skill|Data")
    TObjectPtr<UDataTable> SkillInfoTable;

    /** 현재 캐릭터가 보유 중인 스킬 인스턴스들 */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill|Runtime")
    TMap<FGameplayTag, TObjectPtr<USkillBase>> SkillInstances;

private:
    /** 태그로 SkillInfo 데이터 찾아오기 */
    FSkillInfo* FindSkillInfo(FGameplayTag SkillTag);
};
