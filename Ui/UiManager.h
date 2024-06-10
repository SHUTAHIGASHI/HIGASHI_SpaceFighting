#pragma once
#include "Game.h"
#include <vector>

class UiManager
{
public:
	// コンストラクタ
	UiManager();
	// デストラクタ
	~UiManager();

	// 更新
	void Update();
	// 描画
	void Draw();

	// 不要なUI削除処理
	void DeleteUI();

	/* チャージショットのUI */
	// ショットUIの生成
	void CreateShotUI(int maxShot);
	// ショットUIの削除
	void DeleteShotUI();
	// ショットUIの更新
	void UpdateShotUI(int chargeNum);
	// ショットUIの描画位置設定
	void SetShotDrawPos(float x, float y);
	// ショットUI取得
	class UiBar* GetChargeShotUI();

	/* ボスのHPバー */
	// ボスUIの生成
	void CreateBossUI(int maxHp);
	// ボスUIの削除
	void DeleteBossUI();

	// 敵撃破数の更新
	void CountEnemyKill();

	// ポイント獲得時の処理
	void OnGetPoint(int point);
	// ポイント喪失時の処理
	void OnLosePoint(int point);

	// ステージ進行率の更新
	void SetStageRate(int rate);
	// ボスのHP描画位置設定
	void SetBossHpDrawPos(VECTOR pos);
	// プレイヤーのHPを設定
	void SetPlayerHp(int hp) { m_playerHp = hp; }
	// ボスのHPを設定
	void SetBossHp(int hp) { m_bossHp = hp; }
	// 目標敵撃破数の設定
	void SetEnemyKillCount(int count) { m_enemyKillCountMax = count; }
	// 描画テキスト切替判定
	void SetIsBossRate(bool isBossRate) { m_isBossRate = isBossRate; }
private:
	// ステージ目標の描画
	void DrawStageRate();
	// ステージ番号描画
	void DrawStageNum();

private:	
	// 描画テキスト切替判定
	bool m_isBossRate;

	// ステージ番号
	int m_stageNum;
	// HPバーの画像
	int m_hHpBarImg;
	// 背景画像
	int m_hBgImg;
	// 現在の各種HP
	int m_playerHp;
	int m_bossHp;
	// 敵撃破数
	int m_enemyKillCount;
	int m_enemyKillCountMax;
	// ステージ進行率
	int m_stageRate;

	// 獲得したポイントを一時的に描画する
	class UiPointBase* m_pPointUI = nullptr;
	// プレイヤーのHPバー
	class UiPlayerHpBar* m_pPlayerHp = nullptr;
	// プレイヤーのチャージショット
	class UiBar* m_pChargeShot = nullptr;
	// HPバー
	class UiBar* m_pBossHp = nullptr;
};
