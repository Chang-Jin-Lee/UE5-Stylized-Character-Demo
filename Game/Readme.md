### ������Ʈ ����
- ��ǥ : ����ī�̺� �ϳ����� ĳ���� �ý��� �� �޺� �ý��� ����
- Unreal Engine 5.5 C++ ��� ���� ���� ������Ʈ
- UI�� Slate UI�� ����� ����
-  (DataAsset + ����ý��� + ������Ʈ)�� �Է�/����/AI/�� ���������� ����

### �ٽ� �ý��� ���
- DataAsset ��� ĳ���� �ý���
  - `UCharacterDefinition`(PrimaryDataAsset): �޽�/�ִ�/ĸ��/�̵�/ī�޶�/����(�޺�����ų�����塤Ʈ���̽���������) ����
  - `UCharacterSystemSettings`(DeveloperSettings): �⺻ ĳ����/���� ��ĵ ���/�⺻ �Է� ����
  - `UCharacterSelectionSubsystem`(GameInstanceSubsystem): ���� ��ĵ/����/����/���������� ����(�޽�/�ִ�/����/ī�޶�/���� �Ķ���� �ݿ�)
  - `AGamePlayerController`: ���� �� ����/�⺻ ĳ���� ����, �ܼ� `CharList`, `CharSelect` ����

- ����/�޺� �ý���
  - `UCharacterCombatComponent`: �޺� �Է� ť, �ִ� ��Ƽ����(AttackStart/End, ComboWindowOpen/Close), ��ų/���� ��Ÿ��, ��Ʈ Ʈ���̽�, ���� ���
  - ����/��ų ������ = ���� ������ + (������ Attack ���� �� AttackMultiplier)
  - �Է��� Enhanced Input���� �׼ǡ��̸� ����, ������ �ڻ꿡�� ���ε� ����

- �ɷ�ġ �ý���
  - `UAbilitySettings`(DeveloperSettings): �⺻ �ɷ�ġ Ű-��(Health/Attack/Defense/HealPower ��) ����, Ȯ�� ����
  - `UAbilityComponent`: ���� Ű ��� �� ����/��ȸ/����, `OnTakeAnyDamage` ������ Health ����

- �Է� �ý���(Enhanced Input)
  - `UCharacterInputConfig`(PrimaryDataAsset): �׼ǡ���ݼ�Ʈ/��ų/Ű ����, ��Ÿ�� IMC �������� ��� �ݿ�
  - ���� ��ġ: Possess/BeginPlay Ÿ�ֿ̹� MappingContext ������(���� ������ ����)

- AI/��
  - `AEnemyAIController`: Idle/Chase/Attack ����, �÷��̾� ���� ȸ��+`AddMovementInput` ��� �߰�, ��Ÿ� �� ����
  - `AEnemyCharacter` + `UEnemyDefinition`: �̵� �ӵ�/Ž�������� �ݰ�/����� ���� ��Ʈ ����

- UI/��(Slate)
  - `SPlayerOverlayWidget`: �»�� ����Ʈ �����(Ŀ���ҵ��� ���, ��ġ/ȸ��, Ability �ǽð� ǥ��)
  - `SShopWidget`, `SInventoryWidget`: ����/�κ��丮 ���(Ȯ�� ���)
  - `ANPCShopActor`: ���� �ݰ� �� F ��ȣ�ۿ� �� ���� â ����

- �÷��̾� �Է� ����
  - ī�޶� ��: ���콺 ��/���콺 �巡��(�� ���� ����)
  - Ctrl Ȧ��: ���콺 Ŀ�� ǥ�� + UI �Է� ��ȯ
  - Shift ������Ʈ: `MaxWalkSpeed` ��� ����(�׼� ������� ���ε�)
  - ���� ó��: Z < -500 �� ���� ���� �����

### ��� ���
- ĳ���� ����(�ܼ�)
  - `CharList` �� ��ϵ� ĳ���� ���� ����
  - `CharSelect CharacterDef:AssetName` �� ���� �� ��� ����/����
- �⺻ ����/����Ű(���� ����)
  - ����/��ų: ���콺 ����Ŭ��, ���� 1��
  - ī�޶� ��: ��/�巡��
  - ��ȣ�ۿ�: F (NPC ���� Ʈ���̽�)
  - �κ��丮: I (���)
  - Ŀ��/UI ���: Ctrl Ȧ��
  - ������Ʈ: Shift 

### ������� ���
- ������ �ֵ�(PrimaryDataAsset/DeveloperSettings): C++ �������� ���� �ű� ĳ����/�Է�/�ɷ�ġ Ȯ��
- ����ý���(GameInstanceSubsystem): ĳ���� ���� ���� ����(�� ��ȯ �� �ϰ���)
- Enhanced Input: ���� ��� �׼�/Ű ���� + ��Ÿ�� IMC�� �����ϰ� ��ġ�� ����(Ÿ�̹� �̽� ����)
- �ִ� ��Ƽ���� + �Է� ť: ��Ÿ/�޺� ������, �ǽð� ������ ��ȭ

### ���͸�
- `Source/Game/CharacterSystem/` �� Definition/Settings/Save/SelectionSubsystem
- `Source/Game/Combat/` �� CharacterCombatComponent(+ ��Ƽ���� ����/��Ʈ Ʈ���̽�)
- `Source/Game/Ability/` �� AbilitySettings, AbilityComponent
- `Source/Game/Player/` �� GamePlayerController(�ܼ�/��������/UI �Է�)
- `Source/Game/Enemy/` �� EnemyCharacter, EnemyAIController, EnemyDefinition
- `Source/Game/UI/` �� SPlayerOverlayWidget, SShopWidget, SInventoryWidget
- `Source/Game/NPC/` �� NPCShopActor

### ����/���� ��Ʈ
- UE 5.5, Enhanced Input ���
- ù ���� �� �Է� ������ �ʰ� ����� �� �־� Possess ���� ������ ���� ����

### ������
- �� ������Ʈ�� ���� �̷��� �����ߴ°����� �ڵ�/�ּ�/������ �ڻ����� �巯���� �� �����߽��ϴ�.
- ������ ���ο� �� ����(������ �ֵ�/Ȯ�� ����/������ �Է�/��Ȯ�� å�� �и�)�� �����մϴ�.