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
	// ステージ背景オブジェクトの拡大率設定
	m_status.scale = kScale;
	// モデルのローカル座標を調整
	m_pModel->SetModelLocalPos(VGet(0.0f, -552.0f, 0.0f));
	// ステージ背景オブジェクトの拡大率設定
	m_pModel->SetScale(m_status.scale);
}

ObjectEnemyBase::~ObjectEnemyBase()
{
}

void ObjectEnemyBase::Update()
{
	// ショット生成間隔の更新
	m_shotDelay--;
	if (m_shotDelay < 0)
	{
		if (VSize(VSub(m_status.pos, m_targetPos)) < kShotDistanceMax
			 && VSize(VSub(m_status.pos, m_targetPos)) > kShotDistanceMin)
		{
			// ショット生成
			CreateShot();
		}
	}

	// 弾の更新
	for (auto& shot : m_pShots)
	{
		shot->Update();
	}

	// ショット削除
	DeleteDisableShot();

	// ベースの更新処理
	StageObjectBase::Update();
}

void ObjectEnemyBase::CreateShot()
{
	// ショット生成
	m_pShots.push_back(std::make_shared<Shot>(m_status.pos, m_targetPos));
	// ショットの速度設定
	m_pShots.back()->SetSpeed(kShotSpeed);
	// ショットエフェクト生成
	EffekseerManager::GetInstance().CreateEffect(EffectType::EnemyShot, false, m_pShots.back().get());
	// ショット音再生
	SoundManager::GetInstance().PlaySE(SoundType::shot);
	// ショット生成間隔の設定
	m_shotDelay = GetRand(kShotDelay);
}

void ObjectEnemyBase::DeleteDisableShot()
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

void ObjectEnemyBase::DeleteAllShot()
{
	// 全ショット削除
	m_pShots.clear();
}
