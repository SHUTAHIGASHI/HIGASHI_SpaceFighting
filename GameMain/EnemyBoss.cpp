#include "EnemyBoss.h"
#include "Model.h"
#include "EffekseerManager.h"
#include "SoundManager.h"
#include "Shot.h"
#include "Load.h"

namespace
{
	// ボスの回転速度
	constexpr float kRotateSpeed = 0.02f;
	// 当たり判定半径
	constexpr float kCollisionRadius = 600.0f;
	// 拡大率
	constexpr float kScale = 5.0f;
	// ボスの無敵時間
	constexpr int kInvTime = 1;
	// ボスの死亡後の無敵時間
	constexpr int kDeadInvTime = 60 * 6;
	// ボスの攻撃間隔
	constexpr int kShotDelay = 60;
	// ボスのスポーンする高さ
	constexpr float kSpawnHeight = 10000.0f;
	// ボスの削除する高さ
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
	// ステータス初期化
	m_status.hp = m_maxHp;
	m_status.pos = VGet(spawnPos.x, spawnPos.y + kSpawnHeight, spawnPos.z);
	m_fixedPos = spawnPos;
	m_status.moveSpeed = speed;
	m_status.radius = kCollisionRadius;
	// モデル生成
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
	// 無敵時間更新
	if (m_status.invTime > 0)
	{
		m_status.invTime--;
	}

	// Update処理のメンバ関数ポインタ
	(this->*m_updateFunc)();

	// 角度更新
	m_angle -= m_rotaSpeed;
	if(m_angle < -DX_PI_F * 2)
	{
		m_angle = 0.0f;
	}

	// 移動
	m_status.pos.z += m_status.moveSpeed;
	// 座標更新
	m_pModel->SetPosition(m_status.pos);
	// 回転
	m_pModel->SetRotation(VGet(0.0f, m_angle, 0.0f));

	// エフェクト座標更新
	if (m_status.isDead)
	{
		EffekseerManager::GetInstance().SetEffectPosition(EffectType::Dead, m_status.pos);
	}
}

void EnemyBoss::Draw()
{
	// 描画
	if (m_status.invTime % 2 == 0)
	{
		m_pModel->Draw();
	}

	// 当たり判定
	//DrawSphere3D(VAdd(m_status.pos, m_status.localPos), m_status.radius, 16, GetColor(255, 255, 255), GetColor(255, 255, 255), false);
}

void EnemyBoss::OnDamage()
{
	if (m_status.invTime > 0)
	{
		return;
	}

	// 無敵時間設定
	m_status.invTime = kInvTime;
	// ダメージ音再生
	SoundManager::GetInstance().PlaySE(SoundType::damage);
	// ダメージエフェクト再生
	EffekseerManager::GetInstance().CreateEffect(EffectType::BossDamage, false, this);

	// ダメージ処理
	m_status.hp--;
	if (m_status.hp <= 0)
	{
		OnDead();
	}
}

void EnemyBoss::OnDead()
{
	// 死亡時の更新処理
	m_updateFunc = &EnemyBoss::DeadUpdate;
	// 無敵時間設定
	m_status.invTime = kDeadInvTime;
	// 死亡フラグON
	m_status.isDead = true;
	// 死亡時のエフェクト処理
	EffekseerManager::GetInstance().CreateEffect(EffectType::Dead, false, this);
	// 死亡音再生
	SoundManager::GetInstance().PlaySE(SoundType::dead);
}

void EnemyBoss::DeleteDisableShot()
{
	// いなくなったショットは排除
	auto rmIt = std::remove_if(m_pShots.begin(), m_pShots.end(),
		[](const std::shared_ptr<Shot>& shot)
		{
			return !shot->IsEnabled();
		});

	// 実際に範囲を指定して削除
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
		// ショット生成
		m_pShots.push_back(std::make_shared<Shot>(m_status.pos, m_targetPos));
		// ショットエフェクト生成
		EffekseerManager::GetInstance().CreateEffect(EffectType::EnemyShot, false, m_pShots.back().get());
		// ショット音再生
		SoundManager::GetInstance().PlaySE(SoundType::shot);
		m_shotDelay = kShotDelay;
	}

	// 弾の更新
	for (auto& shot : m_pShots)
	{
		shot->Update();
	}

	// ショット削除
	DeleteDisableShot();
}

void EnemyBoss::DeadUpdate()
{
	// 落下処理
	m_status.pos.y -= 24.0f;
	// 回転速度更新
	m_rotaSpeed += 0.005f;

	if (m_status.pos.y < -kDeleteHeight)
	{
		// ボスの無効化
		m_status.isEnabled = false;
		// エフェクトの停止
		EffekseerManager::GetInstance().SetEffectPosition(EffectType::Dead, Game::kVecZero);
		EffekseerManager::GetInstance().StopEffect(EffectType::Dead);
	}
}
