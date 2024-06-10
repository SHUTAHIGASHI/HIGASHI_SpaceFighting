#include "Stage.h"
#include "StageObjectBase.h"
#include "StageObjectManager.h"
#include "EnemyBoss.h"
#include "Player.h"
#include "UiManager.h"
#include "GameDataManager.h"
#include "Shot.h"

namespace
{
	// �w�i�I�u�W�F�N�g�̍폜����
	constexpr float kDeleteRange = 1000.0f;
	constexpr float kDeleteRangeAfterEnd = 12000.0f;
	// �I�u�W�F�N�g�̐�������
	constexpr float kObjectCreateDistance = 10000.0f;
	// �I�u�W�F�N�g�̐����͈�
	constexpr float kObjectCreateRangeX = Game::kStageWidth * 2;
	constexpr float kObjectCreateRangeY = Game::kStageHeight * 2;
	constexpr float kObjectCreateRangeZ = 10000.0f;
	// �G�̓��������ő吔
	constexpr int kEnemyMaxNum = 5;
}

Stage::Stage(StageData stageData) :
	m_stage(stageData),
	m_countFrame(0),
	m_killEnemyNum(0),
	m_deleteRange(kDeleteRange),
	m_stageLengthMax(stageData.length),
	m_isClear(false),
	m_pObjectManager(std::make_shared<StageObjectManager>()),
	m_pBoss(nullptr),
	m_pPlayer(nullptr),
	m_pUi(nullptr)
{
}

Stage::~Stage()
{
	// �{�X�폜
	if (m_pBoss != nullptr)
	{
		delete m_pBoss;
		m_pBoss = nullptr;
	}
}

void Stage::Init()
{
	// �I�u�W�F�N�g�Ǘ�������
	m_pObjectManager->SetStage(this);
	m_pObjectManager->SetUi(m_pUi);
	m_pObjectManager->Init();
	// �␶��
	for (int i = 0; i < m_stage.rockNum; i++)
	{
		// ��̐���
		m_pObjectManager->CreateObject("rock", GetRandomPos());
	}
	// UI�ɓG���j����ݒ�
	m_pUi->SetEnemyKillCount(m_stage.enemyNum);
	m_pUi->SetIsBossRate(false);
}

void Stage::Update()
{
	// �v���C���[�̈ʒu��ݒ�
	m_pObjectManager->SetPlayerPos(m_pPlayer->GetPos());
	// �I�u�W�F�N�g�Ǘ��X�V
	m_pObjectManager->Update();
	// �v���C���[�ƃI�u�W�F�N�g�̓����蔻��
	m_pObjectManager->CheckPlayerCollision(m_pPlayer);

	// �I�u�W�F�N�g�̍��W�Đݒ�
	for (auto& obj : m_pObjectManager->GetStageObject())
	{
		if (obj->GetPos().z < m_pPlayer->GetPos().z - m_deleteRange)
		{
			obj->SetPos(GetRandomPos());
		}
	}

	// �I�u�W�F�N�g���Ǘ�
	CheckObjectNum();

	// �{�X�X�V
	if (m_pBoss != nullptr)
	{
		UpdateBoss();
		m_pUi->SetBossHp(m_pBoss->GetHP());
		m_pUi->SetBossHpDrawPos(m_pBoss->GetPos());
	}
	else
	{
		m_stage.length -= m_pPlayer->GetSpeed();
		if (m_stage.length <= 0.0f && m_killEnemyNum >= m_stage.enemyNum)
		{
			// �X�e�[�W�i�s�x���Z�b�g
			m_stage.length = 0.0f;
			// �t�F�[�Y�ύX
			if (!m_isClear)
			{
				OnChangeProgress();
			}
		}
		m_pUi->SetStageRate(static_cast<int>(100 - (m_stage.length / m_stageLengthMax * 100)));
	}

	// �G���G���Ǘ�
	EnemyNumManager();

	// �X�e�[�W�N���A����`�F�b�N
	CheckStageClear();
}

void Stage::Draw()
{
	// �I�u�W�F�N�g�Ǘ��`��
	m_pObjectManager->Draw();
	// �{�X�`��
	if (m_pBoss != nullptr)
	{
		m_pBoss->Draw();
	}
}

void Stage::CountKillEnemy()
{
	m_killEnemyNum++;
	m_pUi->CountEnemyKill();
}

void Stage::OnStageClear()
{
	// �X�e�[�W�N���A���̏���
	if (!m_isClear)
	{
		GameDataManager::GetInstance().OnStageClear();
	}

	// �N���A����
	m_isClear = true;
}

