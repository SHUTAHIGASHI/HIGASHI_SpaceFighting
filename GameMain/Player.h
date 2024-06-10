#pragma once
#include "ObjectBase.h"
#include "Game.h"
#include "StageObjectBase.h"
#include <vector>
#include <deque>

class Model;
class GameScore;
class CameraController;
class UiManager;

namespace PlayerInfo
{
	// 最大HP
#ifdef _DEBUG
	constexpr int kHpMax = 30;
#else
	constexpr int kHpMax = 20;
#endif
}

class Player: public ObjectBase
{
public:
	// それぞれのポインタセット
	void SetScore(GameScore* score) { m_pScore = score; }
	void SetCamera(CameraController* camera) { m_pCamera = camera; }
	void SetUi(UiManager* ui) { m_pUi = ui; }

	// コンストラクタ
	Player();
	// デストラクタ
	virtual ~Player();

	// 初期化
	void Init();
	// 更新
	void Update(const InputState& input);
	// 描画
	void Draw();

	// 被ダメージ時
	void OnDamage();
	// 攻撃発射時
	void OnAttack();
	// チャージショット発射時
	void OnChargeAttack();
	// 死亡時
	void OnDead();
	// 衝突時
	void OnCollision(StageObjType type);
	// 回復
	void OnHeal();

	// クロスヘア描画判定
	void SetCrosshair(bool i) { m_isDrawCrosshair = i; }

	// 各取得関数
	const float GetDistance() const { return m_status.pos.z; }
	const int GetCharge() const { return 0; }
	std::vector<std::shared_ptr<class Shot>> GetShot() const { return m_pShot; }

private:// プライベート関数
	// 入力制御
	void ControllMove(const InputState& input);
	// ショット管理
	void ControllShot(const InputState& input);
	// プレイヤーの移動可能範囲チェック
	void CheckMoveLimit();
	// 死亡判定チェック
	void CheckDead();
	// クロスヘア描画
	void DrawCrosshair();
	// プレイヤーのショット削除
	void DeleteDisableShot();

	// メンバ関数ポインタ(更新)
	using m_tUpdateFunc = void (Player::*) (const InputState& input);
	m_tUpdateFunc m_updateFunc = nullptr;
	void NormalUpdate(const InputState& input);     // 通常の更新処理
	void GameEndUpdate(const InputState& input);	// ゲーム終了時の更新処理

private:// プライベート変数：データ関係
	// クロスヘア画像ハンドル
	int m_hCrosshairImg;
	int m_hCrosshairImg2;

private:// プライベート変数：ステータス関係
	// 回避ベクトル
	VECTOR m_avoidDir;
	// プレイヤーのショット遅延
	int m_shotDelay;
	// チャージショット率
	int m_chargeRate;
	// チャージショットカウント
	int m_chargeCount;
	// チャージショット反射時間
	int m_chargeShotTime;
	// チャージショットボタン押下カウント
	int m_chargeShotButtonDelay;
	// 回避カウント
	int m_avoidCount;
	// 回避時回転速度
	float m_avoidRotaSpeed;
	// 回転位置
	float m_playerRotaAxisZ;
	// 回避中
	bool m_isAvoid;
	// クロスヘア判定
	bool m_isDrawCrosshair;

private:// クラス宣言
	// ショット
	std::vector<std::shared_ptr<class Shot>> m_pShot;
	// モデル
	std::shared_ptr<Model> m_pModel;
	// スコア
	GameScore* m_pScore;
	// カメラ
	CameraController* m_pCamera;
	// UI
	UiManager* m_pUi;
};
