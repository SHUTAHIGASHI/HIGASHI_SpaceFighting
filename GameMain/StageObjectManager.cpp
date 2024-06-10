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
	// オブジェクト生成間隔
	constexpr float kObjectCreateInterval = 2000.0f;
	// アイテム生成の確率
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
	// オブジェクトのクリア
	m_pObjects.clear();
	// ステージ、UIの解放
	m_pStage = nullptr;
	m_pUi = nullptr;
}

void StageObjectManager::Init()
{
	// 処理なし
}

void StageObjectManager::Update()
{
	// オブジェクト更新
	for (auto& obj : m_pObjects)
	{
		obj->Update();
		// 敵の場合はプレイヤーの座標を設定
		if (obj->GetType() == StageObjType::Enemy)
		{
			// ターゲット座標設定
			dynamic_cast<ObjectEnemyBase*>(obj.get())->SetTargetPos(m_playerPos);
		}
	}

	// 無効になったオブジェクトは排除
	DeleteDisableObject();

#ifdef _DEBUG
	//printfDx("ObjectNum:%d\n", m_pObjects.size());
#endif
}

void StageObjectManager::Draw()
{
	// オブジェクト描画
	for (auto& obj : m_pObjects)
	{
		obj->Draw();
	}
}

void StageObjectManager::CheckPlayerCollision(Player* player)
{
	// オブジェクトとの当たり判定
	for (auto& obj : m_pObjects)
	{
		// オブジェクトの更新処理
		obj->Update();
		// プレイヤーとの当たり判定チェック
		if (obj->CheckCollision(player))
		{
			// 被ダメージ時の処理
			obj->OnCollision();
			// プレイヤーに衝突したときの処理
			player->OnCollision(obj->GetType());
			continue;
		}

		// プレイヤーのショットとの当たり判定チェック
		for (auto& shot : player->GetShot())
		{
			// ショットとの当たり判定
			if (shot->CheckCollision(obj.get()))
			{
				// スコア加算
				if (obj->GetType() == StageObjType::Enemy)
				{
					// 敵撃破時のスコア加算
					GameDataManager::GetInstance().AddPoint(ScoreInfo::ENEMY_SCORE);
					// UI処理
					m_pUi->OnGetPoint(ScoreInfo::ENEMY_SCORE);
					// ステージの敵撃破数カウント
					m_pStage->CountKillEnemy();
				}
				else if (obj->GetType() == StageObjType::Rock)
				{
					// 岩破壊時のスコア加算
					GameDataManager::GetInstance().AddPoint(ScoreInfo::ROCK_SCORE);
					// UI処理
					m_pUi->OnGetPoint(ScoreInfo::ROCK_SCORE);
				}
				else
				{
					continue;
				}

				// 被ダメージ時の処理
				obj->OnCollision();
				// ショットの削除
				shot->OnHit();

				// アイテムの生成
				if (GetRand(kItemCreateRate) == 0)
				{
					// アイテム座標保存
					m_itemCreatePos.push_back(obj->GetPos());
				}
			}
		}

		// 敵の場合はショットの当たり判定チェック
		if (obj->GetType() == StageObjType::Enemy)
		{
			for (auto& shot : dynamic_cast<ObjectEnemyBase*>(obj.get())->GetShots())
			{
				if (player->CheckCollision(shot.get()))
				{
					// 被ダメージ時の処理
					player->OnDamage();
					// ショット削除
					shot->OnHit();
					continue;
				}
			}
		}
	}

	// アイテム生成
	for (auto& pos : m_itemCreatePos)
	{
		CreateObject("item", pos);
		// エフェクト生成
		EffekseerManager::GetInstance().CreateEffect(EffectType::Heal, false, pos);
	}
	// 生成座標クリア
	m_itemCreatePos.clear();
}

void StageObjectManager::CreateObject(std::string objName, VECTOR pos)
{
	// オブジェクト生成
	if (objName == "rock")
	{
		// 岩生成
		m_pObjects.push_back(std::make_shared<ObjectRock>());
	}
	else if (objName == "item")
	{
		// アイテム生成
		m_pObjects.push_back(std::make_shared<ObjectItem>());
	}
	else if (objName == "enemy")
	{
		// 敵生成
		m_pObjects.push_back(std::make_shared<ObjectEnemy>());
	}

	// 座標設定
	m_pObjects.back()->SetPos(pos);
}

void StageObjectManager::DeleteDisableObject()
{
	// 無効になったオブジェクトは排除
	auto rmIt = std::remove_if(m_pObjects.begin(), m_pObjects.end(),
		[](const std::shared_ptr<StageObjectBase>& obj)
		{
			return !obj->IsEnabled();
		});

	// 実際に範囲を指定して削除
	m_pObjects.erase(rmIt, m_pObjects.end());
}
