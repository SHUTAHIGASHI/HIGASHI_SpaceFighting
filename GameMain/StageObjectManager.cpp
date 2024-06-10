#include "StageObjectManager.h"
#include "StageObjectBase.h"
#include "Stage.h"
#include "ObjectBase.h"
#include "ObjectRock.h"
#include "ObjectItem.h"
#include "ObjectEnemy.h"
#include "Player.h"
#include "Shot.h"
#include "GameDataManager.h"
#include "UiManager.h"
#include "EffekseerManager.h"

namespace
{
	// �I�u�W�F�N�g�����Ԋu
	constexpr float kObjectCreateInterval = 2000.0f;
	// �A�C�e�������̊m��
	constexpr int kItemCreateRate = 20;
}

StageObjectManager::StageObjectManager():
	m_playerPos(Game::kVecZero),
	m_pObjects(),
	m_pStage(nullptr),
	m_pUi(nullptr)
{
}

StageObjectManager::~StageObjectManager()
{
	// �I�u�W�F�N�g�̃N���A
	m_pObjects.clear();
	// �X�e�[�W�AUI�̉��
	m_pStage = nullptr;
	m_pUi = nullptr;
}

void StageObjectManager::Init()
{
	// �����Ȃ�
}

void StageObjectManager::Update()
{
	// �I�u�W�F�N�g�X�V
	for (auto& obj : m_pObjects)
	{
		obj->Update();
		// �G�̏ꍇ�̓v���C���[�̍��W��ݒ�
		if (obj->GetType() == StageObjType::Enemy)
		{
			// �^�[�Q�b�g���W�ݒ�
			dynamic_cast<ObjectEnemyBase*>(obj.get())->SetTargetPos(m_playerPos);
		}
	}

	// �����ɂȂ����I�u�W�F�N�g�͔r��
	DeleteDisableObject();

#ifdef _DEBUG
	//printfDx("ObjectNum:%d\n", m_pObjects.size());
#endif
}

void StageObjectManager::Draw()
{
	// �I�u�W�F�N�g�`��
	for (auto& obj : m_pObjects)
	{
		obj->Draw();
	}
}

void StageObjectManager::CheckPlayerCollision(Player* player)
{
	// �I�u�W�F�N�g�Ƃ̓����蔻��
	for (auto& obj : m_pObjects)
	{
		// �I�u�W�F�N�g�̍X�V����
		obj->Update();
		// �v���C���[�Ƃ̓����蔻��`�F�b�N
		if (obj->CheckCollision(player))
		{
			// ��_���[�W���̏���
			obj->OnCollision();
			// �v���C���[�ɏՓ˂����Ƃ��̏���
			player->OnCollision(obj->GetType());
			continue;
		}

		// �v���C���[�̃V���b�g�Ƃ̓����蔻��`�F�b�N
		for (auto& shot : player->GetShot())
		{
			// �V���b�g�Ƃ̓����蔻��
			if (shot->CheckCollision(obj.get()))
			{
				// �X�R�A���Z
				if (obj->GetType() == StageObjType::Enemy)
				{
					// �G���j���̃X�R�A���Z
					GameDataManager::GetInstance().AddPoint(ScoreInfo::ENEMY_SCORE);
					// UI����
					m_pUi->OnGetPoint(ScoreInfo::ENEMY_SCORE);
					// �X�e�[�W�̓G���j���J�E���g
					m_pStage->CountKillEnemy();
				}
				else if (obj->GetType() == StageObjType::Rock)
				{
					// ��j�󎞂̃X�R�A���Z
					GameDataManager::GetInstance().AddPoint(ScoreInfo::ROCK_SCORE);
					// UI����
					m_pUi->OnGetPoint(ScoreInfo::ROCK_SCORE);
				}
				else
				{
					continue;
				}

				// ��_���[�W���̏���
				obj->OnCollision();
				// �V���b�g�̍폜
				shot->OnHit();

				// �A�C�e���̐���
				if (GetRand(kItemCreateRate) == 0)
				{
					// �A�C�e�����W�ۑ�
					m_itemCreatePos.push_back(obj->GetPos());
				}
			}
		}

		// �G�̏ꍇ�̓V���b�g�̓����蔻��`�F�b�N
		if (obj->GetType() == StageObjType::Enemy)
		{
			for (auto& shot : dynamic_cast<ObjectEnemyBase*>(obj.get())->GetShots())
			{
				if (player->CheckCollision(shot.get()))
				{
					// ��_���[�W���̏���
					player->OnDamage();
					// �V���b�g�폜
					shot->OnHit();
					continue;
				}
			}
		}
	}

	// �A�C�e������
	for (auto& pos : m_itemCreatePos)
	{
		CreateObject("item", pos);
		// �G�t�F�N�g����
		EffekseerManager::GetInstance().CreateEffect(EffectType::Heal, false, pos);
	}
	// �������W�N���A
	m_itemCreatePos.clear();
}

void StageObjectManager::CreateObject(std::string objName, VECTOR pos)
{
	// �I�u�W�F�N�g����
	if (objName == "rock")
	{
		// �␶��
		m_pObjects.push_back(std::make_shared<ObjectRock>());
	}
	else if (objName == "item")
	{
		// �A�C�e������
		m_pObjects.push_back(std::make_shared<ObjectItem>());
	}
	else if (objName == "enemy")
	{
		// �G����
		m_pObjects.push_back(std::make_shared<ObjectEnemy>());
	}

	// ���W�ݒ�
	m_pObjects.back()->SetPos(pos);
}

void StageObjectManager::DeleteDisableObject()
{
	// �����ɂȂ����I�u�W�F�N�g�͔r��
	auto rmIt = std::remove_if(m_pObjects.begin(), m_pObjects.end(),
		[](const std::shared_ptr<StageObjectBase>& obj)
		{
			return !obj->IsEnabled();
		});

	// ���ۂɔ͈͂��w�肵�č폜
	m_pObjects.erase(rmIt, m_pObjects.end());
}
