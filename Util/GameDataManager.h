#pragma once
#include <unordered_map>
#include "Stage.h"

namespace ScoreInfo
{
	// スコア情報
	static const int ROCK_SCORE = 30;
	static const int ENEMY_SCORE = 100;
	static const int BOSS_SCORE = 5000;
	static const int STAGECLEAR_SCORE = 10000;
	static const int HEAL_SCORE = 50;
	static const int MISS_SCORE = 100;
	static const int TIME_SCORE = 10;
}

/// <summary>
/// SEの管理を行うシングルトンクラス
/// </summary>
class GameDataManager
{
public:
	~GameDataManager();
	/// <summary>
	/// SoundManager使用者はGetInstance()を通した参照からしか利用できない
	/// </summary>
	/// <returns></returns>
	static GameDataManager& GetInstance()
	{
		// 唯一の実態
		static GameDataManager instance;
		// それの参照を返す
		return instance;
	}

	/* ステージ関係処理 */
	// 現在のステージ番号取得
	int GetCurrentStageNum() { return m_currentStageNum + 1; }
	// ステージクリア判定
	void OnStageClear();
	// 次のステージへ
	void OnNextStage();
	// ステージリセット
	void ResetStage() { m_currentStageNum = 0; }
	// ステージデータ取得
	StageData GetStageData(int stageNum)
	{
		return m_stageData[stageNum];
	}
	StageData GetStageData()
	{
		return m_stageData[m_currentStageNum];
	}

	/* ポイント関係処理 */
	void SetGamePoint(int point) { m_score = point; }
	// ポイントリセット
	void ResetPoint();
	// ポイント増加
	void AddPoint(int point);
	// ポイント減少
	void SubPoint(int point);

	// 初回プレイ判定セット
	void SetFirstPlay(bool isFirstPlay) { m_isFirstPlay = isFirstPlay; }

	/* 各種情報取得 */
	// ポイント取得
	int GetPoint() { return m_score; }
	// ランク取得
	std::string GetRank() { ConvertRank(); return m_rank; }
	// 全ステージクリア判定
	const bool IsAllStageClear() { return m_isAllStageClear; }
	// 初回プレイ判定
	const bool IsFirstPlay() { return m_isFirstPlay; }

private:
	// シングルトンパターンなのでコンストラクタはprivateに置く
	GameDataManager();
	// コピーも代入も禁止する
	GameDataManager(const GameDataManager&) = delete;		// コピーコンストラクタ
	void operator = (const GameDataManager&) = delete;		// 代入も禁止

	// ステージデータ読み込み
	void LoadStageData();

	// スコアをランクに変換
	void ConvertRank();

private:
	// スコアデータ
	int m_score;
	std::string m_rank;
	// ステージデータ
	std::unordered_map<int, StageData> m_stageData;
	// 現在のステージ番号
	int m_currentStageNum;
	// ステージクリア数カウント
	int m_stageClearCount;
	// 全ステージクリア判定
	bool m_isAllStageClear;
	// 初回プレイ判定
	bool m_isFirstPlay;
};