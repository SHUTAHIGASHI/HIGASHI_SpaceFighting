#pragma once
#include "Game.h"

struct StageData
{
	// ボスの出現判定
	bool isBoss = false;
	// ボス体力
	int bossHp = 0;
	// 岩の数
	int rockNum = 0;
	// 雑魚敵の数
	int enemyNum = 0;
	// ステージの長さ
	float length = 0.0f;
};

class Stage
{
public:
	// コンストラクタ
	Stage(StageData stageData);
	// デストラクタ
	virtual ~Stage();

	// プレイヤーポインタセット
	void SetPlayer(class Player* player) { m_pPlayer = player; }
	// UIポインタセット
	void SetUi(class UiManager* ui) { m_pUi = ui; }

	// 初期化
	void Init();
	// 更新
	void Update();
	// 描画
	void Draw();

	// 撃破数カウント
	void CountKillEnemy();

	// ステージクリア時の処理
	void OnStageClear();
	// ステージ終了時の処理
	void OnStageEnd();

	// ボス取得
	const class EnemyBoss* GetBoss() const
	{
		if (m_pBoss != nullptr) return m_pBoss;
		else return nullptr;
	}

	// クリア判定
	const bool IsClear() const { return m_isClear; }

private:
	// オブジェクト数の管理
	void CheckObjectNum();
	// ステージクリア判定チェック
	void CheckStageClear();
	// 雑魚敵数管理
	void EnemyNumManager();
	// ボス生成
	void CreateBoss();
	// ボスの更新処理
	void UpdateBoss();
	// ステージ進行処理
	void OnChangeProgress();
	// ランダムに位置指定
	VECTOR GetRandomPos();

private:
	// ステージデータ
	StageData m_stage;
	// 秒数カウント
	int m_countFrame;
	// 撃破数カウント
	int m_killEnemyNum;
	// オブジェクト削除範囲
	float m_deleteRange;
	// ステージの長さ
	float m_stageLengthMax;
	// クリア判定
	bool m_isClear;

private:
	// オブジェクト管理
	std::shared_ptr<class StageObjectManager> m_pObjectManager;

	// ボス
	class EnemyBoss* m_pBoss;

	// プレイヤー
	class Player* m_pPlayer;

	// UI
	class UiManager* m_pUi;
};

