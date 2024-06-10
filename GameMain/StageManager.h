#pragma once
#include "Game.h"
#include <vector>
#include <memory>

class StageManager
{
public:
	// コンストラクタ
	StageManager();
	// デストラクタ
	virtual ~StageManager();

	// プレイヤーポインタセット
	void SetPlayer(class Player* player) { m_pPlayer = player; }
	// UIポインタセット
	void SetUi(class UiManager* ui) { m_pUi = ui; }

	// 初期化
	void Init();
	// 更新
	void Update(const InputState& input);
	// 描画
	void Draw();

	// ステージ終了時の処理
	void OnStageEnd();

	// ボス取得
	const class EnemyBoss* GetBoss() const;

	// ステージクリア判定
	const bool IsClear() const;

private:
	// マップ
	std::shared_ptr<class Map> m_pMap;
	// ステージ管理
	std::shared_ptr<class Stage> m_pStage;
	// プレイヤー
	class Player* m_pPlayer;
	// UI
	class UiManager* m_pUi;
};

