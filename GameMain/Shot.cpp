#include "Shot.h"
#include "EffekseerManager.h"

namespace
{
	// 弾の速度
	constexpr float kShotSpeed = 160.0f;
	// 弾の半径
	constexpr float kShotRadius = 64.0f;
	// 弾の有効時間
	constexpr int kShotTime = 60 * 15;
}

Shot::Shot(VECTOR pos, VECTOR target):
	ObjectBase(),
	m_shotTime()
{
	// ショットの初期化
	m_status.moveSpeed = kShotSpeed;
	m_status.radius = kShotRadius;
	// 有効時間
	m_shotTime = kShotTime;
	// ショットの位置と方向
	m_status.pos = pos;
	m_status.dir = VSub(target, pos);
	if (VSize(m_status.dir) > 0) m_status.dir = VNorm(m_status.dir);
	m_status.dir = VScale(m_status.dir, m_status.moveSpeed);
}

Shot::~Shot()
{
	// ショットの削除
	EffekseerManager::GetInstance().StopEffectTargetObj(this);
}

void Shot::Update()
{
	// 弾の有効時間
	m_shotTime--;
	if (m_shotTime <= 0)
	{
		// 有効時間が終了したら削除
		m_status.isEnabled = false;
	}
	// 弾の移動
	if (VSize(m_status.dir) > 0) m_status.dir = VNorm(m_status.dir);
	m_status.dir = VScale(m_status.dir, m_status.moveSpeed);
	m_status.pos = VAdd(m_status.pos, m_status.dir);
}

void Shot::Draw()
{
	// DrawSphere3D(m_status.pos, m_status.radius, 16, 0xe3e3e3, 0xe3e3e3, false);
}

void Shot::OnHit()
{
	// 当たった時の処理
	m_status.isEnabled = false;
}

void Shot::SetScale(float scale)
{
	// 弾の拡大率を変更
	if (scale > 0)
	{
		m_status.radius = kShotRadius * scale;
	}
	else
	{
		m_status.radius = kShotRadius;
	}
}