void Stage::OnStageEnd()
{
	// �w�i�I�u�W�F�N�g�̍폜�͈͂�ύX
	m_deleteRange = kDeleteRangeAfterEnd;
}

void Stage::CheckObjectNum()
{
	int objCount = 0;
	// �I�u�W�F�N�g�����ݒ萔��菭�Ȃ��ꍇ�A����
	for (auto& obj : m_pObjectManager->GetStageObject())
	{
		if (obj->GetType() == StageObjType::Rock)
		{
			objCount++;
		}
	}

	if (objCount < m_stage.rockNum)
	{
		for (int i = 0; i < m_stage.rockNum - objCount; i++)
		{
			// �G�̐���
			m_pObjectManager->CreateObject("rock", GetRandomPos());
		}
	}
}

void Stage::CheckStageClear()
{
	// �{�X�����݂��Ȃ��X�e�[�W�̏ꍇ�A�X�e�[�W�̒�����0�ɂȂ�����N���A
	if (m_stage.length <= 0.0f && m_killEnemyNum >= m_stage.enemyNum)
	{
		// �{�X�����݂���X�e�[�W�̏ꍇ
		if (m_stage.isBoss)
		{
			if (m_pBoss != nullptr)
			{
				// �{�X�����������ꂽ�ꍇ�A�{�X���폜
				if (!m_pBoss->IsEnabled())
				{
					// �X�e�[�W�N���A����
					OnStageClear();
					// �{�X�폜
					delete m_pBoss;
					m_pBoss = nullptr;
					// �{�XUI�폜
					m_pUi->DeleteBossUI();
					// �X�R�A���Z
					GameDataManager::GetInstance().AddPoint(ScoreInfo::BOSS_SCORE);
				}
			}
		}
		else
		{
			m_stage.length = 0.0f;
			// �X�e�[�W�N���A����
			OnStageClear();
		}
	}
}

void Stage::EnemyNumManager()
{
	// �G�̓������������J�E���g
	int countEnemy = 0;
	for (auto& obj : m_pObjectManager->GetStageObject())
	{
		if (obj->GetType() == StageObjType::Enemy)
		{
			countEnemy++;
		}
	}

	// �G�̐���
	if (countEnemy < kEnemyMaxNum)
	{
		m_pObjectManager->CreateObject("enemy", GetRandomPos());
	}
}

void Stage::CreateBoss()
{
	// �{�X�̐������W
	VECTOR bossPos = VGet(0.0f, 0.0f, m_pPlayer->GetDistance() + 4000.0f);
	// �t�F�[�Y�ύX
	m_pBoss = new EnemyBoss(bossPos, m_pPlayer->GetSpeed(), m_stage.bossHp);
	// �{�XUI�ݒ�
	m_pUi->CreateBossUI(m_pBoss->GetMaxHp());
	m_pUi->SetBossHpDrawPos(m_pBoss->GetPos());
	m_pUi->SetIsBossRate(true);
}

void Stage::UpdateBoss()
{
	m_pBoss->SetSpeed(m_pPlayer->GetSpeed());
	m_pBoss->SetTargetPos(m_pPlayer->GetPos());
	m_pBoss->Update();

	// �v���C���[�̃V���b�g�ƃ{�X�̓����蔻��`�F�b�N
	for (auto& shot : m_pPlayer->GetShot())
	{
		if (m_pBoss->CheckCollision(shot.get()))
		{
			// ��_���[�W���̏���
			m_pBoss->OnDamage();
			// �V���b�g�폜
			shot->OnHit();
		}
	}

	// �{�X�̃V���b�g�ƃv���C���[�̓����蔻��`�F�b�N
	for (auto& shot : m_pBoss->GetShot())
	{
		if (m_pPlayer->CheckCollision(shot.get()))
		{
			// ��_���[�W���̏���
			m_pPlayer->OnDamage();
			// �V���b�g�폜
			shot->OnHit();
		}
	}
}

void Stage::OnChangeProgress()
{
	if (m_stage.isBoss)
	{
		CreateBoss();
	}
}

VECTOR Stage::GetRandomPos()
{
	// �I�u�W�F�N�g�̈ʒu��ݒ�
	VECTOR resultPos = Game::kVecZero;
	resultPos.x = static_cast<float>(GetRand(static_cast<int>(kObjectCreateRangeX)) - kObjectCreateRangeX / 2);
	resultPos.y = static_cast<float>(GetRand(static_cast<int>(kObjectCreateRangeY)) - kObjectCreateRangeY / 2);
	resultPos.z = m_pPlayer->GetPos().z + kObjectCreateRangeZ + static_cast<float>(GetRand(static_cast<int>(kObjectCreateRangeZ)));

	return resultPos;
}
