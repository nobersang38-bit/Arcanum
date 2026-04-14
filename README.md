1. 프로젝트 개요
───────────────────────────────────────────────────────────────────────────────────────────
쿼터뷰 스타일의 전략 자동 디펜스 RPG,
실시간 전투와 캐릭터 성장, 장비, 강화, 상점, 가챠 시스템을 구현한 Unreal Engine 5 프로젝트.

수동 조작과 자동 전투 전환을 지원,
장비, 스킬, 재화, 가챠를 연계한 성장 구조,
GameplayTag, DataTable, Subsystem 기반의 데이터 중심 아키텍처를 적용.

개발 기간: 2026.02.03 ~ 2026.04.03
개발 인원: 4명


2. 주요 기능
───────────────────────────────────────────────────────────────────────────────────────────
2.1 Battle & Control
● 쿼터뷰 실시간 디펜스 전투
● 수동 조작 / 자동 전투 전환
● 드래그 기반 유닛 소환 및 전장 개입
● Behavior Tree 기반 타겟 선택 및 스킬 실행
● RVO 기반 군집 이동 처리

2.2 Character Growth & Equipment
● 캐릭터 획득 및 성장 구조
● 무기 / 방어구 장착 및 장비 슬롯 관리
● 장비에 따른 능력치 변화와 전투 스타일 변화
● 외형과 스킬이 함께 바뀌는 장비 연동 구조

2.3 Inventory / Shop / Enhancement
● 장비형 / 소모형 아이템 분리 관리
● 상점 카테고리별 상품 구성 및 구매 처리
● DataTable 기반 단계형 장비 강화 시스템
● 강화 단계에 따른 랜덤 옵션 및 스탯 반영
● 우편함 기반 보상 보관 로직 지원

2.4 Gacha System
● GameplayTag 기반 배너 분류
● DataTable 기반 확률, 픽업, 비용 관리
● 천장(Pity) 및 픽업 확정 보정 시스템
● 결과 연출과 UI를 포함한 가챠 시퀀스 구현

2.5 Data & UI Architecture
● UGameDataSubsystem 기반 중앙 데이터 관리
● GameplayTag + DataTable 기반 Data-Driven 구조
● SaveGame 기반 플레이어 데이터 저장 / 로드
● 중앙 집중형 UI 바인딩 및 화면 상태 관리


3. 기술 요소
──────────────────────────────────────────────────────────────────────────────────────────
● Unreal Engine 5.5.4
● C++
● Blueprint / UMG
● GameplayTag
● DataTable
● Data-Driven Architecture
● Behavior Tree
● Subsystem Architecture
● Lite GAS System
● Object Pooling
● SaveGame Serialization
● Git / GitHub
