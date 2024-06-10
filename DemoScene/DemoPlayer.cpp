#include "DemoPlayer.h"
#include "Model.h"
#include "Game.h"
#include "EffekseerManager.h"
#include "Load.h"

namespace
{
	// 拡大率
	constexpr float kScale = 2.0f;
	// 移動速度
	constexpr float kMoveSpeed = 32.0f;
	constexpr float kSideMoveSpeed = 1.5f;
	// 回転
	constexpr float kRotSpeed = 0.001f;
	constexpr float kRotaMax = DX_PI_F / 8;
	// 高さ
	constexpr float kHeightSpeed = 0.005f;
	constexpr float kHeightSpeedMax = 1.0f;
	constexpr float kHeightMax = 60.0f;
	// 左右移動のフレームカウント値
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

	// モデルの位置設定
	m_pModel->SetPosition(m_status.pos);
	// 拡大率設定
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

	// 回転処理
	UpdateRot();
	// 移動処理
	UpdateMove();

	m_pModel->SetRotation(m_status.rota);
	m_pModel->SetPosition(m_status.pos);

	// エフェクト再生
	EffekseerManager::GetInstance().SetEffectRota(EffectType::EngineFire, m_status.rota);
}

void DemoPlayer::Draw()
{
	// モデル描画
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
	// Y軸移動
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
	// 上下移動範囲
	if (m_status.pos.y > kHeightMax)
	{
		m_isUp = false;
	}
	else if (m_status.pos.y < -kHeightMax)
	{
		m_isUp = true;
	}

	// X軸移動
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

	// Z軸移動
	m_status.pos.z += kMoveSpeed;
}
