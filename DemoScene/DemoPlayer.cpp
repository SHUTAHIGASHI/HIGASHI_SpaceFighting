#include "DemoPlayer.h"
#include "Model.h"
#include "Game.h"
#include "EffekseerManager.h"
#include "Load.h"

namespace
{
	// �g�嗦
	constexpr float kScale = 2.0f;
	// �ړ����x
	constexpr float kMoveSpeed = 32.0f;
	constexpr float kSideMoveSpeed = 1.5f;
	// ��]
	constexpr float kRotSpeed = 0.001f;
	constexpr float kRotaMax = DX_PI_F / 8;
	// ����
	constexpr float kHeightSpeed = 0.005f;
	constexpr float kHeightSpeedMax = 1.0f;
	constexpr float kHeightMax = 60.0f;
	// ���E�ړ��̃t���[���J�E���g�l
	constexpr int kMoveFrameCountMax = 60 * 20;
}

DemoPlayer::DemoPlayer():
	m_rotSpeed(kRotSpeed),
	m_heightSpeed(kHeightSpeed),
	m_isUp(true),
	m_isRight(false),
	m_moveFrameCount(0),
	m_hModel(-1),
	m_pModel(std::make_shared<Model>(MV1DuplicateModel(Load::GetInstance().GetHandle("ship"))))
{
	m_status.scale = kScale;

	// ���f���̈ʒu�ݒ�
	m_pModel->SetPosition(m_status.pos);
	// �g�嗦�ݒ�
	m_pModel->SetScale(m_status.scale);
}

DemoPlayer::~DemoPlayer()
{
}

void DemoPlayer::Update()
{
	m_moveFrameCount++;
	if (m_moveFrameCount > kMoveFrameCountMax)
	{
		m_moveFrameCount = 0;
		m_isRight = !m_isRight;
	}

	// ��]����
	UpdateRot();
	// �ړ�����
	UpdateMove();

	m_pModel->SetRotation(m_status.rota);
	m_pModel->SetPosition(m_status.pos);

	// �G�t�F�N�g�Đ�
	EffekseerManager::GetInstance().SetEffectRota(EffectType::EngineFire, m_status.rota);
}

void DemoPlayer::Draw()
{
	// ���f���`��
	m_pModel->Draw();
}

void DemoPlayer::UpdateRot()
{
	if (!m_isUp)
	{
		m_status.rota.x += kRotSpeed;
	}
	else
	{
		m_status.rota.x -= kRotSpeed;
	}

	if (m_status.rota.x > kRotaMax)
	{
		m_status.rota.x = kRotaMax;
	}
	else if (m_status.rota.x < -kRotaMax)
	{
		m_status.rota.x = -kRotaMax;
	}
}

void DemoPlayer::UpdateMove()
{
	// Y���ړ�
	if (m_isUp)
	{
		m_heightSpeed += kHeightSpeed;
		if (m_heightSpeed > kHeightSpeedMax)
		{
			m_heightSpeed = kHeightSpeedMax;
		}
	}
	else
	{
		m_heightSpeed -= kHeightSpeed;
		if (m_heightSpeed < -kHeightSpeedMax)
		{
			m_heightSpeed = -kHeightSpeedMax;
		}
	}
	m_status.pos.y += m_heightSpeed;
	// �㉺�ړ��͈�
	if (m_status.pos.y > kHeightMax)
	{
		m_isUp = false;
	}
	else if (m_status.pos.y < -kHeightMax)
	{
		m_isUp = true;
	}

	// X���ړ�
	if (m_isRight)
	{
		m_status.pos.x += kSideMoveSpeed;
		if (m_status.pos.x > 1000.0f)
		{
			m_status.pos.x = 1000.0f;
		}
	}
	else
	{
		m_status.pos.x -= kSideMoveSpeed;
		if (m_status.pos.x < 0.0f)
		{
			m_status.pos.x = 0.0f;
		}
	}

	// Z���ړ�
	m_status.pos.z += kMoveSpeed;
}
