# UE5-Stylized-Character-Demo

https://www.youtube.com/watch?v=dnhVrL-So_Q

<table>
  <tr>
    <td align="center">
      <img src="https://github.com/user-attachments/assets/7741f645-7d10-4459-affd-521bd31afa8a" width="500" alt="Image 1"/><br/>
      <b>Post Process Volume off, custom depth off</b>
    </td>
    <td align="center">
      <img src="https://github.com/user-attachments/assets/bfa5a300-dd42-4322-82d6-d30429bd45d4" width="500" alt="Image 2"/><br/>
      <b>Post Process Volume on, custom depth off</b>
    </td>
    <td align="center">
      <img src="https://github.com/user-attachments/assets/ff810a98-d055-4f77-bbca-d63284ef867d" width="500" alt="Image 3"/><br/>
      <b>Post Process Volume on, custom depth on</b>
    </td>
  </tr>
</table>


### 프로젝트 개요
- 목표 : 블루아카이브 하나코의 캐릭터 시스템 및 콤보 시스템 구현
- Unreal Engine 5.5 C++ 기반 샘플 게임 프로젝트
- UI는 Slate UI를 사용해 구현
-  (DataAsset + 서브시스템 + 컴포넌트)와 입력/전투/AI/툴 파이프라인 구현

### 핵심 시스템 요약
- DataAsset 기반 캐릭터 시스템
  - `UCharacterDefinition`(PrimaryDataAsset): 메쉬/애님/캡슐/이동/카메라/전투(콤보·스킬·사운드·트레이스·데미지) 정의
  - `UCharacterSystemSettings`(DeveloperSettings): 기본 캐릭터/에셋 스캔 경로/기본 입력 설정
  - `UCharacterSelectionSubsystem`(GameInstanceSubsystem): 에셋 스캔/선택/저장/스폰·스왑 적용(메쉬/애님/물리/카메라/전투 파라미터 반영)
  - `AGamePlayerController`: 시작 시 저장/기본 캐릭터 스폰, 콘솔 `CharList`, `CharSelect` 지원

- 전투/콤보 시스템
  - `UCharacterCombatComponent`: 콤보 입력 큐, 애님 노티파이(AttackStart/End, ComboWindowOpen/Close), 스킬/공격 쿨타임, 히트 트레이스, 사운드 재생
  - 공격/스킬 데미지 = 스펙 데미지 + (공격자 Attack 스탯 × AttackMultiplier)
  - 입력은 Enhanced Input으로 액션→이름 매핑, 데이터 자산에서 바인딩 가능

- 능력치 시스템
  - `UAbilitySettings`(DeveloperSettings): 기본 능력치 키-값(Health/Attack/Defense/HealPower 등) 구성, 확장 자유
  - `UAbilityComponent`: 임의 키 기반 맵 저장/조회/가감, `OnTakeAnyDamage` 훅으로 Health 감소

- 입력 시스템(Enhanced Input)
  - `UCharacterInputConfig`(PrimaryDataAsset): 액션→공격세트/스킬/키 매핑, 런타임 IMC 생성으로 즉시 반영
  - 안전 장치: Possess/BeginPlay 타이밍에 MappingContext 재적용(간헐 미적용 방지)

- AI/적
  - `AEnemyAIController`: Idle/Chase/Attack 상태, 플레이어 향해 회전+`AddMovementInput` 기반 추격, 사거리 내 공격
  - `AEnemyCharacter` + `UEnemyDefinition`: 이동 속도/탐지·공격 반경/사용할 공격 세트 설정

- UI/툴(Slate)
  - `SPlayerOverlayWidget`: 좌상단 컴팩트 디버그(커스텀뎁스 토글, 위치/회전, Ability 실시간 표시)
  - `SShopWidget`, `SInventoryWidget`: 상점/인벤토리 목업(확장 기반)
  - `ANPCShopActor`: 감지 반경 내 F 상호작용 시 상점 창 오픈

- 플레이어 입력 개선
  - 카메라 줌: 마우스 휠/마우스 드래그(붐 길이 조절)
  - Ctrl 홀드: 마우스 커서 표시 + UI 입력 전환
  - Shift 스프린트: `MaxWalkSpeed` 배속 증가(액션 방식으로 바인딩)
  - 낙사 처리: Z < -500 시 현재 레벨 재시작

### 사용 방법
- 캐릭터 선택(콘솔)
  - `CharList` → 등록된 캐릭터 정의 나열
  - `CharSelect CharacterDef:AssetName` → 선택 및 즉시 스폰/스왑
- 기본 조작/단축키(변경 가능)
  - 공격/스킬: 마우스 왼쪽클릭, 숫자 1번
  - 카메라 줌: 휠/드래그
  - 상호작용: F (NPC 전방 트레이스)
  - 인벤토리: I (토글)
  - 커서/UI 모드: Ctrl 홀드
  - 스프린트: Shift 

### 기술스택 요약
- 데이터 주도(PrimaryDataAsset/DeveloperSettings): C++ 재컴파일 없이 신규 캐릭터/입력/능력치 확대
- 서브시스템(GameInstanceSubsystem): 캐릭터 선택 상태 유지(맵 전환 간 일관성)
- Enhanced Input: 에셋 기반 액션/키 매핑 + 런타임 IMC로 안전하게 합치는 구조(타이밍 이슈 방지)
- 애님 노티파이 + 입력 큐: 연타/콤보 안정성, 실시간 반응성 강화

### 디렉터리
- `Source/Game/CharacterSystem/` … Definition/Settings/Save/SelectionSubsystem
- `Source/Game/Combat/` … CharacterCombatComponent(+ 노티파이 로직/히트 트레이스)
- `Source/Game/Ability/` … AbilitySettings, AbilityComponent
- `Source/Game/Player/` … GamePlayerController(콘솔/오버레이/UI 입력)
- `Source/Game/Enemy/` … EnemyCharacter, EnemyAIController, EnemyDefinition
- `Source/Game/UI/` … SPlayerOverlayWidget, SShopWidget, SInventoryWidget
- `Source/Game/NPC/` … NPCShopActor

### 빌드/실행 노트
- UE 5.5, Enhanced Input 사용
- 첫 실행 시 입력 맵핑이 늦게 적용될 수 있어 Possess 시점 재적용 로직 포함

### 마무리
- 이 프로젝트는 “왜 이렇게 설계했는가”를 코드/주석/데이터 자산으로 드러내는 데 집중했습니다.
- 협업을 염두에 둔 구성(데이터 주도/확장 용이/안전한 입력/명확한 책임 분리)을 지향합니다.
