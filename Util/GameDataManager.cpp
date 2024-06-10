#include "GameDataManager.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace
{
	// ポイント最大値
	constexpr int kScoreMax = 999999;
}

GameDataManager::~GameDataManager()
{
}

void GameDataManager::OnStageClear()
{
	// ステージクリアカウントを増やす
	m_stageClearCount++;
	if (m_stageClearCount >= static_cast<int>(m_stageData.size()))
	{
		// 全ステージクリア判定
		m_isAllStageClear = true;
	}
}

void GameDataManager::OnNextStage()
{
	// 次のステージへ
	m_currentStageNum++;
	if (m_currentStageNum >= static_cast<int>(m_stageData.size()))
	{
		// 最終ステージの場合は最初のステージへ
		m_currentStageNum = 0;
	}
}

void GameDataManager::ResetPoint()
{
	// ポイントリセット
	m_score = 0;
}

void GameDataManager::AddPoint(int point)
{
	// ポイント加算
	m_score += point;
	if (m_score > kScoreMax)
	{
		// ポイント最大値を超えた場合は最大値に設定
		m_score = kScoreMax;
	}
}

void GameDataManager::SubPoint(int point)
{
	// ポイント減算
	m_score -= point;
	if (m_score < 0)
	{
		// ポイントがマイナスになった場合は0に設定
		m_score = 0;
	}
}

GameDataManager::GameDataManager() :
	m_score(0),
	m_rank(""),
	m_stageData(),
	m_currentStageNum(0),
	m_stageClearCount(0),
	m_isAllStageClear(false),
	m_isFirstPlay(true)
{
	// ステージデータ読み込み
	LoadStageData();
}

void GameDataManager::LoadStageData()
{
	std::ifstream file(Game::kStageDataFileName);	// CSVファイルを開く
	std::string line;								// 1行分の文字列
	std::vector<std::string> tempStageData;			// 一時ステージデータ
	int lineCount = 0;								// 行数
	int strFirst = 0, strLast = 0;					// 文字列の最初と最後

	// ファイルが開けたか確認
	if (file.is_open())
	{
		// ファイルから行を読み込む
		while (getline(file, line)) 
		{
			// 1行目はスキップ
			if (lineCount != 0)
			{
				// 区切り文字で文字列を分割
				strFirst = 0, strLast = 0;
				// 一時ステージデータリセット
				tempStageData.clear();
				for (int i = 0; i < line.size(); i++)
				{
					if (line[i] == ',')
					{
						// 区切り文字で文字列を分割
						strLast = i;
						// 文字列を取得
						std::string str;
						for (int j = strFirst; j < strLast; j++)
						{
							// 文字列を取得
							str += line[j];
						}
						// 一時ステージデータに追加
						tempStageData.push_back(str);
						// 次の文字へ
						strFirst = strLast + 1;
					}
				}
				
				// ステージデータに変換
				StageData stageData;
				stageData.isBoss = static_cast<bool>(std::stoi(tempStageData[0]));
				stageData.bossHp = std::stoi(tempStageData[1]);
				stageData.rockNum = std::stoi(tempStageData[2]);
				stageData.enemyNum = std::stoi(tempStageData[3]);
				stageData.length = static_cast<float>(std::stoi(tempStageData[4]));
				// ステージデータに追加
				m_stageData[lineCount - 1] = stageData;
			}
			// 行数をカウント
			lineCount++;
		}
		// ファイルを閉じる
		file.close();
	}
	else
	{
		// ステージデータ読み込みエラー処理
		MessageBox(NULL, "ステージデータ読み込み失敗", "", MB_OK);
	}
}

void GameDataManager::ConvertRank()
{
	// ランク変換
	if (m_score >= 12000)
	{
		m_rank = "S!!";
	}
	else if (m_score >= 6000)
	{
		m_rank = "A";
	}
	else if (m_score >= 3000)
	{
		m_rank = "B";
	}
	else if (m_score >= 1000)
	{
		m_rank = "C";
	}
	else if (m_score >= 500)
	{
		m_rank = "D";
	}
	else
	{
		m_rank = "E";
	}
}
