#include "ObjectEnemyBase.h"
#include "Model.h"
#include "Shot.h"
#include "EffekseerManager.h"
#include "SoundManager.h"

ObjectEnemyBase::ObjectEnemyBase() :
	StageObjectBase(StageObjType::Enemy, "satellite"),
	m_targetPos(Game::kVecZero),
	m_shotDelay(),
	m_pShots()
{
	// �X�e�[�W�w�i�I�u�W�F�N�g�̊g�嗦�ݒ�
	m_status.scale = kScale;
	// ���f���̃��[�J�����W�𒲐�
	m_pModel->SetModelLocalPos(VGet(0.0f, -552.0f, 0.0f));
	// �X�e�[�W�w�i�I�u�W�F�N�g�̊g�嗦�ݒ�
	m_pModel->SetScale(m_status.scale);
}

ObjectEnemyBase::~ObjectEnemyBase()
{
}

void ObjectEnemyBase::Update()
{
	// �V���b�g�����Ԋu�̍X�V
	m_shotDelay--;
	if (m_shotDelay < 0)
	{
		if (VSize(VSub(m_status.pos, m_targetPos)) < kShotDistanceMax
			 && VSize(VSub(m_status.pos, m_targetPos)) > kShotDistanceMin)
		{
			// �V���b�g����
			CreateShot();
		}
	}

	// �e�̍X�V
	for (auto& shot : m_pShots)
	{
		shot->Update();
	}

	// �V���b�g�폜
	DeleteDisableShot();

	// �x�[�X�̍X�V����
	StageObjectBase::Update();
}

void ObjectEnemyBase::CreateShot()
{
	// �V���b�g����
	m_pShots.push_back(std::make_shared<Shot>(m_status.pos, m_targetPos));
	// �V���b�g�̑��x�ݒ�
	m_pShots.back()->SetSpeed(kShotSpeed);
	// �V���b�g�G�t�F�N�g����
	EffekseerManager::GetInstance().CreateEffect(EffectType::EnemyShot, false, m_pShots.back().get());
	// �V���b�g���Đ�
	SoundManager::GetInstance().PlaySE(SoundType::shot);
	// �V���b�g�����Ԋu�̐ݒ�
	m_shotDelay = GetRand(kShotDelay);
}

void ObjectEnemyBase::DeleteDisableShot()
{
	// ���Ȃ��Ȃ����V���b�g�͔r��
	auto rmIt = std::remove_if(m_pShots.begin(), m_pShots.end(),
		[](const std::shared_ptr<Shot>& shot)
		{
			return !shot->IsEnabled();
		});

	// ���ۂɔ͈͂��w�肵�č폜
	m_pShots.erase(rmIt, m_pShots.end());
}

void ObjectEnemyBase::DeleteAllShot()
{
	// �S�V���b�g�폜
	m_pShots.clear();
}
