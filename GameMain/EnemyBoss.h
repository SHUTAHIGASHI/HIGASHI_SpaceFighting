#pragma once
#include "ObjectBase.h"
#include <memory>
#include <vector>
#include "game.h"

class EnemyBoss : public ObjectBase
{
public:
	// コンストラクタ
	EnemyBoss(VECTOR spawnPos, float speed, int maxHp);
	// デストラクタ
	~EnemyBoss();

	// 更新
	void Update();
	// 描画
	void Draw();

	// ダメージ処理
	void OnDamage();
	// 死亡時の処理
	void OnDead();

	// ボス移動速度設定
	void SetSpeed(float speed) { m_status.moveSpeed = speed; }
	// ボスの攻撃ターゲット位置設定
	void SetTargetPos(VECTOR targetPos) { m_targetPos = targetPos; }

	// Getter
	const std::vector<std::shared_ptr<class Shot>> GetShot() const { return m_pShots; }
	const int GetMaxHp() const { return m_maxHp; }

private:
	// 不要なショットの削除
	void DeleteDisableShot();

	// メンバ関数ポインタ(更新)
	using m_tUpdateFunc = void (EnemyBoss::*) ();
	m_tUpdateFunc m_updateFunc = nullptr;
	void SpawnUpdate();		// 通常の更新処理
	void NormalUpdate();	// 通常の更新処理
	void DeadUpdate();		// ゲーム終了時の更新処理
private:
	// ボスの固定位置
	VECTOR m_fixedPos;
	// 攻撃ターゲット座標
	VECTOR m_targetPos;
	// 角度
	float m_angle;
	// 回転速度
	float m_rotaSpeed;
	// 攻撃間隔
	int m_shotDelay;
	// ボスの最大体力
	int m_maxHp;
	// 死亡判定
	bool m_isDead;

	// モデル
	std::shared_ptr<class Model> m_pModel;
	// ボスの攻撃
	std::vector<std::shared_ptr<class Shot>> m_pShots;
};

