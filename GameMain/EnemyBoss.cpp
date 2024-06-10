#include "EnemyBoss.h"
#include "Model.h"
#include "EffekseerManager.h"
#include "SoundManager.h"
#include "Shot.h"
#include "Load.h"

namespace
{
	// �{�X�̉�]���x
	constexpr float kRotateSpeed = 0.02f;
	// �����蔻�蔼�a
	constexpr float kCollisionRadius = 600.0f;
	// �g�嗦
	constexpr float kScale = 5.0f;
	// �{�X�̖��G����
	constexpr int kInvTime = 1;
	// �{�X�̎��S��̖��G����
	constexpr int kDeadInvTime = 60 * 6;
	// �{�X�̍U���Ԋu
	constexpr int kShotDelay = 60;
	// �{�X�̃X�|�[�����鍂��
	constexpr float kSpawnHeight = 10000.0f;
	// �{�X�̍폜���鍂��
	constexpr float kDeleteHeight = 7500.0f;
}

EnemyBoss::EnemyBoss(VECTOR spawnPos, float speed, int maxHp):
	m_updateFunc(&EnemyBoss::SpawnUpdate),
	m_fixedPos(Game::kVecZero),
	m_targetPos(Game::kVecZero),
	m_angle(0.0f),
	m_rotaSpeed(kRotateSpeed),
	m_shotDelay(kShotDelay),
	m_maxHp(maxHp),
	m_isDead(false),
	m_pModel(nullptr)
{
	// �X�e�[�^�X������
	m_status.hp = m_maxHp;
	m_status.pos = VGet(spawnPos.x, spawnPos.y + kSpawnHeight, spawnPos.z);
	m_fixedPos = spawnPos;
	m_status.moveSpeed = speed;
	m_status.radius = kCollisionRadius;
	// ���f������
	m_pModel = std::make_shared<Model>(MV1DuplicateModel(Load::GetInstance().GetHandle("ufo")));
	m_pModel->SetPosition(m_status.pos);
	m_pModel->SetModelLocalPos(VGet(0.0f, -51.0f, 0.0f));
	m_pModel->SetScale(kScale);
	m_pModel->SetRotation(Game::kVecZero);
}

EnemyBoss::~EnemyBoss()
{
}

void EnemyBoss::Update()
{
	// ���G���ԍX�V
	if (m_status.invTime > 0)
	{
		m_status.invTime--;
	}

	// Update�����̃����o�֐��|�C���^
	(this->*m_updateFunc)();

	// �p�x�X�V
	m_angle -= m_rotaSpeed;
	if(m_angle < -DX_PI_F * 2)
	{
		m_angle = 0.0f;
	}

	// �ړ�
	m_status.pos.z += m_status.moveSpeed;
	// ���W�X�V
	m_pModel->SetPosition(m_status.pos);
	// ��]
	m_pModel->SetRotation(VGet(0.0f, m_angle, 0.0f));

	// �G�t�F�N�g���W�X�V
	if (m_status.isDead)
	{
		EffekseerManager::GetInstance().SetEffectPosition(EffectType::Dead, m_status.pos);
	}
}

void EnemyBoss::Draw()
{
	// �`��
	if (m_status.invTime % 2 == 0)
	{
		m_pModel->Draw();
	}

	// �����蔻��
	//DrawSphere3D(VAdd(m_status.pos, m_status.localPos), m_status.radius, 16, GetColor(255, 255, 255), GetColor(255, 255, 255), false);
}

void EnemyBoss::OnDamage()
{
	if (m_status.invTime > 0)
	{
		return;
	}

	// ���G���Ԑݒ�
	m_status.invTime = kInvTime;
	// �_���[�W���Đ�
	SoundManager::GetInstance().PlaySE(SoundType::damage);
	// �_���[�W�G�t�F�N�g�Đ�
	EffekseerManager::GetInstance().CreateEffect(EffectType::BossDamage, false, this);

	// �_���[�W����
	m_status.hp--;
	if (m_status.hp <= 0)
	{
		OnDead();
	}
}

void EnemyBoss::OnDead()
{
	// ���S���̍X�V����
	m_updateFunc = &EnemyBoss::DeadUpdate;
	// ���G���Ԑݒ�
	m_status.invTime = kDeadInvTime;
	// ���S�t���OON
	m_status.isDead = true;
	// ���S���̃G�t�F�N�g����
	EffekseerManager::GetInstance().CreateEffect(EffectType::Dead, false, this);
	// ���S���Đ�
	SoundManager::GetInstance().PlaySE(SoundType::dead);
}

void EnemyBoss::DeleteDisableShot()
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

void EnemyBoss::SpawnUpdate()
{
	m_status.pos.y -= 24.0f;
	if (m_status.pos.y <= m_fixedPos.y)
	{
		m_status.pos.y = m_fixedPos.y;
		m_updateFunc = &EnemyBoss::NormalUpdate;
	}
}

void EnemyBoss::NormalUpdate()
{
	m_shotDelay--;
	if (m_shotDelay < 0)
	{
		// �V���b�g����
		m_pShots.push_back(std::make_shared<Shot>(m_status.pos, m_targetPos));
		// �V���b�g�G�t�F�N�g����
		EffekseerManager::GetInstance().CreateEffect(EffectType::EnemyShot, false, m_pShots.back().get());
		// �V���b�g���Đ�
		SoundManager::GetInstance().PlaySE(SoundType::shot);
		m_shotDelay = kShotDelay;
	}

	// �e�̍X�V
	for (auto& shot : m_pShots)
	{
		shot->Update();
	}

	// �V���b�g�폜
	DeleteDisableShot();
}

void EnemyBoss::DeadUpdate()
{
	// ��������
	m_status.pos.y -= 24.0f;
	// ��]���x�X�V
	m_rotaSpeed += 0.005f;

	if (m_status.pos.y < -kDeleteHeight)
	{
		// �{�X�̖�����
		m_status.isEnabled = false;
		// �G�t�F�N�g�̒�~
		EffekseerManager::GetInstance().SetEffectPosition(EffectType::Dead, Game::kVecZero);
		EffekseerManager::GetInstance().StopEffect(EffectType::Dead);
	}
}
